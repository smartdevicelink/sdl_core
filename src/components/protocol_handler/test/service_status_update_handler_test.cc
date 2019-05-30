/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "protocol_handler/service_status_update_handler.h"
#include "gtest/gtest.h"
#include "protocol_handler/mock_service_status_update_handler_listener.h"

namespace test {
namespace components {
namespace protocol_handler_test {

namespace {
const uint32_t kConnectioKey = 123u;
}

using namespace protocol_handler;
using ::testing::_;
using ::testing::Return;
using namespace hmi_apis;
typedef utils::Optional<Common_ServiceUpdateReason::eType> UpdateReasonOptional;
typedef std::shared_ptr<ServiceStatusUpdateHandler>
    ServiceStatusUpdateHandlerPtr;
typedef std::shared_ptr<MockServiceStatusUpdateHandlerListener>
    MockServiceStatusUpdateHandlerListenerPtr;

struct ServiceUpdate {
  ServiceType service_type_;
  ServiceStatus service_status_;
  ServiceUpdate(ServiceType type, ServiceStatus status)
      : service_type_(type), service_status_(status) {}
};

class ServiceStatusUpdateHandlerTest
    : public ::testing::TestWithParam<ServiceUpdate> {
 public:
  ServiceStatusUpdateHandlerTest() {
    mock_service_status_update_handler_listener_.reset(
        new MockServiceStatusUpdateHandlerListener);
    service_status_update_handler_ =
        std::make_shared<ServiceStatusUpdateHandler>(
            &(*mock_service_status_update_handler_listener_));
  }

  Common_ServiceEvent::eType GetServiceEvent(ServiceStatus status) {
    switch (status) {
      case ServiceStatus::SERVICE_ACCEPTED: {
        return Common_ServiceEvent::REQUEST_ACCEPTED;
      }
      case ServiceStatus::SERVICE_RECEIVED: {
        return Common_ServiceEvent::REQUEST_RECEIVED;
      }
      case ServiceStatus::SERVICE_START_FAILED:
      case ServiceStatus::PTU_FAILED:
      case ServiceStatus::CERT_INVALID:
      case ServiceStatus::INVALID_TIME: {
        return Common_ServiceEvent::REQUEST_REJECTED;
      }
      default: { return Common_ServiceEvent::INVALID_ENUM; }
    }
  }

  UpdateReasonOptional GetUpdateReason(ServiceStatus status) {
    switch (status) {
      case ServiceStatus::SERVICE_START_FAILED:
      case ServiceStatus::SERVICE_ACCEPTED:
      case ServiceStatus::SERVICE_RECEIVED: {
        return UpdateReasonOptional::EMPTY;
      }
      case ServiceStatus::PTU_FAILED: {
        auto reason = Common_ServiceUpdateReason::PTU_FAILED;
        return reason;
      }
      case ServiceStatus::CERT_INVALID: {
        auto reason = Common_ServiceUpdateReason::INVALID_CERT;
        return reason;
      }
      case ServiceStatus::INVALID_TIME: {
        auto reason = Common_ServiceUpdateReason::INVALID_TIME;
        return reason;
      }
      default: {
        auto reason = Common_ServiceUpdateReason::INVALID_ENUM;
        return reason;
      }
    }
  }

 public:
  MockServiceStatusUpdateHandlerListenerPtr
      mock_service_status_update_handler_listener_;
  ServiceStatusUpdateHandlerPtr service_status_update_handler_;
};

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_SERVICE_ACCEPTED,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::SERVICE_ACCEPTED),
        ServiceUpdate(ServiceType::kMobileNav, ServiceStatus::SERVICE_ACCEPTED),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::SERVICE_ACCEPTED)));

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_SERVICE_RECEIVED,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::SERVICE_RECEIVED),
        ServiceUpdate(ServiceType::kMobileNav, ServiceStatus::SERVICE_RECEIVED),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::SERVICE_RECEIVED)));

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_SERVICE_START_FAILED,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::SERVICE_START_FAILED),
        ServiceUpdate(ServiceType::kMobileNav,
                      ServiceStatus::SERVICE_START_FAILED),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::SERVICE_START_FAILED)));

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_CERT_INVALID,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::CERT_INVALID),
        ServiceUpdate(ServiceType::kMobileNav, ServiceStatus::CERT_INVALID),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::CERT_INVALID)));

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_INVALID_TIME,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::INVALID_TIME),
        ServiceUpdate(ServiceType::kMobileNav, ServiceStatus::INVALID_TIME),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::INVALID_TIME)));

INSTANTIATE_TEST_CASE_P(
    OnServiceUpdate_PTU_FAILED,
    ServiceStatusUpdateHandlerTest,
    ::testing::Values(
        ServiceUpdate(ServiceType::kAudio, ServiceStatus::PTU_FAILED),
        ServiceUpdate(ServiceType::kMobileNav, ServiceStatus::PTU_FAILED),
        ServiceUpdate(ServiceType::kRpc, ServiceStatus::PTU_FAILED)));

TEST_P(ServiceStatusUpdateHandlerTest, OnServiceUpdate) {
  auto service_event_ = GetServiceEvent(GetParam().service_status_);
  auto reason_ = GetUpdateReason(GetParam().service_status_);

  EXPECT_CALL(
      *mock_service_status_update_handler_listener_,
      ProcessServiceStatusUpdate(kConnectioKey, _, service_event_, reason_))
      .Times(1);

  service_status_update_handler_->OnServiceUpdate(
      kConnectioKey, GetParam().service_type_, GetParam().service_status_);
}

TEST_F(ServiceStatusUpdateHandlerTest, GetHMIServiceType) {
  EXPECT_EQ(Common_ServiceType::RPC, GetHMIServiceType(ServiceType::kRpc));
  EXPECT_EQ(Common_ServiceType::AUDIO, GetHMIServiceType(ServiceType::kAudio));
  EXPECT_EQ(Common_ServiceType::VIDEO,
            GetHMIServiceType(ServiceType::kMobileNav));
  EXPECT_EQ(Common_ServiceType::INVALID_ENUM,
            GetHMIServiceType(ServiceType::kInvalidServiceType));
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
