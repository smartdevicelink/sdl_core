#ifndef TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_
#define TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_

#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_manager_listener.h"

#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_packet.h"
#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#include "security_manager/security_manager_mock.h"
#endif  // ENABLE_SECURITY

namespace test  {
namespace components  {
namespace protocol_handler_test {

using namespace protocol_handler;
using namespace transport_manager;

/*
 * MOCK implementation of transport_manager::TransportManager interface
 */
class TransportManagerMock: public TransportManager{
 public:
  MOCK_METHOD0(Init,
               int());
  MOCK_METHOD0(SearchDevices,
               int());
  MOCK_METHOD1(ConnectDevice,
               int(const DeviceHandle&));
  MOCK_METHOD1(DisconnectDevice,
               int(const DeviceHandle&));
  MOCK_METHOD1(Disconnect,
               int(const ConnectionUID &));
  MOCK_METHOD1(DisconnectForce,
               int(const ConnectionUID &));
  MOCK_METHOD1(SendMessageToDevice,
               int(const RawMessageSptr));
  MOCK_METHOD1(ReceiveEventFromDevice,
               int(const TransportAdapterEvent&));
  MOCK_METHOD1(AddTransportAdapter,
               int(transport_adapter::TransportAdapter *));
  MOCK_METHOD1(AddEventListener,
               int(TransportManagerListener *));
  MOCK_METHOD0(Stop,
               int());
  MOCK_METHOD1(RemoveDevice,
               int(const DeviceHandle& ));
  MOCK_CONST_METHOD1(Visibility,
                     int(const bool &));
};

/*
 * MOCK implementation of protocol_handler::SessionObserver interface
 */
class SessionObserverMock: public protocol_handler::SessionObserver {
 public:
  MOCK_METHOD2(SetSSLContext,
               int (const uint32_t& key,
                    security_manager::SSLContext* context));
  MOCK_METHOD2(GetSSLContext,
               security_manager::SSLContext* (
                 const uint32_t& key,
                 const protocol_handler::ServiceType& service_type));
  MOCK_METHOD2(SetProtectionFlag,
               void(
                 const uint32_t& key,
                 const protocol_handler::ServiceType& service_type));
  MOCK_METHOD4(OnSessionStartedCallback,
               uint32_t(
                 const transport_manager::ConnectionUID& connection_handle,
                 const uint8_t session_id,
                 const protocol_handler::ServiceType& service_type,
                 const bool is_protected));
  MOCK_METHOD4(OnSessionEndedCallback,
               uint32_t(
                 const transport_manager::ConnectionUID& connection_handle,
                 const uint8_t sessionId,
                 const uint32_t& hashCode,
                 const protocol_handler::ServiceType& service_type));
  MOCK_METHOD2(KeyFromPair,
               uint32_t(
                 transport_manager::ConnectionUID connection_handle,
                 uint8_t sessionId));
  MOCK_METHOD3(PairFromKey,
               void(
                 uint32_t key,
                 transport_manager::ConnectionUID* connection_handle,
                 uint8_t* sessionId));
  MOCK_METHOD4(GetDataOnSessionKey,
               int32_t(uint32_t key,
                       uint32_t* app_id,
                       std::list<int32_t>* sessions_list,
                       uint32_t* device_id));
  MOCK_METHOD4(GetDataOnDeviceID,
               int32_t(
                 uint32_t device_handle,
                 std::string* device_name,
                 std::list<uint32_t>* applications_list,
                 std::string* mac_address));
};
#ifdef ENABLE_SECURITY
using test::components::security_manager_test::SSLContextMock;

/*
 * MOCK implementation of security_manager::SecurityManager
 */
class SecurityManagerMock: public security_manager::SecurityManager {
 public:
  MOCK_METHOD1(AddListener,
               void(security_manager::SecurityManagerListener *));
  MOCK_METHOD1(CreateSSLContext,
               security_manager::SSLContext*(const uint32_t &));
  MOCK_METHOD1(StartHandshake,
               void(uint32_t));
  MOCK_METHOD4(SendInternalError,
               void(const uint32_t ,
                    const uint8_t&,
                    const std::string&,
                    const uint32_t ));
};
#endif  // ENABLE_SECURITY

/*
 * Matcher for checking RawMessage with InternalError Query
 * Check error id
 */
MATCHER_P2(ControlMessage, ExpectedFrameData, ExpectedEncryption,
           std::string(ExpectedEncryption ? "Protected" : "Unprotected") + " control message ") {
  // Nack shall be always with flag protected off
  DCHECK(ExpectedFrameData  != 0x03 /*FRAME_DATA_START_SERVICE_NACK*/ ||
         ExpectedEncryption == false);
  const RawMessagePtr message = arg;
  const ProtocolPacket packet(message->connection_key(), message->data(), message->data_size());
  if (FRAME_TYPE_CONTROL != packet.frame_type()) {
    *result_listener << "Is not control message";
    return false;
  }
  if (ExpectedFrameData != packet.frame_data()) {
    *result_listener << "Control message is not with data 0x"
                     << std::hex << (int)ExpectedFrameData;
    return false;
  }
  if (ExpectedEncryption != packet.protection_flag()) {
    *result_listener << "Control message is " <<
                        (ExpectedEncryption ? "" : "not ") << "protected";
    return false;
  }
  return true;
}
}  // namespace test
}  // namespace components
}  // namespace protocol_handler_test
#endif  // TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_
