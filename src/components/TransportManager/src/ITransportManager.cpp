#include "CTransportManager.hpp"

NsAppLink::NsTransportManager::ITransportManager * NsAppLink::NsTransportManager::ITransportManager::create(void)
{
    return new CTransportManager();
}

NsAppLink::NsTransportManager::ITransportManager::~ITransportManager(void)
{
}
