#include "transport_manager/websocket_server/websocket_listener.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/websocket_server/websocket_device.h"
#include "utils/file_system.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOG_VARIABLE("WebSocketListener")

WebSocketListener::WebSocketListener(TransportAdapterController* controller,
                                     const TransportManagerSettings& settings,
                                     const int num_threads)
    : controller_(controller)
    , ioc_(num_threads)
#ifdef ENABLE_SECURITY
    , ctx_(ssl::context::sslv23)
    , start_secure_(false)
#endif  // ENABLE_SECURITY
    , acceptor_(ioc_)
    , socket_(ioc_)
    , io_pool_(new boost::asio::thread_pool(num_threads))
    , num_threads_(num_threads)
    , shutdown_(false)
    , settings_(settings) {
}

WebSocketListener::~WebSocketListener() {
  Terminate();
}

TransportAdapter::Error WebSocketListener::Init() {
  SDL_LOG_AUTO_TRACE();
  const auto old_shutdown_value = shutdown_.exchange(false);
  if (old_shutdown_value) {
    ioc_.restart();
    io_pool_.reset(new boost::asio::thread_pool(num_threads_));
  }
  return StartListening();
}

void WebSocketListener::Terminate() {
  SDL_LOG_AUTO_TRACE();
  Shutdown();
}

TransportAdapter::Error WebSocketListener::StartListening() {
  SDL_LOG_AUTO_TRACE();
  if (acceptor_.is_open()) {
    return TransportAdapter::OK;
  }

#ifdef ENABLE_SECURITY
  auto const ta_error = AddCertificateAuthority();
  if (TransportAdapter::OK != ta_error) {
    return ta_error;
  }
#endif

  auto const address =
      boost::asio::ip::make_address(settings_.websocket_server_address());
  tcp::endpoint endpoint = {address, settings_.websocket_server_port()};

  // Open the acceptor
  boost::system::error_code ec;
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    auto str_err = "ErrorOpen: " + ec.message();
    SDL_LOG_ERROR(str_err << " host/port: " << endpoint.address().to_string()
                          << "/" << endpoint.port());
    return TransportAdapter::FAIL;
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if (ec) {
    std::string str_err = "ErrorSetOption: " + ec.message();
    SDL_LOG_ERROR(str_err << " host/port: " << endpoint.address().to_string()
                          << "/" << endpoint.port());
    return TransportAdapter::FAIL;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
    std::string str_err = "ErrorBind: " + ec.message();
    SDL_LOG_ERROR(str_err << " host/port: " << endpoint.address().to_string()
                          << "/" << endpoint.port());
    return TransportAdapter::FAIL;
  }

  // Start listening for connections
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    std::string str_err = "ErrorListen: " + ec.message();
    SDL_LOG_ERROR(str_err << " host/port: " << endpoint.address().to_string()
                          << "/" << endpoint.port());
    return TransportAdapter::FAIL;
  }

  if (false == Run()) {
    return TransportAdapter::FAIL;
  }

  return TransportAdapter::OK;
}

#ifdef ENABLE_SECURITY
TransportAdapter::Error WebSocketListener::AddCertificateAuthority() {
  SDL_LOG_AUTO_TRACE();

  const auto cert_path = settings_.ws_server_cert_path();
  SDL_LOG_DEBUG("Path to certificate : " << cert_path);
  const auto key_path = settings_.ws_server_key_path();
  SDL_LOG_DEBUG("Path to key : " << key_path);
  const auto ca_cert_path = settings_.ws_server_ca_cert_path();
  SDL_LOG_DEBUG("Path to ca cert : " << ca_cert_path);
  start_secure_ = settings_.wss_server_supported();

  if (start_secure_ && (!file_system::FileExists(cert_path) ||
                        !file_system::FileExists(key_path) ||
                        !file_system::FileExists(ca_cert_path))) {
    SDL_LOG_ERROR("Certificate or key file not found");
    return TransportAdapter::FAIL;
  }

  if (!start_secure_) {
    auto check_config = [](const std::string& config,
                           const std::string config_name) {
      bool start_unsecure = config.empty();
      if (!start_unsecure) {
        SDL_LOG_ERROR("Configuration for secure WS is incomplete. "
                      << config_name
                      << " config is "
                         "present, meanwhile others may be missing. Please "
                         "check INI file");
      }
      return start_unsecure;
    };
    if (!check_config(cert_path, "Server cert") ||
        !check_config(key_path, "Server key") ||
        !check_config(ca_cert_path, "CA cert")) {
      return TransportAdapter::FAIL;
    }
  } else {
    SDL_LOG_INFO("WebSocket server will start secure connection");
    ctx_.add_verify_path(cert_path);
    ctx_.set_options(boost::asio::ssl::context::default_workarounds);
    using context = boost::asio::ssl::context_base;
    ctx_.set_verify_mode(ssl::verify_peer | ssl::verify_fail_if_no_peer_cert);
    boost::system::error_code sec_ec;
    ctx_.use_certificate_chain_file(cert_path, sec_ec);
    ctx_.load_verify_file(ca_cert_path);
    if (sec_ec) {
      SDL_LOG_ERROR(
          "Loading WS server certificate failed: " << sec_ec.message());
      return TransportAdapter::FAIL;
    }
    sec_ec.clear();
    ctx_.use_private_key_file(key_path, context::pem, sec_ec);
    if (sec_ec) {
      SDL_LOG_ERROR("Loading WS server key failed: " << sec_ec.message());
      return TransportAdapter::FAIL;
    }
  }

  return TransportAdapter::OK;
}
#endif  // ENABLE_SECURITY

bool WebSocketListener::Run() {
  SDL_LOG_AUTO_TRACE();
  const bool is_connection_open = WaitForConnection();
  if (is_connection_open) {
    boost::asio::post(*io_pool_.get(), [&]() { ioc_.run(); });
  } else {
    SDL_LOG_ERROR("Connection is shutdown or acceptor isn't open");
  }

  return is_connection_open;
}

bool WebSocketListener::WaitForConnection() {
  SDL_LOG_AUTO_TRACE();
  if (!shutdown_ && acceptor_.is_open()) {
    acceptor_.async_accept(
        socket_,
        std::bind(
            &WebSocketListener::StartSession, this, std::placeholders::_1));
    return true;
  }
  return false;
}

template <>
void WebSocketListener::ProcessConnection(
    std::shared_ptr<WebSocketConnection<WebSocketSession<> > > connection,
    const DeviceSptr device,
    const ApplicationHandle app_handle) {
  SDL_LOG_AUTO_TRACE();
  controller_->ConnectionCreated(
      connection, device->unique_device_id(), app_handle);

  controller_->ConnectDone(device->unique_device_id(), app_handle);

  connection->Run();

  connection_list_lock.Acquire();
  connection_list_.push_back(connection);
  connection_list_lock.Release();

  WaitForConnection();
}

#ifdef ENABLE_SECURITY
template <>
void WebSocketListener::ProcessConnection(
    std::shared_ptr<WebSocketConnection<WebSocketSecureSession<> > > connection,
    const DeviceSptr device,
    const ApplicationHandle app_handle) {
  SDL_LOG_AUTO_TRACE();
  controller_->ConnectionCreated(
      connection, device->unique_device_id(), app_handle);

  controller_->ConnectDone(device->unique_device_id(), app_handle);

  connection->Run();

  connection_list_lock.Acquire();
  connection_list_.push_back(connection);
  connection_list_lock.Release();

  WaitForConnection();
}
#endif  // ENABLE_SECURITY

void WebSocketListener::StartSession(boost::system::error_code ec) {
  SDL_LOG_AUTO_TRACE();
  if (ec) {
    std::string str_err = "ErrorAccept: " + ec.message();
    SDL_LOG_ERROR(str_err);
    return;
  }

  if (shutdown_) {
    return;
  }

  const ApplicationHandle app_handle = socket_.native_handle();

  std::shared_ptr<WebSocketDevice> device =
      std::static_pointer_cast<WebSocketDevice>(
          controller_->GetWebEngineDevice());

  SDL_LOG_INFO("Connected client: " << app_handle);

#ifdef ENABLE_SECURITY
  if (start_secure_) {
    auto connection =
        std::make_shared<WebSocketConnection<WebSocketSecureSession<> > >(
            device->unique_device_id(),
            app_handle,
            std::move(socket_),
            ctx_,
            controller_);
    ProcessConnection(connection, device, app_handle);
    return;
  }
#endif  // ENABLE_SECURITY

  auto connection = std::make_shared<WebSocketConnection<WebSocketSession<> > >(
      device->unique_device_id(), app_handle, std::move(socket_), controller_);
  ProcessConnection(connection, device, app_handle);
}

void WebSocketListener::Shutdown() {
  SDL_LOG_AUTO_TRACE();
  if (false == shutdown_.exchange(true)) {
    ioc_.stop();
    socket_.close();
    boost::system::error_code ec;
    acceptor_.close(ec);

    if (ec) {
      SDL_LOG_ERROR("Acceptor closed with error: " << ec);
    }

    io_pool_->stop();
    io_pool_->join();
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
