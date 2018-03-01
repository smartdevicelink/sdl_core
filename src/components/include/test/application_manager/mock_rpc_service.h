#ifndef MOCK_RPC_SERVICE_H
#define MOCK_RPC_SERVICE_H

#include "gmock/gmock.h"
#include "application_manager/rpc_service.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockRPCService : public application_manager::rpc_service::RPCService {
 public:
  MOCK_METHOD1(
      ManageHMICommand,
      bool(const application_manager::commands::MessageSharedPtr message));
  MOCK_METHOD2(
      ManageMobileCommand,
      bool(const application_manager::commands::MessageSharedPtr message,
           application_manager::commands::Command::CommandSource origin));
  MOCK_METHOD2(SendMessageToMobile,
               void(application_manager::commands::MessageSharedPtr, bool));
  MOCK_METHOD1(set_protocol_handler, void(protocol_handler::ProtocolHandler*));
  MOCK_METHOD1(set_hmi_message_handler,
               void(hmi_message_handler::HMIMessageHandler*));
  MOCK_METHOD1(
      SendMessageToHMI,
      void(const application_manager::commands::MessageSharedPtr message));

  MOCK_METHOD1(SendPostMessageToMobile,
               void(const application_manager::MessagePtr& message));
  MOCK_METHOD1(SendPostMessageToHMI,
               void(const application_manager::MessagePtr& message));
};
}
}
}

#endif  // MOCK_RPC_SERVICE_H
