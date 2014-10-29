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
  observers_.push_back(observer);
}

void GenericModule::RemoveObserver(utils::SharedPtr<ModuleObserver> observer) {
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
    (*it)->OnError(error);
  }
}

}  //  namespace functional_modules