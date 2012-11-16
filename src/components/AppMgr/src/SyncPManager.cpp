#include "AppMgr/SyncPManager.h"
#include "AppMgr/Base64.h"

namespace NsAppManager
{

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
    mPData = data;
}

/**
 * \brief get base64-encoded PData
 * \return vector of strings of base64-encoded information
 */
const SyncPManager::PData& SyncPManager::getPData() const
{
    return mPData;
}

/**
 * \brief set raw string data
 * \param data vector of strings
 */
void SyncPManager::setRawData(const SyncPManager::RawData &data)
{
    mPData.clear();
    for(RawData::const_iterator it = data.begin(); it != data.end(); it++)
    {
        const std::string& rawString = *it;
        std::string pData = base64_encode((const unsigned char*)rawString.c_str(), rawString.length());
        mPData.push_back(pData);
    }
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
