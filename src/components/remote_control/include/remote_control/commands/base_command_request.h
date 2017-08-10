/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_BASE_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_BASE_COMMAND_REQUEST_H_

#include "remote_control/commands/command.h"
#include "remote_control/event_engine/event_observer.h"
#include "application_manager/message.h"
#include "application_manager/service.h"
#include "utils/logger.h"
#include "interfaces/HMI_API.h"
#include "remote_control/rc_app_extension.h"
#include "json/json.h"
#include "remote_control/remote_plugin_interface.h"

namespace remote_control {

namespace commands {

// Forward declaration to make this struct friend to BaseCommandRequest
struct OnDriverAnswerCallback;

/**
 * @brief Base command class for requests
 */
class BaseCommandRequest
    : public Command,
      public rc_event_engine::EventObserver<application_manager::MessagePtr,
                                            std::string> {
 public:
  /**
   * @brief BaseCommandRequest class constructor
   *
   * @param message Message from mobile
   **/
  BaseCommandRequest(const application_manager::MessagePtr& message,
                     RemotePluginInterface& rc_module);

  /**
   * @brief BaseCommandRequest class destructor
   */
  virtual ~BaseCommandRequest();

  /**
   * @brief BaseCommandRequest on timeout reaction
   */
  virtual void OnTimeout();

  // TODO(KKolodiy): need rename to Execute
  void Run();
  void on_event(const rc_event_engine::Event<application_manager::MessagePtr,
                                             std::string>& event);

  /**
   * @brief Generates correct request to HMI
   * @param function_id request ID
   * @param msg_params json with message params
   * @return generated request shared ptr
   */
  application_manager::MessagePtr CreateHmiRequest(
      const char* function_id, const Json::Value& message_params);

  /**
   * @brief Prepares response for sending to mobile
   * Adds necessary fields to message
   * @param success true if successful; false, if failed
   * @param result_code Mobile result code in string ("SUCCESS", "INVALID_DATA",
   * e.t.c)
   * @param info Provides additional human readable info regarding the
   *result(may be empty)
   */
  void PrepareResponse(const bool success,
                       const char* result_code,
                       const std::string& info);

 protected:
  application_manager::MessagePtr message_;
  Json::Value response_params_;

  /**
   * @brief AcquireResource try to allocate resource for application
   * In case if allocation of resource is not required, return ALLOWED by
   * default.
   * This method should be overrided in RPCs that requires resource allocation
   * @return result of resource allocation, in case if allocation os not
   * required, return ALLOWED
   */
  virtual AcquireResult::eType AcquireResource(const Json::Value&) {
    return AcquireResult::ALLOWED;
  }

  /**
   * @brief IsResourceFree check resource state
   * This is default implementation which has to be redefined for RPCs which
   * need to manage the resources
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  virtual bool IsResourceFree(const std::string& module_type) const {
    UNUSED(module_type);
    return true;
  }

  /**
   * @brief SetResourceState changes state of resource
   * This is default implementation which has to be redefined for RPCs which
   * need to manage the resources
   * @param Message containing type of module to extract
   * @param State to set for resource
   */
  virtual void SetResourceState(const Json::Value&,
                                const ResourceState::eType) {}

  /**
   * @brief Get extension for specified application. If extension doesn't exist,
   * it will be created
   * @param app pointer to application
   * @return pointer to extension
   */
  RCAppExtensionPtr GetAppExtension(
      application_manager::ApplicationSharedPtr app) const;

  /**
   * @brief Converts HMI result code to string with mobile result code
   *
   * @param hmi_code HMI result code
   * @return String with mobile result code
   */
  const char* GetMobileResultCode(
      const hmi_apis::Common_Result::eType& hmi_code) const;

  /**
   * @brief Sends Mobile response
   * @param success true if successful; false, if failed
   * @param result_code Mobile result code in string ("SUCCESS", "INVALID_DATA",
   *e.t.c)
   * @param info Provides additional human readable info regarding the
   *result(may be empty)
   */
  void SendResponse(const bool success,
                    const char* result_code,
                    const std::string& info);

  /**
   * @brief Parse result code from response
   *
   * @param message Response from HMI or Can
   * @param result_code Outgoing param with mobile result code in string
   *("SUCCESS", "INVALID_DATA", e.t.c)
   * @param info Outgoing param with additional human readable info regarding
   *the result(may be empty)
   * @return true if it is success response? otherwise false
   */
  bool ParseResultCode(const Json::Value& value,
                       std::string& result_code,
                       std::string& info);

  /**
     * @brief Sends request to HMI
     * @param message_to_send to send
     */
  void SendMessageToHMI(const application_manager::MessagePtr& message_to_send);

  /**
   * @brief Sends request to CAN or HMI
   * @param function_id request ID
   * @param msg_params json with message params
   */
  void SendRequest(const char* function_id, const Json::Value& message_params);

  application_manager::ApplicationSharedPtr app() {
    DCHECK(app_);
    return app_;
  }

  /**
   * @brief executes specific logic of children classes
   */
  // TODO(KKolodiy): need rename to Run
  void virtual Execute() = 0;

  /**
   * @brief Validates request by xml schema
   */
  bool Validate();

  /*
   * @brief Parses incoming string into Json
   * @param parsed_mgs Resulting json object (must be valid pointer)
   * @returns True if json string was valid false otherwise.
   */
  virtual bool ParseJsonString(Json::Value* parsed_msg);

  /**
   * @brief Interface method that is called whenever new event received
   * @param event The received event
   */
  void virtual OnEvent(
      const rc_event_engine::Event<application_manager::MessagePtr,
                                   std::string>& event) = 0;

  virtual std::string ModuleType(const Json::Value& message);
  virtual std::vector<std::string> ControlData(const Json::Value& message);
  virtual application_manager::TypeAccess CheckAccess(
      const Json::Value& message);

  bool auto_allowed() const {
    return auto_allowed_;
  }

  void set_auto_allowed(bool value) {
    auto_allowed_ = value;
  }

  application_manager::ServicePtr service() {
    return service_;
  }

  void set_disallowed_info(const std::string& info) {
    disallowed_info_ = info;
  }

 private:
  void CheckHMILevel(application_manager::TypeAccess access,
                     bool hmi_consented = false);
  void UpdateHMILevel(
      const rc_event_engine::Event<application_manager::MessagePtr,
                                   std::string>& event);
  bool CheckPolicyPermissions();
  bool CheckDriverConsent();
  inline bool IsAutoAllowed(application_manager::TypeAccess access) const;
  inline bool IsNeededDriverConsent(
      application_manager::TypeAccess access) const;
  void SendDisallowed(application_manager::TypeAccess access);
  void SendGetUserConsent(const Json::Value& value);
  void ProcessAccessResponse(
      const rc_event_engine::Event<application_manager::MessagePtr,
                                   std::string>& event);
  application_manager::ApplicationSharedPtr app_;
  application_manager::ServicePtr service_;
  bool auto_allowed_;
  std::string disallowed_info_;

  friend struct OnDriverAnswerCallback;
};

}  // namespace commands

}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_BASE_COMMAND_REQUEST_H_
