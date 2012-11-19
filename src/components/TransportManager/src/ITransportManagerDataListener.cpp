#include "TransportManager/ITransportManagerDataListener.hpp"

NsAppLink::NsTransportManager::ITransportManagerDataListener::~ITransportManagerDataListener()
{
}

void NsAppLink::NsTransportManager::ITransportManagerDataListener::onFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDataListener::onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
