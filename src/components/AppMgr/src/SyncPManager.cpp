#include "AppMgr/SyncPManager.h"
#include "AppMgr/Base64.h"

namespace NsAppManager
{

SyncPManager::~SyncPManager()
{
    mPData.clear();
}

void SyncPManager::setPData(const SyncPManager::PData &data)
{
    mPData = data;
}

const SyncPManager::PData& SyncPManager::getPData() const
{
    return mPData;
}

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

SyncPManager::SyncPManager()
{
}

SyncPManager::SyncPManager(const SyncPManager &)
{
}

}
