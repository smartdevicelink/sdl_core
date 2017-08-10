#include "functional_module/generic_module.h"
namespace functional_modules {

typedef std::deque<ModuleObserver*>::iterator ModuleObserverIterator;

GenericModule::GenericModule(ModuleID module_id)
    : kModuleId_(module_id), state_(ServiceState::IDLE) {}

GenericModule::~GenericModule() {
  observers_.clear();
}

void GenericModule::AddObserver(ModuleObserver* const observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  observers_.push_back(observer);
}

void GenericModule::RemoveObserver(ModuleObserver* const observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  for (ModuleObserverIterator it = observers_.begin(); observers_.end() != it;
       ++it) {
    if (*it == observer) {
      observers_.erase(it);
      return;
    }
  }
}

void GenericModule::NotifyObservers(ModuleObserver::Errors error) {
  for (ModuleObserverIterator it = observers_.begin(); observers_.end() != it;
       ++it) {
    (*it)->OnError(error, kModuleId_);
  }
}

void GenericModule::set_service(application_manager::ServicePtr service) {
  service_ = service;
}

void GenericModule::OnServiceStateChanged(ServiceState state) {
  state_ = state;

  if (HMI_ADAPTER_INITIALIZED == state_) {
    // We must subscribe to necessary HMI notifications
    service_->SubscribeToHMINotification(hmi_api::on_interior_vehicle_data);
    service_->SubscribeToHMINotification(hmi_api::on_remote_control_settings);
    // Disabled
    //    service_->SubscribeToHMINotification(hmi_api::on_reverse_apps_allowing);
    //    service_->SubscribeToHMINotification(hmi_api::on_device_rank_changed);
  }
}

application_manager::ServicePtr GenericModule::service() {
  return service_;
}

}  //  namespace functional_modules
