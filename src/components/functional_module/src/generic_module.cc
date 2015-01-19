#include "functional_module/generic_module.h"

namespace functional_modules {

typedef std::deque<utils::SharedPtr<ModuleObserver>>::iterator ModuleObserverIterator;

GenericModule::GenericModule(ModuleID module_id)
  : kModuleId_(module_id)
  , state_(ModuleState::NORMAL) {}

GenericModule::~GenericModule() {
  observers_.clear();
}

void GenericModule::AddObserver(utils::SharedPtr<ModuleObserver> observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  observers_.push_back(observer);
}

void GenericModule::RemoveObserver(utils::SharedPtr<ModuleObserver> observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  for (ModuleObserverIterator it = observers_.begin();
       observers_.end() != it;
       ++it) {
    if (*it == observer) {
      observers_.erase(it);
      return;
    }
  }
}

void GenericModule::NotifyObservers(ModuleObserver::Errors error) {
  for (ModuleObserverIterator it = observers_.begin();
       observers_.end() != it; ++it) {
    (*it)->OnError(error, kModuleId_);
  }
}

void GenericModule::SetServiceHandler(application_manager::ServicePtr service) {
  service_ = service;

  // Whe module obtain pointer to service, we can subscribe to necessary HMI notifications
  service_->SubscribeToHMINotification(hmi_api::on_control_changed);
}

}  //  namespace functional_modules
