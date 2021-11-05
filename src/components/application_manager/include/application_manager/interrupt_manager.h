/*
InterruptManager header file (under construction)
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_INTERRUPT_MANAGER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_INTERRUPT_MANAGER_H

#include "application_manager/rpc_handler_impl.h"
#include "config_profile/profile.h"
#include "application_manager/application_manager.h"
#include "utils/logger.h"

namespace application_manager {

namespace interrupt_manager {

enum IMStatus
{
    IM_DEACTIVATE = 0,
    IM_ACTIVATE,
    IM_ACTIVATE_DEFAULT
};

enum InterruptCheckResult
{
    NO_INTERRUPT = 0,
    REJECT_ONS_IN_QUEUE,
    REJECT_RECEIVED_ONS,
    INTERRUPT_ERROR,
};

class InterruptManager
{
public:
    InterruptManager();
    InterruptManager(ApplicationManager& app_manager, const policy::PolicySettings& policy_settings);
    ~InterruptManager();

    void Init();

    InterruptCheckResult CheckRpcInterrupt(const std::shared_ptr<Message> outgoing_message);
    bool DoRpcReject(mobile_apis::FunctionID::eType function_id);	
    IMStatus GetRunningStatus();    
    bool ReadInterruptSetting();
    bool IsSubjectMessageToPrioritized(std::shared_ptr<Message> outgoing_message);
    std::shared_ptr<Message> highest_priority_ons_rpc_;
    std::list<int32_t> reject_rpc_list_;
    std::string appPriority;
    std::string firstAppPriority;
    std::string app_hmi_level;
    std::string first_app_hmi_level;
    ns_smart_device_link::ns_smart_objects::SmartObject message_;
    ns_smart_device_link::ns_smart_objects::SmartObject first_message_;
    int32_t app_id;
    int32_t first_app_id; 
    ApplicationSharedPtr app;
    ApplicationSharedPtr first_app; 
    int32_t function_id;
    int32_t first_function_id;

    std::string function_name;
    std::string first_function_name;

    // Priority Table
    rpc::policy_table_interface_base::rpc_priority_type  rpc_priority_table_;
    rpc::policy_table_interface_base::app_priority_type apptype_priority_table_;
    rpc::policy_table_interface_base::hmi_status_priority_type  hmists_priority_table_;

private:    
    InterruptCheckResult CheckRpcPriority(const std::shared_ptr<Message> outgoing_message);
    InterruptCheckResult CheckAppTypePriority(const std::shared_ptr<Message> outgoing_message);
  	InterruptCheckResult CheckHmiStatusPriority(const std::shared_ptr<Message> outgoing_message);
    bool ReadRpcPriorityTable();
    bool ReadAppTypePriorityTable();
    bool ReadHmiStatusPriorityTable();

private:	
    IMStatus im_status_;
    ApplicationManager& app_manager_;
    const policy::PolicySettings& policy_settings_;
};

}  // namespace interrupt_manager
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_INTERRUPT_MANAGER_H

