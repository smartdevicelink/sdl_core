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
#include "application_manager/hmi_command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message.h"
#include "application_manager/policies_manager/policies_manager.h"
#include "application_manager/request_controller.h"
#include "application_manager/resume_ctrl.h"
#include "protocol_handler/protocol_observer.h"
#include "hmi_message_handler/hmi_message_observer.h"

#include "media_manager/media_manager_impl.h"

#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"

#include "formatters/CSmartFactory.hpp"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"

#include "protocol_handler/service_type.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/prioritized_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/lock.h"
#include "utils/singleton.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace threads {
class Thread;
}
class CommandNotificationImpl;

namespace application_manager {
namespace mobile_api = mobile_apis;

class ApplicationManagerImpl;

namespace impl {
using namespace threads;

/*
 * These dummy classes are here to locally impose strong typing on different
 * kinds of messages
 * Currently there is no type difference between incoming and outgoing messages
 * And due to ApplicationManagerImpl works as message router it has to distinguish
 * messages passed from it's different connection points
 * TODO(ik): replace these with globally defined message types
 * when we have them.
 */
struct MessageFromMobile: public utils::SharedPtr<Message> {
  explicit MessageFromMobile(const utils::SharedPtr<Message>& message):
    utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
};

struct MessageToMobile: public utils::SharedPtr<Message> {
  explicit MessageToMobile(const utils::SharedPtr<Message>& message,
                           bool final_message):
    utils::SharedPtr<Message>(message), is_final(final_message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
  // Signals if connection to mobile must be closed after sending this message
  bool is_final;
 };

struct MessageFromHmi: public utils::SharedPtr<Message> {
  explicit MessageFromHmi(const utils::SharedPtr<Message>& message):
    utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
};

struct MessageToHmi: public utils::SharedPtr<Message> {
  explicit MessageToHmi(const utils::SharedPtr<Message>& message):
    utils::SharedPtr<Message>(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
};

// Short type names for proiritized message queues
typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageFromMobile> > FromMobileQueue;
typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageToMobile> > ToMobileQueue;
typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageFromHmi> > FromHmiQueue;
typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageToHmi> > ToHmiQueue;
}

class ApplicationManagerImpl : public ApplicationManager,
  public hmi_message_handler::HMIMessageObserver,
  public protocol_handler::ProtocolObserver,
  public connection_handler::ConnectionHandlerObserver,
  public impl::FromMobileQueue::Handler,
  public impl::ToMobileQueue::Handler,
  public impl::FromHmiQueue::Handler,
  public impl::ToHmiQueue::Handler,
  public utils::Singleton<ApplicationManagerImpl> {
  friend class ResumeCtrl;
  public:
    ~ApplicationManagerImpl();

    /**
     * @brief Stop work.
     *
     * @return TRUE on success otherwise FALSE.
     **/
    virtual bool Stop();

    /////////////////////////////////////////////////////

    ApplicationSharedPtr application(int32_t app_id) const;
    inline const std::set<ApplicationSharedPtr>& applications() const;
    ApplicationSharedPtr active_application() const;
    std::vector<ApplicationSharedPtr> applications_by_button(uint32_t button);
    std::vector<ApplicationSharedPtr> applications_by_ivi(uint32_t vehicle_info);
    std::vector<ApplicationSharedPtr> applications_with_navi();

    /////////////////////////////////////////////////////

    HMICapabilities& hmi_capabilities();

    ApplicationSharedPtr RegisterApplication(
      const utils::SharedPtr<smart_objects::SmartObject>& request_for_registration);
    /*
     * @brief Closes application by id
     *
     * @param app_id Application id
     * @param is_resuming describes - is this unregister
     *        is normal or need to be resumed
     */
    void UnregisterApplication(const uint32_t& app_id, bool is_resuming = false);

    /*
     * @brief Sets unregister reason for closing all registered applications
     * duringHU switching off
     *
     * @param reason Describes the reason for HU switching off
     */
    void SetUnregisterAllApplicationsReason(
        mobile_api::AppInterfaceUnregisteredReason::eType reason);

    /*
     * @brief Closes all registered applications
     */
    void UnregisterAllApplications();

    bool RemoveAppDataFromHMI(ApplicationSharedPtr app);
    bool LoadAppDataToHMI(ApplicationSharedPtr app);
    bool ActivateApplication(ApplicationSharedPtr app);
    void DeactivateApplication(ApplicationSharedPtr app);
    void ConnectToDevice(uint32_t id);
    void OnHMIStartedCooperation();

    /*
     * @brief Returns unique correlation ID for HMI request
     *
     * @return Unique correlation ID
     */
    uint32_t GetNextHMICorrelationID();

    /* @brief Starts audio passthru process
     *
     * @return true on success, false if passthru is already in process
     */
    bool begin_audio_pass_thru();

    /*
     * @brief Finishes already started audio passthru process
     *
     * @return true on success, false if passthru is not active
     */
    bool end_audio_pass_thru();

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
    void StartAudioPassThruThread(int32_t session_key, int32_t correlation_id,
                                  int32_t max_duration, int32_t sampling_rate,
                                  int32_t bits_per_sample, int32_t audio_type);

    /*
     * @brief Terminates audio pass thru thread
     * @param application_key Id of application for which
     * audio pass thru should be stopped
     */
    void StopAudioPassThru(int32_t application_key);

    void SendAudioPassThroughNotification(uint32_t session_key,
                                          std::vector<uint8_t> binaryData);

    std::string GetDeviceName(connection_handler::DeviceHandle handle);

    /////////////////////////////////////////////////////

    void set_hmi_message_handler(hmi_message_handler::HMIMessageHandler* handler);
    void set_connection_handler(connection_handler::ConnectionHandler* handler);
    virtual void set_policy_manager(policies::PolicyManager* policy_manager);
    void set_protocol_handler(protocol_handler::ProtocolHandler* handler);

    ///////////////////////////////////////////////////////

    void StartDevicesDiscovery();

    // Put message to the queue to be sent to mobile.
    // if |final_message| parameter is set connection to mobile will be closed
    // after processing this message
    void SendMessageToMobile(
      const utils::SharedPtr<smart_objects::SmartObject>& message, bool final_message = false);
    bool ManageMobileCommand(
      const utils::SharedPtr<smart_objects::SmartObject>& message);
    void SendMessageToHMI(
      const utils::SharedPtr<smart_objects::SmartObject>& message);
    bool ManageHMICommand(
      const utils::SharedPtr<smart_objects::SmartObject>& message);

    /////////////////////////////////////////////////////////
    /*
     * @brief Overriden ProtocolObserver method
     */
    virtual void OnMessageReceived(const protocol_handler::
                                   RawMessagePtr& message);

    /*
     * @brief Overriden ProtocolObserver method
     */
    virtual void OnMobileMessageSent(const protocol_handler::
                                     RawMessagePtr& message);

    void OnMessageReceived(
      hmi_message_handler::MessageSharedPointer message);
    void OnErrorSending(hmi_message_handler::MessageSharedPointer message);

    void OnDeviceListUpdated(const connection_handler::DeviceList& device_list);
    void RemoveDevice(const connection_handler::DeviceHandle& device_handle);
    bool OnServiceStartedCallback(const connection_handler::DeviceHandle& device_handle,
                                  const int32_t& session_key,
                                  const protocol_handler::ServiceType& type);
    bool OnServiceResumedCallback(
                const connection_handler::DeviceHandle& device_handle,
                const int32_t& old_session_key, const int32_t& new_session_key,
                const protocol_handler::ServiceType& type);
    void OnServiceEndedCallback(const int32_t& session_key,
                                const protocol_handler::ServiceType& type);

    /**
     * @ Add notification to collection
     *
     * @param ptr Reference to shared pointer that point on hmi notification
     */
    void addNotification(const CommandSharedPtr& ptr);

    /**
     * @ Add notification to collection
     *
     * @param ptr Reference to shared pointer that point on hmi notification
     */
    void removeNotification(const CommandSharedPtr& ptr);

    /**
     * @ Updates request timeout
     *
     * @param connection_key Connection key of application
     * @param mobile_correlation_id Correlation ID of the mobile request
     * @param new_timeout_value New timeout to be set
     */
    void updateRequestTimeout(uint32_t connection_key,
                              uint32_t mobile_correlation_id,
                              uint32_t new_timeout_value);

    /*
     * @brief Retrieves application id associated whith correlation id
     *
     * @param correlation_id Correlation ID of the HMI request
     *
     * @return application id associated whith correlation id
     */
    const uint32_t application_id(const int32_t correlation_id);

    /*
     * @brief Sets application id correlation id
     *
     * @param correlation_id Correlation ID of the HMI request
     * @param app_id Application ID
     */
    void set_application_id(const int32_t correlation_id,
                            const uint32_t app_id);

    /*
     * @brief Change AudioStreamingState for all application according to
     * system audio-mixing capabilities (NOT_AUDIBLE/ATTENUATED) and
     * send notification for this changes
     */
    void Mute();

    /*
     * @brief Change AudioStreamingState for all application to AUDIBLE and
     * send notification for this changes
     */
    void Unmute();

    /*
     * @brief Checks HMI level and returns true if audio streaming is allowed
     */
    bool IsAudioStreamingAllowed(uint32_t connection_key) const;

    /*
     * @brief Checks HMI level and returns true if video streaming is allowed
     */
    bool IsVideoStreamingAllowed(uint32_t connection_key) const;

    /*
     * @brief Save binary data to specified directory
     *
     * @param application name
     * @param binary file name
     * @param binary data
     * @param path for saving data
     * @param offset for saving data to existing file with offset. If offset is 0 - create new file ( overrite existing )
     *
     * @return SUCCESS if file was saved, other code otherwise
     */
    mobile_apis::Result::eType SaveBinary(
                                const std::string& app_name,
                                const std::vector<uint8_t>& binary_data,
                                const std::string& save_path,
                                const uint32_t offset = 0);

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
                       ApplicationSharedPtr app);

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
    utils::SharedPtr<Message> ConvertRawMsgToMessage(
      const protocol_handler::RawMessagePtr& message);

    void ProcessMessageFromMobile(const utils::SharedPtr<Message>& message);
    void ProcessMessageFromHMI(const utils::SharedPtr<Message>& message);




    // threads::MessageLoopThread<*>::Handler implementations
    /*
     * @brief Handles for threads pumping different types
     * of messages. Beware, each is called on different thread!
     */
    // CALLED ON messages_from_mobile_ thread!
    virtual void Handle(const impl::MessageFromMobile& message) OVERRIDE;

    // CALLED ON messages_to_mobile_ thread!
    virtual void Handle(const impl::MessageToMobile& message) OVERRIDE;

    // CALLED ON messages_from_hmi_ thread!
    virtual void Handle(const impl::MessageFromHmi& message) OVERRIDE;

    // CALLED ON messages_to_hmi_ thread!
    virtual void Handle(const impl::MessageToHmi& message) OVERRIDE;

  private:


    // members

    /**
     * @brief Resume controler is responcible for save and load information
     * about persistent application data on disk, and save session ID for resuming
     * application in case INGITION_OFF or MASTER_RESSET
     */

    /**
     * @brief Map of connection keys and associated applications
     */
    std::map<int32_t, ApplicationSharedPtr> applications_;

    /**
     * @brief List of applications
     */
    std::set<ApplicationSharedPtr> application_list_;

    /**
     * @brief Set of HMI notifications with timeout.
     */
    std::list<CommandSharedPtr> notification_list_;

    /**
     * @brief Map of correlation id  and associated application id.
     */
    std::map<const int32_t, const uint32_t> appID_list_;

    bool audio_pass_thru_active_;
    sync_primitives::Lock audio_pass_thru_lock_;
    bool is_distracting_driver_;
    bool is_vr_session_strated_;
    bool hmi_cooperating_;
    bool is_all_apps_allowed_;
    media_manager::MediaManager* media_manager_;

    hmi_message_handler::HMIMessageHandler* hmi_handler_;
    connection_handler::ConnectionHandler*  connection_handler_;
    protocol_handler::ProtocolHandler*      protocol_handler_;
    policies::PolicyManager*                policy_manager_;
    request_controller::RequestController   request_ctrl_;
    HMICapabilities                         hmi_capabilities_;

    // TODO(YS): Remove old implementation
    policies_manager::PoliciesManager policies_manager_;

    hmi_apis::HMI_API*                      hmi_so_factory_;
    mobile_apis::MOBILE_API*                mobile_so_factory_;

    static log4cxx::LoggerPtr logger_;

    static uint32_t corelation_id_;
    static const uint32_t max_corelation_id_;

    // The reason of HU shutdown
    mobile_api::AppInterfaceUnregisteredReason::eType unregister_reason_;

    // Construct message threads when everything is already created

    // Thread that pumps messages coming from mobile side.
    impl::FromMobileQueue messages_from_mobile_;
    // Thread that pumps messages being passed to mobile side.
    impl::ToMobileQueue messages_to_mobile_;
    // Thread that pumps messages coming from HMI.
    impl::FromHmiQueue messages_from_hmi_;
    // Thread that pumps messages being passed to HMI.
    impl::ToHmiQueue messages_to_hmi_;

    // Lock for applications list
    sync_primitives::Lock applications_list_lock_;

    DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);

    FRIEND_BASE_SINGLETON_CLASS(ApplicationManagerImpl);
};

const std::set<ApplicationSharedPtr>& ApplicationManagerImpl::applications() const {
  return application_list_;
}

bool ApplicationManagerImpl::vr_session_started() const {
  return is_vr_session_strated_;
}

bool ApplicationManagerImpl::driver_distraction() const {
  return is_distracting_driver_;
}

inline bool ApplicationManagerImpl::all_apps_allowed() const {
  return is_all_apps_allowed_;
}
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_H_
