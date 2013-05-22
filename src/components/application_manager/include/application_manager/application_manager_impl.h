/**
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_

#include <vector>
#include <map>
#include "application_manager/application_manager.h"
#include "application_manager/message.h"
#include "application_manager/application.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "connection_handler/connection_handler_observer.h"
#include "request_watchdog/watchdog_subscriber.h"
#include "utils/macro.h"
#include "utils/shared_ptr.h"

namespace application_manager {

class ApplicationManagerImpl : public ApplicationManager
  , public hmi_message_handler::HMIMessageObserver
  , public connection_handler::ConnectionHandlerObserver
    , public request_watchdog::WatchdogSubscriber {
  public:
    ~ApplicationManagerImpl();
    static ApplicationManagerImpl* instance();

    /////////////////////////////////////////////////////

    Application* application(int app_id);
    std::vector<Application*> applications() const;
    Application* active_application() const;

    /////////////////////////////////////////////////////

    bool RegisterApplication(Application* application);
    bool UnregisterApplication(Application* application);
    bool UnregisterAllApplications();
    bool RemoveAppDataFromHMI(Application* application);
    bool LoadAppDataToHMI(Application* application);
    bool ActivateApplication(Application* application);
    MessageChaining* AddMessageChain(MessageChaining* chain,
                                     unsigned int connection_key,
                                     unsigned int correlation_id);
    bool DecreaseMessageChain(const MessageChaining* chain);

    /////////////////////////////////////////////////////

    void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler);
    void set_mobile_message_handler(
      mobile_message_handler::MobileMessageHandler* handler);
    void set_connection_handler(
      connection_handler::ConnectionHandler* handler);
    void set_watchdog(request_watchdog::Watchdog* watchdog);

    /*TODO(PV): set or create?*/
    void set_hmi_matrix(HMIMatrix* matrix);
    /*TODO(PV): set or create?*/
    void set_policies_manager(PoliciesManager* managaer);

    ///////////////////////////////////////////////////////

    void SendMessageToMobile(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);
    void SendMessageToHMI(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);

    /////////////////////////////////////////////////////////

    void onMessageReceived(application_manager::Message* message);
    void onErrorSending(application_manager::Message* message);

    void OnDeviceListUpdated(
      const connection_handler::DeviceList& device_list);
    void RemoveDevice(
      const connection_handler::DeviceHandle device_handle);
    void OnSessionStartedCallback(
      connection_handler::DeviceHandle device_handle, int session_key,
      int first_session_key);
    void OnSessionEndedCallback(int session_key,
                                int first_session_key);

    void onTimeoutExpired(request_watchdog::RequestInfo);

  private:
    ApplicationManagerImpl();
    /**
       * \brief Performs check using PoliciesManager of availability
       * of the message for the application. If error occured it is sent
       * as response to initiator of request.
       * \param message Message received for application
       * \param application Application that recieved message to be checked by policies
       * \return bool Indicates whether message is allowed for application
       */
    bool CheckPolicies(smart_objects::CSmartObject* message,
                       Application* application);

    /**
       * \brief Using HMIMatrix checks which messages sent to HMI are of higher priority
       * and acts accordingly (closes message with lower priority,
       * rejects message in case message with higher priority is operating on HMI).
       * If error occured it is sent as response to initiator of request.
       * \param message Message received for application
       * \return bool Indicates whether message is allowed for application
       */
    bool CheckHMIMatrix(smart_objects::CSmartObject* message);

    std::map<int, Application*> applications_;
    bool hmi_deletes_commands_;

    DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_
