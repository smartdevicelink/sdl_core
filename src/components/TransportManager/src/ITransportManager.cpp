#include "CTransportManager.hpp"

AppLink::TransportManager::ITransportManager * AppLink::TransportManager::ITransportManager::create(void)
{
    return new CTransportManager();
}

AppLink::TransportManager::ITransportManager::~ITransportManager(void)
{
}
