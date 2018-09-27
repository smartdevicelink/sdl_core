#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/application_manager.h"
#include "application_manager/app_extension.h"

namespace resumption {

struct ResumptionHandlingCallbacks;

namespace app_mngr = application_manager;

class ExtensionPendingResumptionHandler
    : public application_manager::event_engine::EventObserver {
 public:
  ExtensionPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  virtual ~ExtensionPendingResumptionHandler() {}

  // EventObserver interface
  virtual void on_event(
      const application_manager::event_engine::Event& event) = 0;

  virtual void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      app_mngr::Application& app,
      ResumptionHandlingCallbacks callbacks) = 0;

  virtual void ClearPendingResumptionRequests() = 0;

 protected:
  virtual ResumptionRequest MakeResumptionRequest(
      const uint32_t corr_id,
      const hmi_apis::FunctionID::eType function_id,
      const smart_objects::SmartObject& message);

  app_mngr::ApplicationManager& application_manager_;
};
}  //  namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H
