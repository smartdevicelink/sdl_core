#include "transport_manager/bluetooth_le/ble_client.h"

#include <thread>
#include <utility>
#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("Ble_Client")

namespace{
    const char* socket_name =  "./localBleReader";
}

namespace transport_manager {
namespace transport_adapter {

BleClient::BleClient(OnDataSentCallback&& sent_callback, OnConnectedCallback&& connected_callback)
    : socket_id_(-1)
    , connected_(false)
    , message_queue_()
    , connected_callback_(connected_callback)
    , sent_callback_(sent_callback)
{}

void BleClient::Init()
{
    SDL_LOG_AUTO_TRACE();

    struct sockaddr_un addr{};
    addr.sun_family = AF_LOCAL;
    addr.sun_path[0] = '\0';
    strcpy(&addr.sun_path[1], socket_name );
    socklen_t len = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(&addr.sun_path[1]);
    int err;

    SDL_LOG_DEBUG("Creating local socket...");
    socket_id_ = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (socket_id_ < 0) {
        err = errno;
        SDL_LOG_ERROR("Cannot open socket: " << strerror(err));
        return;
    }

    if (!TryToConnect(addr, len)) {
        SDL_LOG_ERROR("Failed to connect");
        close(socket_id_);
    } else {
        connected_ = true;
    }

    connected_callback_(connected_);
}

void BleClient::Run()
{
    SDL_LOG_AUTO_TRACE();
    while(connected_ && !message_queue_.IsShuttingDown()) {
        if (message_queue_.empty()) {
            message_queue_.wait();
        }

        protocol_handler::RawMessagePtr message;
        if (message_queue_.pop(message)) {
            int n = write(socket_id_, message->data(), message->data_size());
            if (n > 0) {
                sent_callback_(message);
            } else {
                sent_callback_(nullptr);
            }
        }
    }
}

BleClient::~BleClient()
{
    if(connected_){
        close(socket_id_);
    }
}

bool BleClient::TryToConnect(sockaddr_un& addr, socklen_t len) {
    const int connect_attempts = 10;
    const int attempt_interval_ms = 500;

    for (int i = 0; i < connect_attempts; ++i) {
        SDL_LOG_DEBUG("Attempt #" << i + 1 << " to connect to socket...");

        if (connect(socket_id_, (struct sockaddr *) &addr, len) < 0) {
            SDL_LOG_ERROR("Connect() failed: " << strerror(errno)
              << ". Retry in " << attempt_interval_ms << "ms");
            usleep(attempt_interval_ms);
            continue;
        }

        SDL_LOG_DEBUG("Successfully connected to socket");
        return true;
    }

    SDL_LOG_ERROR("Connection attempts exceeded. Can't connect to socket");
    return false;
}

void BleClient::Stop() {
    SDL_LOG_DEBUG("Requesting client to stop");
    message_queue_.Shutdown();
}

void BleClient::Send(::protocol_handler::RawMessagePtr message) {
    message_queue_.push(message);
}

}  // namespace transport_adapter
}  // namespace transport_manager