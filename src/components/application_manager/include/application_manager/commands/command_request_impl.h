/*
 Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_

#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/smart_object_keys.h"

namespace application_manager {
namespace commands {

/**
 * @brief Class is intended to encapsulate RPC as an object
 */
class CommandRequestImpl : public CommandImpl,
                           public event_engine::EventObserver {
 public:
  enum RequestState {
    kAwaitingHMIResponse = 0,
    kTimedOut,
    kHandlingResponse,
    kCompleted
  };

  /**
   * @brief CommandRequestImpl class constructor
   * @param message Incoming SmartObject message
   */

  CommandRequestImpl(const MessageSharedPtr& message,
                     ApplicationManager& application_manager,
                     rpc_service::RPCService& rpc_service,
                     HMICapabilities& hmi_capabilities,
                     policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief CommandRequestImpl class destructor
   *
   **/
  virtual ~CommandRequestImpl();

  /**
   * @brief Checks command permissions according to policy table
   */
  bool CheckPermissions() OVERRIDE;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() OVERRIDE;

  /**
   * @brief Cleanup all resources used by command
   **/
  bool CleanUp() OVERRIDE;

  /**
   * @brief Execute corresponding command by calling the action on reciever
   **/
  void Run() OVERRIDE;

  /**
   * @brief Retrieves request default timeout.
   * If request has a custom timeout, request_timeout_ should be reassign to it
   * @return Request default timeout
   */

  uint32_t default_timeout() const OVERRIDE;

  virtual void on_event(const event_engine::MobileEvent& event);

  /**
   * @brief Retrieves correlation ID
   */
  uint32_t correlation_id() const OVERRIDE;

  void SendProviderRequest(
      const mobile_apis::FunctionID::eType& mobile_function_id,
      const hmi_apis::FunctionID::eType& hmi_function_id,
      const smart_objects::SmartObject* msg,
      bool use_events = false);

  void SendMobileRequest(const mobile_apis::FunctionID::eType& function_id,
                         smart_objects::SmartObjectSPtr msg,
                         bool use_events = false);

  /*
   * * @brief Retrieves connection key
   */
  uint32_t connection_key() const OVERRIDE;

  /**
   * @brief Retrieves request ID
   */
  int32_t function_id() const OVERRIDE;

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   */
  void HandleTimeOut() FINAL;

  /**
   * @brief Checks Mobile result code for single RPC
   * @param result_code contains result code from response to Mobile
   * @return true if result code complies to successful result codes,
   * false otherwise.
   */
  static bool IsMobileResultSuccess(
      const mobile_apis::Result::eType result_code);

  /**
   * @brief Checks HMI result code for single RPC
   * @param result_code contains result code from HMI response
   * @return true if result code complies to successful result codes,
   * false otherwise.
   */
  static bool IsHMIResultSuccess(
      const hmi_apis::Common_Result::eType result_code);

  /*
   * @brief Default EvenObserver's pure virtual method implementation
   * @param event The received event
   */
  void HandleOnEvent(const event_engine::Event& event) FINAL;

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceeded its limit
   * @note default value is required as a lot of requests don't use it
   * as they use automatically generated reason
   */
  virtual void OnTimeOut();

  /*
   * @brief Algorithm of handling on_event
   * @param Event The received event
   */
  virtual void on_event(const event_engine::Event&);

  void OnUpdateTimeOut() OVERRIDE;

#ifdef __QNX__
  /*
   * @brief Function is called by RequestController when message was sent to HMI
   */
  void OnHMIMessageSent() OVERRIDE;
#endif  // __QNX__

  /*
   * @brief AllowedToTerminate tells request Controller if it can terminate this
   * request by response.
   * By default, RequestCtrl should terminate all requests by their responses.
   *  If request need to terminate itself, it should override this function
   * false
   * @return allowed_to_terminate_ value
   */
  bool AllowedToTerminate() OVERRIDE;

  /*
   * @brief SetAllowedToTerminate set up allowed to terminate flag.
   * If true, request controller will terminate request on response
   */
  void SetAllowedToTerminate(const bool allowed) OVERRIDE;

  /*
   * @brief Check syntax of string from mobile
   * @param str - string that need to be checked
   * @param allow_empty_string if true methods allow empty sting
   * @return true if success otherwise return false
   */
  virtual bool CheckSyntax(const std::string& str,
                           bool allow_empty_line = false);

  /**
   * @brief CheckResult checks whether the overall result
   * of the responses is successful
   * @param first response
   * @param second response
   * @return true if the overall result is successful
   * otherwise - false
   */
  bool CheckResult(const ResponseInfo& first, const ResponseInfo& second) const;

 protected:
  /**
   * @brief Adds interface to be awaited for by sdl request command
     @param interface_id interface which SDL expects to response in given time
  */
  void StartAwaitForInterface(const HmiInterfaces::InterfaceID& interface_id);

  /**
   * @brief Gets interface awaiting state.
   * @param interface_id interface which SDL awaits for response in given time
   * @return true if SDL awaits for response from given interface in
   * interface_id
   */
  bool IsInterfaceAwaited(const HmiInterfaces::InterfaceID& interface_id) const;

  /**
   * @brief Stops SDL awaiting from given HMI interface
   * @param interface_id interface from which SDL no longer awaits
   * for response in given time
   */
  void EndAwaitForInterface(const HmiInterfaces::InterfaceID& interface_id);

  /*
   * @brief Checks if there some not delivered hmi responses exist
   * @return true if pending responses exist, otherwise - false
   */
  bool IsPendingResponseExist() const;

  /*
   * @brief Parses mobile message and replaces mobile app id with HMI app id
   * @param message Message to replace its ids
   * @return True if replacement succeeded, otherwise - false
   */
  bool ReplaceMobileWithHMIAppId(smart_objects::SmartObject& message) OVERRIDE;

  /*
   * @brief Parses message from HMI and replaces HMI app id with mobile app id
   * @param message Message to replace its ids
   * @return True if replacement succeeded, otherwise - false
   */
  bool ReplaceHMIWithMobileAppId(smart_objects::SmartObject& message) OVERRIDE;

  RequestState current_state_;

  /**
   * @brief This set stores all the interfaces which are awaited by SDL to
   * return a response to some request
   */
  std::set<HmiInterfaces::InterfaceID> awaiting_response_interfaces_;
  mutable sync_primitives::Lock awaiting_response_interfaces_lock_;

  /*
   * @brief Returns current state of request
   * @return current request state. E.g. kAwaitingResponse, kTimedOut,
   * kResponded
   */
  virtual RequestState current_state() const;

  /*
   * @brief Sets current state of request
   * @param request state to set. E.g. kAwaitingResponse, kTimedOut,
   * kResponded
   */
  virtual void set_current_state(const RequestState state);

  /*
   * @brief This lock is used to guarantee thread safe access to request state
   */
  std::shared_ptr<sync_primitives::RecursiveLock> state_lock_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandRequestImpl);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
