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
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "interfaces/HMI_API.h"

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
    , public request_watchdog::WatchdogSubscriber {
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
    bool UnregisterApplication(Application* application);
    void UnregisterAllApplications();
    bool RemoveAppDataFromHMI(Application* application);
    bool LoadAppDataToHMI(Application* application);
    bool ActivateApplication(Application* application);
    void ConnectToDevice(unsigned int id);

    /*
     * @brief Closes all registered applications
     *
     * @param reason Describes reason for exiting application
     */
    void ExitAllApplications(
        const hmi_apis::Common_ApplicationsCloseReason::eType& reason);

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
        unsigned int function_id,
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
     * @brief Retrieves if mixing audio is supported by HMI
     * (ie recording TTS command and playing audio)
     *
     * @return Current state of the mixing audio flag
     */
    bool attenuated_supported() const;

    /*
     * @brief Sets state for mixing audio
     *
     * @param state New state to be set
     */
    void set_attenuated_supported(bool state);

    /*
     * @brief Retrieves driver distraction state
     *
     * @return Current state of the distraction state
     */
    inline const hmi_apis::Common_DriverDistractionState::eType&
        driver_distraction() const;

    /*
     * @brief Sets state for driver distraction
     *
     * @param state New state to be set
     */
    void set_driver_distraction(
        const hmi_apis::Common_DriverDistractionState::eType& state);

    /*
     * @brief Retrieves UI supported languages
     *
     * @return Currently supported UI languages
     */
    inline const smart_objects::CSmartObject*
      ui_supported_languages() const;

    /*
     * @brief Sets supported UI languages
     *
     * @param supported_languages Supported UI languages
     */
    void set_ui_supported_languages(
        const smart_objects::CSmartObject& supported_languages);

    /*
     * @brief Retrieves TTS  supported languages
     *
     * @return Currently supported TTS languages
     */
    inline const smart_objects::CSmartObject*
      tts_supported_languages() const;

    /*
     * @brief Sets supported TTS languages
     *
     * @param supported_languages Supported TTS languages
     */
    void set_tts_supported_languages(
        const smart_objects::CSmartObject& supported_languages);

    /*
     * @brief Retrieves VR supported languages
     *
     * @return Currently supported VR languages
     */
    inline const smart_objects::CSmartObject*
      vr_supported_languages() const;

    /*
     * @brief Sets supported VR languages
     *
     * @param supported_languages Supported VR languages
     */
    void set_vr_supported_languages(
        const smart_objects::CSmartObject& supported_languages);

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

    /**
     * @brief Map of connection keys and associated applications
     */
    std::map<int, Application*>                     applications_;
    /**
     * @brief List of applications
     */
    std::set<Application*>                          application_list_;
    MessageChains                                   message_chaining_;
    bool                                            hmi_deletes_commands_;
    bool                                            audio_pass_thru_flag_;
    threads::Thread*                                perform_audio_thread_;
    bool                                            attenuated_supported_;
    hmi_apis::Common_DriverDistractionState::eType  driver_distraction_;
    smart_objects::CSmartObject*                    ui_supported_languages_;
    smart_objects::CSmartObject*                    tts_supported_languages_;
    smart_objects::CSmartObject*                    vr_supported_languages_;
    bool                                            is_vr_session_strated;

    DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);
};

const std::set<Application*>& ApplicationManagerImpl::applications() const {
    return application_list_;
}

const hmi_apis::Common_DriverDistractionState::eType&
    ApplicationManagerImpl::driver_distraction() const {
  return driver_distraction_;
}

const smart_objects::CSmartObject*
    ApplicationManagerImpl::ui_supported_languages() const {
  return ui_supported_languages_;
}

const smart_objects::CSmartObject*
    ApplicationManagerImpl::tts_supported_languages() const {
  return tts_supported_languages_;
}

const smart_objects::CSmartObject*
    ApplicationManagerImpl::vr_supported_languages() const {
  return vr_supported_languages_;
}

inline bool ApplicationManagerImpl::vr_session_started() const {
  return is_vr_session_strated;
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_
