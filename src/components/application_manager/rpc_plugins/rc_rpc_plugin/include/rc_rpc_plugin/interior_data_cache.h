#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_H
#include <string>
#include "smart_objects/smart_object.h"

namespace rc_rpc_plugin {
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

/**
 * @brief The InteriorDataCache interface for caching data class
 * Provide ability to cache module data by module type name and clear cache
 */
class InteriorDataCache {
public:
    /**
     * @brief Add add module data to cache
     * @param module_type module type name
     * @param module_data data to be cached
     */
    virtual void Add(const std::string& module_type, const smart_objects::SmartObject& module_data ) = 0;

    /**
     * @brief Retrive Get cached data
     * @param module_type data type to get from cache
     * @return smart object with cached data, or nulll smart object
     */
    virtual smart_objects::SmartObject Retrive(const std::string& module_type) const = 0;

    /**
     * @brief Check check if data exists in cache
     * @param module_type module type name to check in cache
     * @return true if cached, false otherwize
     */
    virtual bool Check(const std::string& module_type) const = 0 ;

    /**
     * @brief Clear clear all cached data
     */
    virtual void Clear() = 0;

};
}  // rc_rpc_plugin

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIORDATACACHE_H
