#include "transport_manager/bluetooth_le/ble_server.h"
#include <thread>
#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("Ble_Server")

namespace{
    const int buffer_size = 131072; // Copied from INI file
    std::vector<uint8_t> buffer(buffer_size);
}

namespace transport_manager {
namespace transport_adapter {

constexpr char BleServer::WriterSocketName[];
constexpr char BleServer::ControlSocketName[];

BleServer::BleServer(const std::string& socket_name, MessageDelegate&& callback)
    : socket_name_(socket_name)
    , callback_(callback)
{}

void BleServer::Init()
{
    SDL_LOG_AUTO_TRACE();
    connected_ = false;
    stop_requested_ = false;

    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    socklen_t len;
    server_sockaddr.sun_family = AF_LOCAL;
    server_sockaddr.sun_path[0] = '\0';
    strcpy(&server_sockaddr.sun_path[1], socket_name_.c_str() );
    len = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(&server_sockaddr.sun_path[1]);
    int err;

    server_sock_ = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (server_sock_ < 0) {
        err = errno;
        SDL_LOG_ERROR("Cannot open socket: " << strerror(err) << " " << socket_name_);
        return;
    }

    unlink(socket_name_.c_str());
    int rc = bind(server_sock_, (struct sockaddr *) &server_sockaddr, len);
    if (rc < 0){
        err = errno;
        SDL_LOG_ERROR("BIND ERROR: " << strerror(err) << " " << socket_name_);
        close(server_sock_);
        return;
    }

    int backlog = 1;
    rc = listen(server_sock_, backlog);
    if (rc < 0){
        err = errno;
        SDL_LOG_ERROR("LISTEN ERROR: " << strerror(err) << " " << socket_name_);
        close(server_sock_);
        return;
    }

    SDL_LOG_INFO("Socket listening... " << socket_name_);

    client_sock_ = accept(server_sock_, (struct sockaddr *) &client_sockaddr, &len);
    if (client_sock_ < 0){
        err = errno;
        SDL_LOG_ERROR("ACCEPT ERROR: " << strerror(err) << " " << socket_name_);
        close(server_sock_);
        close(client_sock_);
        return;
    }

    connected_ = true;
}

void BleServer::Run()
{
    SDL_LOG_AUTO_TRACE();

       while (connected_ && !stop_requested_) {
        buffer.resize(buffer_size);
        const int n = read(client_sock_, buffer.data(), buffer_size - 1);
        if(n > 0) {
            buffer.resize(n);
            callback_(buffer);
        }
    }
}

void BleServer::Stop() {
    SDL_LOG_DEBUG("Requesting server to stop");
    stop_requested_ = true;
    close(client_sock_);
    close(server_sock_);
    connected_ = false;
}

BleServer::~BleServer(){
    if(connected_){
        close(client_sock_);
        close(server_sock_);
    }
}

}  // namespace transport_adapter
}  // namespace transport_manager