#include "TransportManager/ITransportManagerDataListener.hpp"

NsAppLink::NsTransportManager::ITransportManagerDataListener::~ITransportManagerDataListener()
{

}

void NsAppLink::NsTransportManager::ITransportManagerDataListener::onFrameReceived(tConnectionHandle ConnectionHandle, const Blob & Data)
{
}

void NsAppLink::NsTransportManager::ITransportManagerDataListener::onFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
{
}
