#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "smart_objects/smart_object.h"

namespace resumption {

namespace app_mngr = application_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "ExtensionPenndingResumptionHandler")

ExtensionPendingResumptionHandler::ExtensionPendingResumptionHandler(
    app_mngr::ApplicationManager& application_manager)
    : application_manager::event_engine::EventObserver(
          application_manager.event_dispatcher())
    , application_manager_(application_manager) {}

ResumptionRequest ExtensionPendingResumptionHandler::MakeResumptionRequest(
    const uint32_t corr_id,
    const hmi_apis::FunctionID::eType function_id,
    const smart_objects::SmartObject& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  resumption::ResumptionRequest resumption_request;
  resumption_request.request_ids.correlation_id = corr_id;
  resumption_request.request_ids.function_id = function_id;
  resumption_request.message = message;
  return resumption_request;
}

}  //  namespace resumption