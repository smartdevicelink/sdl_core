#include "rc_rpc_plugin/interior_data_cache_impl.h"


namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

InteriorDataCacheImpl::InteriorDataCacheImpl() {

}

void InteriorDataCacheImpl::Add(const std::string &module_type, const smart_objects::SmartObject &module_data) {
    LOG4CXX_AUTO_TRACE(logger_);
    sync_primitives::AutoLock autolock(subscriptions_lock_);
    subscriptions_[module_type] = module_data;
}

smart_objects::SmartObject InteriorDataCacheImpl::Retrive(const std::string &module_type) const {
    LOG4CXX_AUTO_TRACE(logger_);
    sync_primitives::AutoLock autolock(subscriptions_lock_);
    auto it = subscriptions_.find(module_type);
    if (it == subscriptions_.end()) {
        return smart_objects::SmartObject(smart_objects::SmartType_Null);
    }
    return it->second;
}

bool InteriorDataCacheImpl::Check(const std::string &module_type) const{
    LOG4CXX_AUTO_TRACE(logger_);
    sync_primitives::AutoLock autolock(subscriptions_lock_);
    auto it = subscriptions_.find(module_type);
    return it != subscriptions_.end();
}

void InteriorDataCacheImpl::Clear() {
    LOG4CXX_AUTO_TRACE(logger_);
    sync_primitives::AutoLock autolock(subscriptions_lock_);
    subscriptions_.clear();
}

}
