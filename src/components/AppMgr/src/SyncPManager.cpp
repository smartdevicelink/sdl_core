//
// Copyright (c) 2013 Ford Motor Company
//

#include "AppMgr/SyncPManager.h"
#include "Utils/Base64.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace NsAppManager
{

    log4cplus::Logger SyncPManager::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("SyncPManager"));

    /**
     * \brief Default class destructor
     */
    SyncPManager::~SyncPManager()
    {
        mPData.clear();
    }

    /**
     * \brief set base64-encoded PData
     * \param data vector of strings of base64-encoded information
     */
    void SyncPManager::setPData(const SyncPManager::PData &data, const std::string& appName, const int& methodId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Setting PData of length " << data.size() );
        mPData = data;
        std::stringstream stringStream((std::stringstream::in | std::stringstream::out));
        stringStream << appName << "_" << methodId;
        std::string fileName;
        stringStream >> fileName;
        serializeToFile( fileName, mPData );
        LOG4CPLUS_INFO_EXT(mLogger, "PData of length " << data.size() << " serialized to file " << fileName );
    }

    /**
     * \brief get base64-encoded PData
     * \return vector of strings of base64-encoded information
     */
    const SyncPManager::PData& SyncPManager::getPData() const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Getting PData of length " << mPData.size() );
        return mPData;
    }

    /**
     * \brief set raw string data
     * \param data vector of strings
     */
    void SyncPManager::setRawData(const SyncPManager::RawData &data)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Setting raw data of length " << data.size() );
        mPData.clear();
        for(RawData::const_iterator it = data.begin(); it != data.end(); it++)
        {
            const std::string& rawString = *it;
            std::string pData = base64_encode((const unsigned char*)rawString.c_str(), rawString.length());
            mPData.push_back(pData);
        }
        LOG4CPLUS_INFO_EXT(mLogger, "PData now is of mength length " << mPData.size() );
    }

    /**
     * \brief set raw string data
     * \return vector of strings
     */
    SyncPManager::RawData SyncPManager::getRawData() const
    {
        RawData rawData;
        for(PData::const_iterator it = mPData.begin(); it != mPData.end(); it++)
        {
            std::string rawString = base64_decode(*it);
            rawData.push_back(rawString);
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Getting raw data of length " << rawData.size() );
        return rawData;
    }

    /**
     * \brief Default class constructor
     */
    SyncPManager::SyncPManager()
    {
    }

    /**
     * \brief Copy constructor
     */
    SyncPManager::SyncPManager(const SyncPManager &)
    {
    }

    /**
     * \brief serialize a string vector to the text file
     * \param fileName name of the file to serialize to
     * \param value a value to serialize
     * \return success of an operation - true or false
     */
    bool SyncPManager::serializeToFile(const std::string &fileName, const PData &value) const
    {
        if(fileName.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " AppMgrCore cannot serialize to a file: a filename is empty!");
            return false;
        }
        if(!value.empty())
        {
            std::ofstream file(fileName, std::ios::out | std::ios::trunc);
            if(file.is_open())
            {
                for(PData::const_iterator it = value.begin(); it != value.end(); it++)
                {
                    file << *it << std::endl;
                }
                LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore successfully serialized a vector of size " <<value.size() << " to a file " << fileName);
                file.close();
                return true;
            }
            else
            {
                LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot serialize to a file: error creating file!");
                return false;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot serialize to a file: value is empty!");
        return false;
    }

    /**
     * \brief deserialize a string vector from the text file
     * \param fileName name of the file to deserialize from
     * \param value a value to deserialize
     * \return success of an operation - true or false
     */
    bool SyncPManager::deserializeFromFile(const std::string &fileName, PData &value)
    {
        value.empty();
        std::ifstream file(fileName);
        if( file.is_open() )
        {
            if( file.good() )
            {
                for ( std::string line; getline(file, line); )
                {
                    value.push_back(line);
                }
            }
            file.close();
            LOG4CPLUS_INFO_EXT(mLogger, "PData of length " << value.size() << " deserialized from file " << fileName );
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot deserialize a file: probably file doesn't exist!");
        }
        return false;
    }

}
