#include "functional_module/generic_module.h"

namespace functional_modules {

GenericModule::GenericModule(ModuleID module_id)
  : kModuleId_(module_id) {}

GenericModule::~GenericModule() {
  observers_.clear();
}

void GenericModule::AddObserver(utils::SharedPtr<ModuleObserver> observer) {
  observers_.push_back(observer);
}

void GenericModule::RemoveObserver(utils::SharedPtr<ModuleObserver> observer) {
  for (std::deque<utils::SharedPtr<ModuleObserver>>::iterator it = observers_.begin();
       observers_.end() != it;
       ++it) {
    if (*it == observer) {
      observers_.erase(it);
      return;
    }
  }
}

}  //  namespace functional_modules