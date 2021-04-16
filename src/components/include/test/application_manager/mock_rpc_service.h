#ifndef MOCK_RPC_SERVICE_H
#define MOCK_RPC_SERVICE_H

#include "application_manager/rpc_service.h"
#include "gmock/gmock.h"
#include "smart_objects/object_schema_item.h"

namespace test {
namespace components {
namespace application_manager_test {

using ns_smart_device_link::ns_smart_objects::SMember;

class MockRPCService : public application_manager::rpc_service::RPCService {
 public:
  MOCK_METHOD2(
      ManageHMICommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandSource source));
  MOCK_METHOD3(
      ManageHMICommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandSource source,
           const std::string warning_info));
  MOCK_METHOD2(
      ManageMobileCommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandSource origin));
  MOCK_METHOD3(
      ManageMobileCommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandSource origin,
           const std::string warning_info));
  MOCK_METHOD2(SendMessageToMobile,
               void(application_manager::commands::MessageSharedPtr, bool));
  MOCK_METHOD1(
      SendMessageToHMI,
      void(const application_manager::commands::MessageSharedPtr message));
  MOCK_METHOD1(set_protocol_handler,
               void(protocol_handler::ProtocolHandler* handler));
  MOCK_METHOD1(set_hmi_message_handler,
               void(hmi_message_handler::HMIMessageHandler* handler));
  MOCK_METHOD2(
      IsAppServiceRPC,
      bool(int32_t function_id,
           application_manager::commands::Command::CommandSource source));
  MOCK_METHOD3(UpdateMobileRPCParams,
               void(const mobile_apis::FunctionID::eType& function_id,
                    const mobile_apis::messageType::eType& message_type,
                    const std::map<std::string, SMember>& members));

  MOCK_METHOD3(UpdateHMIRPCParams,
               void(const hmi_apis::FunctionID::eType& function_id,
                    const hmi_apis::messageType::eType& message_type,
                    const std::map<std::string, SMember>& members));

  MOCK_METHOD0(Stop, void());
};
}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // MOCK_RPC_SERVICE_H
