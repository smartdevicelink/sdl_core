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
           application_manager::commands::Command::CommandOrigin origin));
  MOCK_METHOD2(SendMessageToMobile,
               void(application_manager::commands::MessageSharedPtr, bool));
  MOCK_METHOD1(
      SendMessageToHMI,
      void(const application_manager::commands::MessageSharedPtr message));
#ifdef SDL_REMOTE_CONTROL
  MOCK_METHOD1(SendPostMessageToMobile,
               void(const application_manager::MessagePtr& message));
  MOCK_METHOD1(SendPostMessageToHMI,
               void(const application_manager::MessagePtr& message));
#endif  // SDL_REMOTE_CONTROL
};
}
}
}

#endif  // MOCK_RPC_SERVICE_H
