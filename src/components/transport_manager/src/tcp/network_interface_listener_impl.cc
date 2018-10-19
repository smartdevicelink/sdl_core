#include "transport_manager/tcp/network_interface_listener_impl.h"
#include "platform_specific_network_interface_listener_impl.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

NetworkInterfaceListenerImpl::NetworkInterfaceListenerImpl(
    TcpClientListener* tcp_client_listener,
    const std::string designated_interface)
    : platform_specific_impl_(new PlatformSpecificNetworkInterfaceListener(
          tcp_client_listener, designated_interface)) {
  LOG4CXX_AUTO_TRACE(logger_);
}

NetworkInterfaceListenerImpl::~NetworkInterfaceListenerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool NetworkInterfaceListenerImpl::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  return platform_specific_impl_->Init();
}

void NetworkInterfaceListenerImpl::Deinit() {
  LOG4CXX_AUTO_TRACE(logger_);
  platform_specific_impl_->Deinit();
}

bool NetworkInterfaceListenerImpl::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  return platform_specific_impl_->Start();
}

bool NetworkInterfaceListenerImpl::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  return platform_specific_impl_->Stop();
}

}  // namespace transport_adapter
}  // namespace transport_manager
