#include "transport_manager/tcp/platform_specific/qnx/platform_specific_network_interface_listener_impl.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

PlatformSpecificNetworkInterfaceListener::
    PlatformSpecificNetworkInterfaceListener(
        TcpClientListener* tcp_client_listener,
        const std::string designated_interface) {}

PlatformSpecificNetworkInterfaceListener::
    ~PlatformSpecificNetworkInterfaceListener() {
  SDL_LOG_AUTO_TRACE();
}

bool PlatformSpecificNetworkInterfaceListener::Init() {
  SDL_LOG_AUTO_TRACE();
  return true;
}

void PlatformSpecificNetworkInterfaceListener::Deinit() {
  SDL_LOG_AUTO_TRACE();
}

bool PlatformSpecificNetworkInterfaceListener::Start() {
  SDL_LOG_AUTO_TRACE();
  return true;
}

bool PlatformSpecificNetworkInterfaceListener::Stop() {
  SDL_LOG_AUTO_TRACE();
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
