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

const SyncPManager::PData &SyncPManager::getPData() const
{
    return mPData;
}

SyncPManager::SyncPManager()
{
}

SyncPManager::SyncPManager(const SyncPManager &)
{
}

}
