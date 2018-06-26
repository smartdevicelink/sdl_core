#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_IMPL_H

#include <map>
#include "utils/macro.h"
#include "utils/lock.h"
#include "rc_rpc_plugin/interior_data_cache.h"
namespace rc_rpc_plugin {
class InteriorDataCacheImpl : public InteriorDataCache {

public:
    InteriorDataCacheImpl();
    void Add(const std::string &module_type, const smart_objects::SmartObject &module_data) OVERRIDE;
    smart_objects::SmartObject Retrive(const std::string &module_type) const OVERRIDE;
    bool Check(const std::string &module_type) const OVERRIDE;
    void Clear() OVERRIDE;
private:
    std::map<std::string, smart_objects::SmartObject> subscriptions_;
    mutable sync_primitives::Lock subscriptions_lock_;
};


}  // rc_rpc_plugin


#endif // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_H
