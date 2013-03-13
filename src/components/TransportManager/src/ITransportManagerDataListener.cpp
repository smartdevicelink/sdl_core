/**
 * \file ITransportManagerDataListener.cpp
 * \brief Class ITransportManagerDataListener.
 * Copyright (c) 2013 Ford Motor Company
 */

#include "TransportManager/ITransportManagerDataListener.hpp"

NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener::~ITransportManagerDataListener()
{
}

void NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener::onFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
}

void NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener::onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
