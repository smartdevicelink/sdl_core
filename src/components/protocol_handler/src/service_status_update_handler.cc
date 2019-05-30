#include "protocol_handler/service_status_update_handler.h"
#include "interfaces/HMI_API.h"

namespace protocol_handler {

hmi_apis::Common_ServiceType::eType GetHMIServiceType(
    protocol_handler::ServiceType service_type) {
  using namespace hmi_apis;
  using namespace protocol_handler;
  switch (service_type) {
    case SERVICE_TYPE_RPC: {
      return Common_ServiceType::RPC;
    }
    case SERVICE_TYPE_AUDIO: {
      return Common_ServiceType::AUDIO;
    }
    case SERVICE_TYPE_NAVI: {
      return Common_ServiceType::VIDEO;
    }
    default: { return Common_ServiceType::INVALID_ENUM; }
  }
}

void ServiceStatusUpdateHandler::OnServiceUpdate(
    const uint32_t connection_key,
    const protocol_handler::ServiceType service_type,
    ServiceStatus service_status) {
  using namespace hmi_apis;
  typedef utils::Optional<Common_ServiceUpdateReason::eType>
      UpdateReasonOptional;
  auto hmi_service_type = GetHMIServiceType(service_type);

  switch (service_status) {
    case ServiceStatus::SERVICE_RECEIVED: {
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_RECEIVED,
          UpdateReasonOptional(UpdateReasonOptional::EMPTY));
    }
    case ServiceStatus::SERVICE_ACCEPTED: {
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_ACCEPTED,
          UpdateReasonOptional(UpdateReasonOptional::EMPTY));
    }
    case ServiceStatus::SERVICE_START_FAILED: {
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_REJECTED,
          UpdateReasonOptional(UpdateReasonOptional::EMPTY));
    }
    case ServiceStatus::PTU_FAILED: {
      auto update_reason = Common_ServiceUpdateReason::PTU_FAILED;
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_REJECTED,
          update_reason);
    }
    case ServiceStatus::CERT_INVALID: {
      auto update_reason = Common_ServiceUpdateReason::INVALID_CERT;
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_REJECTED,
          update_reason);
    }
    case ServiceStatus::INVALID_TIME: {
      auto update_reason = Common_ServiceUpdateReason::INVALID_TIME;
      return listener_->ProcessServiceStatusUpdate(
          connection_key,
          hmi_service_type,
          Common_ServiceEvent::REQUEST_REJECTED,
          update_reason);
    }
    default: { return; }
  }
}
}  // namespace protocol_handler
