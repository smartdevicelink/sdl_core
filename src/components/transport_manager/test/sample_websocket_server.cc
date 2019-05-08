#include "transport_manager/cloud/sample_websocket_server.h"

namespace {
// Report a failure
void Fail(char const* tag, boost::system::error_code ec) {
  std::cerr << tag << ": " << ec.message() << "\n";
}
}  // namespace

namespace sample {
namespace websocket {

WSSession::WSServer::WSServer(tcp::socket&& socket)
    : ws_(std::move(socket)), strand_(ws_.get_executor()) {}

void WSSession::WSServer::Run() {
  // Accept the websocket handshake
  ws_.async_accept(boost::asio::bind_executor(
      strand_,
      std::bind(
          &WSServer::OnAccept, shared_from_this(), std::placeholders::_1)));
}

void WSSession::WSServer::OnAccept(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_CONNECTION_ACCEPT", ec);
  }
}

WSSession::WSSession(const std::string& address, uint16_t port)
    : address_(address), port_(port), acceptor_(ioc_), socket_(ioc_) {
  endpoint_ = {boost::asio::ip::make_address(address), port};
  boost::system::error_code error;

  // Open the acceptor
  acceptor_.open(endpoint_.protocol(), error);
  if (error) {
    Fail("ERROR_ACCEPTOR_OPEN", error);
    return;
  }

  // Allow address reuse
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    Fail("ERROR_SET_OPTION", error);
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint_, error);
  if (error) {
    Fail("ERROR_BIND", error);
    return;
  }

  // Start listening for connections
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);
  if (error) {
    Fail("ERROR_LISTEN", error);
    return;
  }
}

void WSSession::Run() {
  if (acceptor_.is_open()) {
    acceptor_.async_accept(
        socket_, std::bind(&WSSession::on_accept, this, std::placeholders::_1));
    ioc_.run();
  }
}

void WSSession::Stop() {
  try {
    ioc_.stop();
    acceptor_.close();
  } catch (...) {
    std::cerr << "Failed to close connection" << std::endl;
  }
}

void WSSession::on_accept(boost::system::error_code ec) {
  if (ec) {
    Fail("ERROR_ON_ACCEPT", ec);
    ioc_.stop();
    return;
  }

  // Make websocket object and start
  ws_ = std::make_shared<WSServer>(std::move(socket_));
  ws_->Run();
}

WSSSession::WSSServer::WSSServer(tcp::socket&& socket, ssl::context& ctx)
    : wss_(std::move(socket), ctx) {}

void WSSSession::WSSServer::Run() {
  // Perform the SSL handshake
  wss_.next_layer().async_handshake(ssl::stream_base::server,
                                    std::bind(&WSSServer::OnSSLHandshake,
                                              shared_from_this(),
                                              std::placeholders::_1));
}

void WSSSession::WSSServer::OnSSLHandshake(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_SSL_HANDSHAKE", ec);
  }

  // Accept the websocket handshake
  wss_.async_accept(std::bind(
      &WSSServer::OnAccept, shared_from_this(), std::placeholders::_1));
}

void WSSSession::WSSServer::OnAccept(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_ON_ACCEPT", ec);
  }
}

WSSSession::WSSSession(const std::string& address,
                       uint16_t port,
                       const std::string& certificate,
                       const std::string& private_key)
    : acceptor_(ioc_), socket_(ioc_), ctx_(ssl::context::sslv23_server) {
  beast::error_code ec;
  endpoint_ = {boost::asio::ip::make_address(address), port};

  // Load the certificate
  ctx_.use_certificate(
      boost::asio::buffer(certificate.c_str(), certificate.size()),
      ssl::context::file_format::pem,
      ec);
  if (ec) {
    Fail("ERROR_USE_CERTIFICATE", ec);
    return;
  }

  // Load the private key
  ctx_.use_rsa_private_key(
      boost::asio::buffer(private_key.c_str(), private_key.size()),
      ssl::context::file_format::pem,
      ec);
  if (ec) {
    Fail("ERROR_USE_RSA_PRIVATE_KEY", ec);
    return;
  }

  // Open the acceptor
  acceptor_.open(endpoint_.protocol(), ec);
  if (ec) {
    Fail("EEROR_ACCEPTOR_OPEN", ec);
    return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    Fail("ERROR_SET_OPTION", ec);
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint_, ec);
  if (ec) {
    Fail("ERROR_BIND", ec);
    return;
  }

  // Start listening for connections
  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    Fail("ERROR_LISTEN", ec);
    return;
  }
}

// Start accepting incoming connections
void WSSSession::Run() {
  do_accept();
}

void WSSSession::Stop() {
  try {
    ioc_.stop();
    acceptor_.close();
  } catch (...) {
    std::cerr << "Failed to close connection" << std::endl;
  }
}

void WSSSession::do_accept() {
  if (acceptor_.is_open()) {
    acceptor_.async_accept(
        socket_,
        std::bind(
            &WSSSession::on_accept, shared_from_this(), std::placeholders::_1));
    ioc_.run();
  }
}

void WSSSession::on_accept(boost::system::error_code ec) {
  if (ec) {
    Fail("ERROR_ON_ACCEPT", ec);
    ioc_.stop();
    return;
  }
  // Create the session and run it
  wss_ = std::make_shared<WSSServer>(std::move(socket_), ctx_);
  wss_->Run();
}

}  // namespace websocket
}  // namespace sample