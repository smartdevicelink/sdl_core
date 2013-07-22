#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "transport_manager/tcp_adapter.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/device_handle_generator_impl.h"

TEST(TcpAdapterBasicTest, Basic) {
  using transport_manager::device_adapter::TcpDeviceAdapter;
  using transport_manager::device_adapter::DeviceAdapter;

  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ("sdl-tcp", device_adapter->getDeviceType());
  EXPECT_TRUE(device_adapter->isClientOriginatedConnectSupported());
  EXPECT_FALSE(device_adapter->isServerOriginatedConnectSupported());
  EXPECT_FALSE(device_adapter->isSearchDevicesSupported());
}

TEST(TcpAdapterBasicTest, NotInitialised) {
  using transport_manager::device_adapter::TcpDeviceAdapter;
  using transport_manager::device_adapter::DeviceAdapter;
  using transport_manager::DeviceHandle;

  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ(DeviceAdapter::NOT_SUPPORTED, device_adapter->searchDevices());
  EXPECT_EQ(DeviceAdapter::NOT_SUPPORTED, device_adapter->connect(DeviceHandle("xxx"), 2, 3));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->acceptConnect(DeviceHandle("xxx"), 2, 3));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->declineConnect(DeviceHandle("xxx"), 2));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->disconnect(1));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->disconnectDevice(DeviceHandle("xxx")));
}

class ClientTcpSocket {
 public:
  bool connect(uint16_t server_port) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
      return false;

    struct sockaddr_in addr;
    memset((char*) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK );
    addr.sin_port = htons(server_port);

    if (::connect(socket_, (struct sockaddr*) &addr, sizeof(addr)) < 0)
      return false;
    else
      return true;
  }

  bool send(const std::string& str) {
    ssize_t written = write(socket_, str.c_str(), str.size());
    return written == str.size();
  }

  std::string receive(std::size_t size) {
    char* buf = new char[size];
    ssize_t read = recv(socket_, buf, size, MSG_WAITALL);
    return std::string(buf, buf + size);
  }

  void disconnect() {
    close(socket_);
  }

 private:
  uint16_t port_;
  int socket_;
};

namespace transport_manager {
namespace device_adapter {
class MockDeviceAdapterListener : public DeviceAdapterListener {
 public:
  MOCK_METHOD1(onSearchDeviceDone,
      void(const DeviceAdapter* device_adapter));
  MOCK_METHOD2(onSearchDeviceFailed,
      void(const DeviceAdapter* device_adapter, const SearchDeviceError& error));
  MOCK_METHOD2(onConnectDone,
      void(const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id));
  MOCK_METHOD3(onConnectFailed,
      void(const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id, const ConnectError& error));
  MOCK_METHOD3(onConnectRequested,
      void(const DeviceAdapter* device_adapter, const DeviceHandle device_handle, const ApplicationHandle app_handle));
  MOCK_METHOD3(onUnexpectedDisconnect,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const CommunicationError& error));
  MOCK_METHOD2(onDisconnectDone,
      void(const DeviceAdapter* device_adapter, const SessionID session_id));
  MOCK_METHOD3(onDisconnectFailed,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const DisconnectError& error));
  MOCK_METHOD2(onDisconnectDeviceDone,
      void(const DeviceAdapter* device_adapter, const SessionID session_id));
  MOCK_METHOD3(onDisconnectDeviceFailed,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const DisconnectDeviceError& error));
  MOCK_METHOD3(onDataSendDone,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const RawMessageSptr data_container));
  MOCK_METHOD4(onDataSendFailed,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const RawMessageSptr data_container, const DataSendError& error));
  MOCK_METHOD3(onDataReceiveDone,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const RawMessageSptr data_container));
  MOCK_METHOD3(onDataReceiveFailed,
      void(const DeviceAdapter* device_adapter, const SessionID session_id, const DataReceiveError& error));
  MOCK_METHOD2(onCommunicationError,
      void(const DeviceAdapter* device_adapter, const SessionID session_id));
};}
}

using transport_manager::device_adapter::TcpDeviceAdapter;
using transport_manager::device_adapter::DeviceAdapter;
using transport_manager::device_adapter::MockDeviceAdapterListener;
using transport_manager::DeviceHandleGeneratorImpl;
using ::testing::_;
using ::testing::Invoke;

static const transport_manager::SessionID session_id = 999;
void acceptConnection(const DeviceAdapter* device_adapter,
                      const transport_manager::DeviceHandle device_handle,
                      const transport_manager::ApplicationHandle app_handle) {
  EXPECT_EQ(
      DeviceAdapter::OK,
      const_cast<DeviceAdapter*>(device_adapter)->acceptConnect(device_handle,
                                                                app_handle,
                                                                session_id));
}

void disconnect(const DeviceAdapter* device_adapter,
                const transport_manager::SessionID session_id) {
  EXPECT_EQ(DeviceAdapter::OK,
            const_cast<DeviceAdapter*>(device_adapter)->disconnect(session_id));
}

class TcpAdapterTest : public ::testing::Test {
 public:
  TcpAdapterTest()
      : device_adapter_(static_cast<DeviceAdapter*>(new TcpDeviceAdapter)),
        suspended_(false),
        finished_(false) {
    pthread_mutex_init(&suspend_mutex_, 0);
    pthread_cond_init(&suspend_cond_, 0);
  }

  virtual void SetUp() {
    const DeviceAdapter::Error error = device_adapter_->init(
        new DeviceHandleGeneratorImpl, 0);
    ASSERT_EQ(DeviceAdapter::OK, error);
    device_adapter_->addListener(&mock_dal_);
    while (!device_adapter_->isInitialised())
      sleep(0);
  }

  virtual ~TcpAdapterTest() {
    pthread_mutex_lock(&suspend_mutex_);
    if (!finished_)
      suspended_ = true;
    struct timeval now;
    gettimeofday(&now, NULL);
    timespec abs_time;
    abs_time.tv_sec = now.tv_sec + 1;
    abs_time.tv_nsec = now.tv_usec * 1000;
    while (suspended_) {
      int i;
      if (ETIMEDOUT
          == pthread_cond_timedwait(&suspend_cond_, &suspend_mutex_,
                                    &abs_time)) {
        break;
      }
    }
    pthread_mutex_unlock(&suspend_mutex_);
    delete device_adapter_;
    pthread_mutex_destroy(&suspend_mutex_);
    pthread_cond_destroy(&suspend_cond_);
  }

  void wakeUp() {
    pthread_mutex_lock(&suspend_mutex_);
    finished_ = true;
    suspended_ = false;
    pthread_cond_signal(&suspend_cond_);
    pthread_mutex_unlock(&suspend_mutex_);
  }

  DeviceAdapter* device_adapter_;
  MockDeviceAdapterListener mock_dal_;
  ClientTcpSocket client_;

  pthread_cond_t suspend_cond_;
  pthread_mutex_t suspend_mutex_;
  bool suspended_;
  bool finished_;
};

MATCHER_P(ContainsMessage, str, ""){ return strlen(str) == arg->data_size() && 0 == memcmp(str, arg->data(), arg->data_size());}

TEST_F(TcpAdapterTest, Connect) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectRequested(device_adapter_,_,_)).WillOnce(
        Invoke(acceptConnection));
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, session_id)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
}

TEST_F(TcpAdapterTest, Receive) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectRequested(device_adapter_,_,_)).WillOnce(
        Invoke(acceptConnection));
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, session_id));
    EXPECT_CALL(
        mock_dal_,
        onDataReceiveDone(device_adapter_, session_id, ContainsMessage("abcd")))
        .WillOnce(InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  EXPECT_TRUE(client_.send("abcd"));
}

TEST_F(TcpAdapterTest, Send) {
  struct Helper {
    void sendMessage(const DeviceAdapter* device_adapter,
                     const transport_manager::SessionID session_id) {
      EXPECT_EQ(
          DeviceAdapter::OK,
          const_cast<DeviceAdapter*>(device_adapter)->sendData(session_id,
                                                               message));
    }
    transport_manager::RawMessageSptr message;
  };
  Helper helper;
  helper.message = new protocol_handler::RawMessage(
      1, 1, 1, reinterpret_cast<const unsigned char*>("efgh"), 4);

  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectRequested(device_adapter_,_,_)).WillOnce(
        Invoke(acceptConnection));
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, session_id)).WillOnce(
        Invoke(&helper, &Helper::sendMessage));
    EXPECT_CALL(mock_dal_,
        onDataSendDone(device_adapter_, session_id, helper.message)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }

  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  EXPECT_EQ("efgh", client_.receive(4));
}

TEST_F(TcpAdapterTest, DisconnectFromClient) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectRequested(device_adapter_,_,_)).WillOnce(
        Invoke(acceptConnection));
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, session_id));
    EXPECT_CALL(mock_dal_,
                onUnexpectedDisconnect(device_adapter_, session_id, _));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, session_id)).
        WillOnce(InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  client_.disconnect();
}

TEST_F(TcpAdapterTest, DisconnectFromServer) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectRequested(device_adapter_,_,_)).WillOnce(
        Invoke(acceptConnection));
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, session_id)).WillOnce(
        Invoke(disconnect));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, session_id)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
}

