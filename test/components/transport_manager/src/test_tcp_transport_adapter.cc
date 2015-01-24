#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/mock_transport_adapter_listener.h"

namespace transport_manager {
namespace transport_adapter {

using RawMessage = ::protocol_handler::RawMessage;

TEST(TcpAdapterBasicTest, Basic) {
  TransportAdapter* transport_adapter = new TcpTransportAdapter(12345);

  EXPECT_EQ("sdl-tcp", transport_adapter->GetDeviceType());
  EXPECT_TRUE(transport_adapter->IsServerOriginatedConnectSupported());
  EXPECT_TRUE(transport_adapter->IsClientOriginatedConnectSupported());
  EXPECT_TRUE(transport_adapter->IsSearchDevicesSupported());
}

TEST(TcpAdapterBasicTest, NotInitialised) {
  TransportAdapter* transport_adapter = new TcpTransportAdapter(12345);

  EXPECT_EQ(TransportAdapter::BAD_STATE, transport_adapter->SearchDevices());
  EXPECT_EQ(TransportAdapter::OK,
            transport_adapter->Connect(DeviceUID("xxx"), 2));
  EXPECT_EQ(TransportAdapter::BAD_STATE,
            transport_adapter->Disconnect(DeviceUID("xxx"), 2));
  EXPECT_EQ(TransportAdapter::BAD_STATE,
            transport_adapter->DisconnectDevice(DeviceUID("xxx")));
}

class ClientTcpSocket {
 public:
  bool Connect(uint16_t server_port) {
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

  bool Send(const std::string& str) {
    size_t size = str.size();
    ssize_t written = write(socket_, str.c_str(), size);
    if (written != -1) {
      size_t count = static_cast<size_t>(written);
      return count == size;
    }
    else {
      return false;
    }
  }

  std::string receive(size_t size) {
    char* buf = new char[size];
    ssize_t read = recv(socket_, buf, size, MSG_WAITALL);
    if (read != -1) {
      return std::string(buf, buf + read);
    }
    else {
      return std::string();
    }
  }

  void Disconnect() {
    close(socket_);
  }

 private:
  uint16_t port_;
  int socket_;
};

using ::testing::_;
using ::testing::Invoke;

void Disconnect(const TransportAdapter* transport_adapter,
                const DeviceUID device_handle,
                const ApplicationHandle app_handle) {
  EXPECT_EQ(
      TransportAdapter::OK,
      const_cast<TransportAdapter*>(transport_adapter)->Disconnect(device_handle,
                                                             app_handle));
}

class TcpAdapterTest : public ::testing::Test {
 public:
  TcpAdapterTest()
      : port_(ChoosePort()),
        transport_adapter_(new TcpTransportAdapter(port_)),
        suspended_(false),
        finished_(false) {
    pthread_mutex_init(&suspend_mutex_, 0);
    pthread_cond_init(&suspend_cond_, 0);
  }

  uint16_t ChoosePort() {
    return getpid() % 1000 + 3000;
  }

  virtual void SetUp() {
    const TransportAdapter::Error error = transport_adapter_->Init();
    ASSERT_EQ(TransportAdapter::OK, error);
    transport_adapter_->AddListener(&mock_dal_);
    time_t end_time = time(NULL) + 5;
    while (!transport_adapter_->IsInitialised() && time(NULL) < end_time)
      sleep(0);
    ASSERT_TRUE(transport_adapter_->IsInitialised());
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
      if (ETIMEDOUT
          == pthread_cond_timedwait(&suspend_cond_, &suspend_mutex_,
                                    &abs_time)) {
        break;
      }
    }
    pthread_mutex_unlock(&suspend_mutex_);
    delete transport_adapter_;
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

  uint16_t port() const {return port_;}

  const uint16_t port_;
  TransportAdapter* transport_adapter_;
  ::test::components::transport_manager::MockTransportAdapterListener mock_dal_;
  ClientTcpSocket client_;

  pthread_cond_t suspend_cond_;
  pthread_mutex_t suspend_mutex_;
  bool suspended_;
  bool finished_;
};

class TcpAdapterTestWithListenerAutoStart : public TcpAdapterTest {
  virtual void SetUp() {
    TcpAdapterTest::SetUp();
    transport_adapter_->StartClientListening();
  }
};

MATCHER_P(ContainsMessage, str, ""){ return strlen(str) == arg->data_size() && 0 == memcmp(str, arg->data(), arg->data_size());}

TEST_F(TcpAdapterTestWithListenerAutoStart, Connect) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.Connect(port()));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, Receive) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _));
    EXPECT_CALL(
        mock_dal_,
        OnDataReceiveDone(transport_adapter_, _, _, ContainsMessage("abcd"))).
        WillOnce(InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.Connect(port()));
  EXPECT_TRUE(client_.Send("abcd"));
}

struct SendHelper {
  explicit SendHelper(TransportAdapter::Error expected_error)
      : expected_error_(expected_error),
        message_(
            new RawMessage(
                1,
                1,
                const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>("efgh")),
                4)) {
  }
  void sendMessage(const TransportAdapter* transport_adapter,
                   const DeviceUID device_handle,
                   const ApplicationHandle app_handle) {
    EXPECT_EQ(
        expected_error_,
        const_cast<TransportAdapter*>(transport_adapter)->SendData(device_handle,
                                                             app_handle,
                                                             message_));
  }
  TransportAdapter::Error expected_error_;
  RawMessagePtr message_;
};

TEST_F(TcpAdapterTestWithListenerAutoStart, Send) {
  SendHelper helper(TransportAdapter::OK);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _)).WillOnce(
        Invoke(&helper, &SendHelper::sendMessage));
    EXPECT_CALL(mock_dal_,
        OnDataSendDone(transport_adapter_, _, _, helper.message_)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }

  EXPECT_TRUE(client_.Connect(port()));
  EXPECT_EQ("efgh", client_.receive(4));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, DisconnectFromClient) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _));
    EXPECT_CALL(mock_dal_, OnUnexpectedDisconnect(transport_adapter_, _, _, _));
    EXPECT_CALL(mock_dal_, OnDisconnectDone(transport_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.Connect(port()));
  client_.Disconnect();
}

TEST_F(TcpAdapterTestWithListenerAutoStart, DisconnectFromServer) {
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _)).WillOnce(
        Invoke(Disconnect));
    EXPECT_CALL(mock_dal_, OnDisconnectDone(transport_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.Connect(port()));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, SendToDisconnected) {
  SendHelper* helper = new SendHelper(TransportAdapter::BAD_PARAM);
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _)).WillOnce(
        Invoke(Disconnect));
    EXPECT_CALL(mock_dal_, OnDisconnectDone(transport_adapter_, _, _)).WillOnce(
        ::testing::DoAll(Invoke(helper, &SendHelper::sendMessage),
                         InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp)));
  }
  EXPECT_TRUE(client_.Connect(port()));
}

TEST_F(TcpAdapterTestWithListenerAutoStart, SendFailed) {
  static unsigned char zzz[2000000];  //2000000 is much more than socket buffer
  SendHelper* helper = new SendHelper(TransportAdapter::OK);
  helper->message_ = new RawMessage(1, 1, zzz, sizeof(zzz));
  {
    ::testing::InSequence seq;
    EXPECT_CALL(mock_dal_, OnConnectDone(transport_adapter_, _, _)).WillOnce(
        Invoke(helper, &SendHelper::sendMessage));
    EXPECT_CALL(mock_dal_,
                OnDataSendFailed(transport_adapter_, _, _, helper->message_, _));
    EXPECT_CALL(mock_dal_, OnDisconnectDone(transport_adapter_, _, _)).WillOnce(
        InvokeWithoutArgs(this, &TcpAdapterTest::wakeUp));
  }
  EXPECT_TRUE(client_.Connect(port()));
  client_.receive(2);
  client_.Disconnect();
}

TEST_F(TcpAdapterTest, StartStop) {
  EXPECT_EQ(TransportAdapter::BAD_STATE, transport_adapter_->StopClientListening());
  EXPECT_FALSE(client_.Connect(port()));
  EXPECT_EQ(TransportAdapter::OK, transport_adapter_->StartClientListening());
  EXPECT_TRUE(client_.Connect(port()));
  client_.Disconnect();
  EXPECT_EQ(TransportAdapter::BAD_STATE, transport_adapter_->StartClientListening());
  EXPECT_TRUE(client_.Connect(port()));
  client_.Disconnect();
  EXPECT_EQ(TransportAdapter::OK, transport_adapter_->StopClientListening());
  EXPECT_FALSE(client_.Connect(port()));
  wakeUp();
}

}  // namespace
}  // namespace

