#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "transport_manager/tcp/tcp_adapter.h"
#include "transport_manager/device_adapter/device_adapter_listener.h"
#include "transport_manager/mock_device_adapter_listener.h"

using ::test::components::transport_manager::MockDeviceAdapterListener;

namespace transport_manager {
namespace device_adapter {

TEST(TcpAdapterBasicTest, Basic) {
  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ("sdl-tcp", device_adapter->getDeviceType());
  EXPECT_TRUE(device_adapter->isClientOriginatedConnectSupported());
  EXPECT_TRUE(device_adapter->isServerOriginatedConnectSupported());
  EXPECT_TRUE(device_adapter->isSearchDevicesSupported());
}

TEST(TcpAdapterBasicTest, NotInitialised) {
  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->SearchDevices());
  EXPECT_EQ(DeviceAdapter::OK,
            device_adapter->connect(DeviceUID("xxx"), 2));
  EXPECT_EQ(DeviceAdapter::BAD_STATE,
            device_adapter->disconnect(DeviceUID("xxx"), 2));
  EXPECT_EQ(DeviceAdapter::BAD_STATE,
            device_adapter->disconnectDevice(DeviceUID("xxx")));
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

using ::testing::_;
using ::testing::Invoke;

void disconnect(const DeviceAdapter* device_adapter,
                const DeviceUID device_handle,
                const ApplicationHandle app_handle) {
  EXPECT_EQ(
      DeviceAdapter::OK,
      const_cast<DeviceAdapter*>(device_adapter)->disconnect(device_handle,
                                                             app_handle));
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
    const DeviceAdapter::Error error = device_adapter_->init();
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

class TcpAdapterTestWithListenerAutoStart : public TcpAdapterTest {
  virtual void SetUp() {
    TcpAdapterTest::SetUp();
    device_adapter_->startClientListening();
  }
};

MATCHER_P(ContainsMessage, str, ""){ return strlen(str) == arg->data_size() && 0 == memcmp(str, arg->data(), arg->data_size());}

TEST_F(TcpAdapterTestWithListenerAutoStart, Connect) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, Receive) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _));
    EXPECT_CALL(
        mock_dal_,
        onDataReceiveDone(device_adapter_, _, _, ContainsMessage("abcd"))).
        WillOnce(InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  EXPECT_TRUE(client_.send("abcd"));
}

struct SendHelper {
  explicit SendHelper(DeviceAdapter::Error expected_error)
      : expected_error_(expected_error),
        message_(
            new protocol_handler::RawMessage(
                1,
                1,
                const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>("efgh")),
                4)) {
  }
  void sendMessage(const DeviceAdapter* device_adapter,
                   const DeviceUID device_handle,
                   const ApplicationHandle app_handle) {
    EXPECT_EQ(
        expected_error_,
        const_cast<DeviceAdapter*>(device_adapter)->sendData(device_handle,
                                                             app_handle,
                                                             message_));
  }
  DeviceAdapter::Error expected_error_;
  transport_manager::RawMessageSptr message_;
};

TEST_F(TcpAdapterTestWithListenerAutoStart, Send) {
  SendHelper helper(DeviceAdapter::OK);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _)).WillOnce(
        Invoke(&helper, &SendHelper::sendMessage));
    EXPECT_CALL(mock_dal_,
        onDataSendDone(device_adapter_, _, _, helper.message_)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }

  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  EXPECT_EQ("efgh", client_.receive(4));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, DisconnectFromClient) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _));
    EXPECT_CALL(mock_dal_, onUnexpectedDisconnect(device_adapter_, _, _, _));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  client_.disconnect();
}

TEST_F(TcpAdapterTestWithListenerAutoStart, DisconnectFromServer) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _)).WillOnce(
        Invoke(disconnect));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, SendToDisconnected) {
  SendHelper* helper = new SendHelper(DeviceAdapter::BAD_PARAM);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _)).WillOnce(
        Invoke(disconnect));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, _, _)).WillOnce(
        ::testing::DoAll(Invoke(helper, &SendHelper::sendMessage),
                         InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp)));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, SendFailed) {
  static unsigned char zzz[2000000];  //2000000 is much more than socket buffer
  SendHelper* helper = new SendHelper(DeviceAdapter::OK);
  helper->message_ = new protocol_handler::RawMessage(1, 1, zzz, sizeof(zzz));
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, onConnectDone(device_adapter_, _, _)).WillOnce(
        Invoke(helper, &SendHelper::sendMessage));
    EXPECT_CALL(mock_dal_,
                onDataSendFailed(device_adapter_, _, _, helper->message_, _));
    EXPECT_CALL(mock_dal_, onDisconnectDone(device_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  client_.receive(2);
  client_.disconnect();
}

TEST_F(TcpAdapterTest, StartStop) {
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter_->stopClientListening());
  EXPECT_FALSE(client_.connect(TcpDeviceAdapter::default_port));
  EXPECT_EQ(DeviceAdapter::OK, device_adapter_->startClientListening());
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  client_.disconnect();
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter_->startClientListening());
  EXPECT_TRUE(client_.connect(TcpDeviceAdapter::default_port));
  client_.disconnect();
  EXPECT_EQ(DeviceAdapter::OK, device_adapter_->stopClientListening());
  EXPECT_FALSE(client_.connect(TcpDeviceAdapter::default_port));
  wakeUp();
}

}  // namespace
}  // namespace

