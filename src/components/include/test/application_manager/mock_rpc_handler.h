#ifndef MOCK_RPC_HANDLER_H
#define MOCK_RPC_HANDLER_H

#include "application_manager/rpc_handler.h"
#include "telemetry_monitor/telemetry_observable.h"

#include "gmock/gmock.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockRPCHandler : public application_manager::rpc_handler::RPCHandler {
 public:
  MOCK_METHOD4(ValidateRpcSO,
               bool(smart_objects::SmartObject& message,
                    utils::SemanticVersion& msg_version,
                    rpc::ValidationReport& report_out,
                    bool allow_unknown_parameters));

  MOCK_METHOD1(OnMessageReceived,
               void(protocol_handler::RawMessagePtr message));

  MOCK_METHOD1(OnMobileMessageSent,
               void(protocol_handler::RawMessagePtr message));

  MOCK_METHOD1(OnMessageReceived,
               void(std::shared_ptr<application_manager::Message> message));

  MOCK_METHOD1(OnErrorSending,
               void(std::shared_ptr<application_manager::Message> message));

  MOCK_METHOD1(SetTelemetryObserver,
               void(application_manager::AMTelemetryObserver* observer));
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // MOCK_RPC_HANDLER_H
