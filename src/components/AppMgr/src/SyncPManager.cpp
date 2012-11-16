#include "AppMgr/SyncPManager.h"
#include "AppMgr/Base64.h"

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
void SyncPManager::setPData(const SyncPManager::PData &data)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Setting PData of length " << data.size() );
    mPData = data;
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

}
