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
#include <set>
#include "application_manager/application_manager.h"
#include "application_manager/message.h"
#include "application_manager/application_impl.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "request_watchdog/watchdog_subscriber.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "interfaces/HMI_API.h"
#include "application_manager/hmi_capabilities.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class CSmartObject;
}
}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace threads {
class Thread;
}

namespace application_manager {

/**
  *@brief Typedef for shared pointer
*/
typedef utils::SharedPtr<MessageChaining> MessageChainPtr;

/**
  *@brief Map of messages between mobile app and hmi
*/
typedef std::map<unsigned int, MessageChainPtr> MessageChains;


class ApplicationManagerImpl : public ApplicationManager
  , public hmi_message_handler::HMIMessageObserver
  , public connection_handler::ConnectionHandlerObserver
  , public request_watchdog::WatchdogSubscriber
    , public HMICapabilities {
  public:
    ~ApplicationManagerImpl();
    static ApplicationManagerImpl* instance();

    /////////////////////////////////////////////////////

    Application* application(int app_id);
    inline const std::set<Application*>& applications() const;
    Application* active_application() const;
    std::vector<Application*> applications_by_button(unsigned int button);
    std::vector<Application*> applications_by_ivi(unsigned int vehicle_info);
    std::vector<Application*> applications_with_navi();

    std::set<connection_handler::Device>& device_list();

    /////////////////////////////////////////////////////

    bool RegisterApplication(Application* application);
    /*
     * @brief Closes application by id
     *
     * @param app_id Application id
     *
     * @return operation result
     */
    bool UnregisterApplication(int app_id);

    /*
     * @brief Closes all registered applications
     *
     * @param hmi_off Describes if the reason for exiting
     *  applications was HU switching off
     */
    void UnregisterAllApplications(bool hmi_off = false);
    bool RemoveAppDataFromHMI(Application* application);
    bool LoadAppDataToHMI(Application* application);
    bool ActivateApplication(Application* application);
    void ConnectToDevice(unsigned int id);
    void OnHMIStartedCooperation();

    /*
     * @brief Add to the chain amount of requests sent to hmi
     * from mobile request, to ensure that response to mobile
     * will be sent only after all hmi response were received.
     *
     * @param chain Pointer to MessageChaining class.
     * If parameter is empty new instance is created,
     * otherwise counter of MessageChaining for
     * corresponding correlation ID is increased
     *
     * @param connection_key of connection for Mobile side
     * @param correlation_id Correlation id for response for Mobile side
     * @param function_id Id of HMI request/response
     * @return pointer to MessageChaining
     */
    MessageChaining* AddMessageChain(
      MessageChaining* chain,
      unsigned int connection_key,
      unsigned int correlation_id,
      unsigned int function_id = 0,  // TODO(VS): delete this param
      const NsSmartDeviceLink::NsSmartObjects::CSmartObject* data = NULL);

    /*
     * @brief Decrease chain for correlation ID
     * after response from hmi was received.
     *
     * @param correlation_id Correlation id for response for Mobile side
     *
     * @return true if there is no other pending responses
     */
    bool DecreaseMessageChain(unsigned int correlation_id);

    /*
     * @brief Retriev MessageChaining object from chain
     *
     * @param correlation_id Correlation id for response for Mobile side
     *
     * @return MessageChaining on success, otherwise NULL
     */
    MessageChaining* GetMessageChain(unsigned int correlation_id) const;

    /*
     * @brief Retrieves flag for audio pass thru request
     *
     * @return Current state of the audio pass thru request flag
     */
    bool audio_pass_thru_flag() const;

    /*
     * @brief Sets flag for audio pass thru request
     *
     * @param flag New state to be set
     */
    void set_audio_pass_thru_flag(bool flag);

    /*
     * @brief Retrieves driver distraction state
     *
     * @return Current state of the distraction state
     */
    inline bool driver_distraction() const;

    /*
     * @brief Sets state for driver distraction
     *
     * @param state New state to be set
     */
    void set_driver_distraction(
      bool is_distracting);

    /*
     * @brief Retrieves if VR session has started
     *
     * @return Current VR session state (started, stopped)
     */
    inline bool vr_session_started() const;

    /*
     * @brief Sets VR session state
     *
     * @param state Current HMI VR session state
     */
    void set_vr_session_started(const bool& state);

    /*
     * @brief Retrieves currently active UI language
     *
     * @return Currently active UI language
     */
    inline const hmi_apis::Common_Language::eType&
    active_ui_language() const;

    /*
     * @brief Sets currently active UI language
     *
     * @param language Currently active UI language
     */
    void set_active_ui_language(
      const hmi_apis::Common_Language::eType& language);

    /*
     * @brief Retrieves currently active VR language
     *
     * @return Currently active VR language
     */
    inline const hmi_apis::Common_Language::eType&
    active_vr_language() const;

    /*
     * @brief Sets currently active VR language
     *
     * @param language Currently active VR language
     */
    void set_active_vr_language(
      const hmi_apis::Common_Language::eType& language);

    /*
     * @brief Retrieves currently active TTS language
     *
     * @return Currently active TTS language
     */
    inline const hmi_apis::Common_Language::eType&
    active_tts_language() const;

    /*
     * @brief Sets currently active TTS language
     *
     * @param language Currently active TTS language
     */
    void set_active_tts_language(
      const hmi_apis::Common_Language::eType& language);

    void set_vehicle_type(
      const smart_objects::CSmartObject& vehicle_type);

    const smart_objects::CSmartObject* vehicle_type() const;

    /*
     * @brief Retrieves SDL access to all mobile apps
     *
     * @return Currently active state of the access
     */
    inline bool all_apps_allowed() const;

    /*
     * @brief Sets SDL access to all mobile apps
     *
     * @param allowed SDL access to all mobile apps
     */
    void set_all_apps_allowed(const bool& allowed);

    /*
     * @brief Starts audio pass thru thread
     *
     * @param session_key     Session key of connection for Mobile side
     * @param correlation_id  Correlation id for response for Mobile side
     * @param max_duration    Max duration of audio recording in milliseconds
     * @param sampling_rate   Value for rate(8, 16, 22, 44 kHz)
     * @param bits_per_sample The quality the audio is recorded.
     * @param audio_type      Type of audio data
     */
    void StartAudioPassThruThread(int session_key, int correlation_id,
                                  int max_duration, int sampling_rate,
                                  int bits_per_sample, int audio_type);

    /*
     * @brief Terminates audio pass thru thread
     */
    void StopAudioPassThruThread();

    /////////////////////////////////////////////////////

    void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler);
    void set_mobile_message_handler(
      mobile_message_handler::MobileMessageHandler* handler);
    void set_connection_handler(
      connection_handler::ConnectionHandler* handler);
    void set_watchdog(request_watchdog::Watchdog* watchdog);

    ///////////////////////////////////////////////////////

    void StartDevicesDiscovery();
    void SendMessageToMobile(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);
    bool ManageMobileCommand(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);
    void SendMessageToHMI(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);
    bool ManageHMICommand(
      const utils::SharedPtr<smart_objects::CSmartObject>& message);

    /////////////////////////////////////////////////////////

    void onMessageReceived(
      utils::SharedPtr<application_manager::Message> message);
    void onErrorSending(
      utils::SharedPtr<application_manager::Message> message);

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

    void CreateHMIMatrix(HMIMatrix* matrix);

    void CreatePoliciesManager(PoliciesManager* managaer);

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

    bool ConvertMessageToSO(const Message& message,
                            smart_objects::CSmartObject& output);
    bool ConvertSOtoMessage(
      const smart_objects::CSmartObject& message, Message& output);

    /**
     * @brief Map of connection keys and associated applications
     */
    std::map<int, Application*>                   applications_;
    /**
     * @brief List of applications
     */
    std::set<Application*>                        application_list_;
    MessageChains                                 message_chaining_;
    bool                                          audio_pass_thru_flag_;
    threads::Thread*                              perform_audio_thread_;
    bool                                          is_distracting_driver_;
    bool                                          is_vr_session_strated_;
    bool                                          hmi_cooperating_;
    bool                                          is_all_apps_allowed_;
    hmi_apis::Common_Language::eType              ui_language_;
    hmi_apis::Common_Language::eType              vr_language_;
    hmi_apis::Common_Language::eType              tts_language_;
    smart_objects::CSmartObject* vehicle_type_;

    hmi_message_handler::HMIMessageHandler*       hmi_handler_;
    mobile_message_handler::MobileMessageHandler* mobile_handler_;
    connection_handler::ConnectionHandler*        connection_handler_;
    request_watchdog::Watchdog*                   watchdog_;

    static log4cxx::LoggerPtr                     logger_;

    DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);
};

const std::set<Application*>& ApplicationManagerImpl::applications() const {
  return application_list_;
}

bool ApplicationManagerImpl::vr_session_started() const {
  return is_vr_session_strated_;
}

bool ApplicationManagerImpl::driver_distraction() const {
  return is_distracting_driver_;
}

inline const hmi_apis::Common_Language::eType&
ApplicationManagerImpl::active_ui_language() const {
  return ui_language_;
}

inline const hmi_apis::Common_Language::eType&
ApplicationManagerImpl::active_vr_language() const {
  return vr_language_;
}

inline const hmi_apis::Common_Language::eType&
ApplicationManagerImpl::active_tts_language() const {
  return tts_language_;
}

inline bool ApplicationManagerImpl::all_apps_allowed() const {
  return is_all_apps_allowed_;
}
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_
