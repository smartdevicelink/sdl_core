#pragma once

#include "gmock/gmock.h"
#include "application_manager/rpc_protection_mediator.h"

namespace application_manager {
class MockRPCProtectionMediator : public RPCProtectionMediator {
 public:
  MOCK_METHOD4(DoesRPCNeedEncryption,
               bool(const uint32_t function_id,
                    std::shared_ptr<Application> app,
                    const uint32_t conrrelation_id,
                    const bool is_rpc_service_secure));
  MOCK_METHOD1(DoesRPCNeedEncryption, bool(const uint32_t conrrelation_id));
  MOCK_METHOD1(EncryptResponseByForce, void(const uint32_t conrrelation_id));
  MOCK_METHOD3(CreateNegativeResponse,
               std::shared_ptr<smart_objects::SmartObject>(
                   const uint32_t connection_key,
                   const uint32_t function_id,
                   const uint32_t conrrelation_id));
};
}
