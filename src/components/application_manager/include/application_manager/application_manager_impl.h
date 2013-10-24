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

#include <cstdint>
#include <vector>
#include <map>
#include <set>
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message_chaining.h"
#include "application_manager/message.h"
#include "application_manager/application_impl.h"
#include "application_manager/policies_manager/policies_manager.h"
#include "application_manager/request_controller.h"
#include "media_manager/media_manager_impl.h"

#include "hmi_message_handler/hmi_message_observer.h"
#include "mobile_message_handler/mobile_message_observer.h"

#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"


#include "formatters/CSmartFactory.hpp"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
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
 *@brief Map representing hmi request
 *
 *@param int hmi correlation ID
 *@param MessageChainPtr Mobile request temporary data
 */
typedef std::map<unsigned int, MessageChainPtr> HMIRequest;

/**
 *@brief Map representing mobile request and pending HMI requests
 *
 *@param int mobile correlation ID
 *@param HMIRequest Sent HMI request
 */
typedef std::map<unsigned int, HMIRequest> MobileRequest;

/**
 *@brief Map of application ID and incoming mobile requests
 */
typedef std::map<unsigned int, MobileRequest> MessageChain;

class ApplicationManagerImpl : public ApplicationManager,
  public hmi_message_handler::HMIMessageObserver,
  public mobile_message_handler::MobileMessageObserver,
  public connection_handler::ConnectionHandlerObserver,
  public HMICapabilities {
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

    const std::set<connection_handler::Device>& device_list();

    /////////////////////////////////////////////////////

    /**
     * @brief Checks if all HMI capabilities received
     *
     * @return TRUE if all information received, otherwise FALSE
     */
    bool IsHMICapabilitiesInitialized();

    Application* RegisterApplication(
      const utils::SharedPtr<smart_objects::SmartObject>& request_for_registration);
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
    void DeactivateApplication(Application* application);
    void ConnectToDevice(unsigned int id);
    void OnHMIStartedCooperation();

    /*
     * @brief Returns unique correlation ID for HMI request
     *
     * @return Unique correlation ID
     */
    unsigned int GetNextHMICorrelationID();

    /*
     * @brief Add to the chain amount of requests sent to hmi
     * from mobile request, to ensure that all response were received
     * before sending response to mobile.
     *
     * @param connection_key Connection key of application stored in
     * MessageChaining object
     *
     * @param correlation_id Correlation of Mobile request stored in
     * MessageChaining object
     *
     *
     * @param hmi_correlation_id Param is map index and it is equeal to hmi
     * correlation id. Returned from GetNextHMICorrelationID.
     *
     * If param is grate then 0, stored  MessageChaining counter incremented,
     * otherwise new Message chain created.
     *
     * @param data Temporary SmartObject from mobile request.
     * Sometimes request data is needed in mobile response.
     *
     * @return TRUE on success, otherwise FALSE
     */
    MessageChaining* AddMessageChain(
      const unsigned int& connection_key, const unsigned int& correlation_id,
      const unsigned int& hmi_correlation_id, MessageChaining* msg_chaining,
      const smart_objects::SmartObject* data = NULL);

    /*
     * @brief Decrease chain after response from hmi was received
     *
     * @param hmi_correlation_id Unique HMI correlation id from response
     * @param mobile_correlation_id Unique correlation id for Mobile response
     * returned in this parameter
     *
     * @return true if there is no other pending responses
     */
    bool DecreaseMessageChain(const unsigned int& hmi_correlation_id,
                              unsigned int& mobile_correlation_id);

    /*
     * @brief Retrieve MessageChaining object from chain for corresponding
     * HMI correlation ID
     *
     * @param hmi_correlation_id HMI correlation id from HMI response
     *
     * @return MessageChaining on success, otherwise NULL
     */
    MessageChaining* GetMessageChain(
      const unsigned int& hmi_correlation_id) const;

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
    void set_driver_distraction(bool is_distracting);

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
    void set_active_ui_language(const hmi_apis::Common_Language::eType& language);

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
    void set_active_vr_language(const hmi_apis::Common_Language::eType& language);

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

    void set_vehicle_type(const smart_objects::SmartObject& vehicle_type);

    const smart_objects::SmartObject* vehicle_type() const;

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
    void StopAudioPassThru();

    void SendAudioPassThroughNotification(unsigned int session_key,
                                          unsigned int correlation_id,
                                          std::vector<unsigned char> binaryData);

    std::string GetDeviceName(connection_handler::DeviceHandle handle);

    virtual void set_is_vr_cooperating(bool value);
    virtual void set_is_tts_cooperating(bool value);
    virtual void set_is_ui_cooperating(bool value);
    virtual void set_is_navi_cooperating(bool value);
    virtual void set_is_ivi_cooperating(bool value);
    /////////////////////////////////////////////////////

    void set_hmi_message_handler(hmi_message_handler::HMIMessageHandler* handler);
    void set_mobile_message_handler(
      mobile_message_handler::MobileMessageHandler* handler);
    void set_connection_handler(connection_handler::ConnectionHandler* handler);

    ///////////////////////////////////////////////////////

    void StartDevicesDiscovery();
    void SendMessageToMobile(
      const utils::SharedPtr<smart_objects::SmartObject>& message);
    bool ManageMobileCommand(
      const utils::SharedPtr<smart_objects::SmartObject>& message);
    void SendMessageToHMI(
      const utils::SharedPtr<smart_objects::SmartObject>& message);
    bool ManageHMICommand(
      const utils::SharedPtr<smart_objects::SmartObject>& message);

    /////////////////////////////////////////////////////////
    /*
     * @brief Overridden mobile message handler method
     * for incoming mobile messages
     *
     * @param message Incoming mobile message
     *
     */
    virtual void OnMobileMessageReceived(const MobileMessage& message);

    void OnMessageReceived(
      utils::SharedPtr<application_manager::Message> message);
    void OnErrorSending(utils::SharedPtr<application_manager::Message> message);

    void OnDeviceListUpdated(const connection_handler::DeviceList& device_list);
    void RemoveDevice(const connection_handler::DeviceHandle device_handle);
    bool OnSessionStartedCallback(connection_handler::DeviceHandle device_handle,
                                  int session_key,
                                  int first_session_key,
                                  connection_handler::ServiceType type);

    void OnSessionEndedCallback(int session_key,
                                int first_session_key,
                                connection_handler::ServiceType type);

  private:
    ApplicationManagerImpl();
    bool InitThread(threads::Thread* thread);
    hmi_apis::HMI_API& hmi_so_factory();
    mobile_apis::MOBILE_API& mobile_so_factory();

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
    bool CheckPolicies(smart_objects::SmartObject* message,
                       Application* application);

    /**
     * \brief Using HMIMatrix checks which messages sent to HMI are of higher priority
     * and acts accordingly (closes message with lower priority,
     * rejects message in case message with higher priority is operating on HMI).
     * If error occured it is sent as response to initiator of request.
     * \param message Message received for application
     * \return bool Indicates whether message is allowed for application
     */
    bool CheckHMIMatrix(smart_objects::SmartObject* message);

    bool ConvertMessageToSO(const Message& message,
                            smart_objects::SmartObject& output);
    bool ConvertSOtoMessage(const smart_objects::SmartObject& message,
                            Message& output);

    void ProcessMessageFromMobile(const utils::SharedPtr<Message>& message);
    void ProcessMessageFromHMI(const utils::SharedPtr<Message>& message);

    bool RemoveMobileRequestFromMessageChain(unsigned int mobile_correlation_id,
        unsigned int connection_key);

    /**
     * @brief Map of connection keys and associated applications
     */
    std::map<int, Application*> applications_;
    /**
     * @brief List of applications
     */
    std::set<Application*> application_list_;
    MessageChain message_chaining_;
    bool audio_pass_thru_flag_;
    bool is_distracting_driver_;
    bool is_vr_session_strated_;
    bool hmi_cooperating_;
    bool is_all_apps_allowed_;
    hmi_apis::Common_Language::eType ui_language_;
    hmi_apis::Common_Language::eType vr_language_;
    hmi_apis::Common_Language::eType tts_language_;
    smart_objects::SmartObject* vehicle_type_;
    media_manager::MediaManager* media_manager_;

    hmi_message_handler::HMIMessageHandler* hmi_handler_;
    mobile_message_handler::MobileMessageHandler* mobile_handler_;
    connection_handler::ConnectionHandler* connection_handler_;


    policies_manager::PoliciesManager policies_manager_;

    MessageQueue<utils::SharedPtr<Message>> messages_from_mobile_;
    MessageQueue<utils::SharedPtr<Message>> messages_to_mobile_;
    MessageQueue<utils::SharedPtr<Message>> messages_from_hmh_;
    MessageQueue<utils::SharedPtr<Message>> messages_to_hmh_;

    threads::Thread* from_mobile_thread_;
    friend class FromMobileThreadImpl;
    threads::Thread* to_mobile_thread_;
    friend class ToMobileThreadImpl;
    threads::Thread* from_hmh_thread_;
    friend class FromHMHThreadImpl;
    threads::Thread* to_hmh_thread_;
    friend class ToHMHThreadImpl;

    hmi_apis::HMI_API* hmi_so_factory_;
    mobile_apis::MOBILE_API* mobile_so_factory_;

    static log4cxx::LoggerPtr logger_;
    static unsigned int message_chain_current_id_;
    static const unsigned int message_chain_max_id_;
    request_controller::RequestController         request_ctrl;

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
