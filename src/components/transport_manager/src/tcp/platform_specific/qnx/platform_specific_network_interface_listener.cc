#include "transport_manager/tcp/platform_specific/qnx/platform_specific_network_interface_listener_impl.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

PlatformSpecificNetworkInterfaceListener::
    PlatformSpecificNetworkInterfaceListener(
        TcpClientListener* tcp_client_listener,
        const std::string designated_interface) {}

PlatformSpecificNetworkInterfaceListener::
    ~PlatformSpecificNetworkInterfaceListener() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool PlatformSpecificNetworkInterfaceListener::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  return true;
}

void PlatformSpecificNetworkInterfaceListener::Deinit() {
  LOG4CXX_AUTO_TRACE(logger_);
}

bool PlatformSpecificNetworkInterfaceListener::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  return true;
}

bool PlatformSpecificNetworkInterfaceListener::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
