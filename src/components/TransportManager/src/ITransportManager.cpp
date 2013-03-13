/**
 * \file ITransportManager.cpp
 * \brief Class ITransportManager.
 * Copyright (c) 2013 Ford Motor Company
 */

#include "CTransportManager.hpp"

NsSmartDeviceLink::NsTransportManager::ITransportManager * NsSmartDeviceLink::NsTransportManager::ITransportManager::create(void)
{
    return new CTransportManager();
}

NsSmartDeviceLink::NsTransportManager::ITransportManager::~ITransportManager(void)
{
}
