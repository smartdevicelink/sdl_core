/*
 * Copyright (c) 2014, Ford Motor Company
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

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "application_manager/hmi_command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/message.h"
#include "application_manager/message_helper.h"
#include "application_manager/request_controller.h"
#include "application_manager/resume_ctrl.h"
#include "application_manager/vehicle_info_data.h"
#include "protocol_handler/protocol_observer.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "hmi_message_handler/hmi_message_sender.h"
#include "application_manager/policies/policy_handler_observer.h"
#include "media_manager/media_manager_impl.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "formatters/CSmartFactory.hpp"

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"

#include "interfaces/v4_protocol_v1_2_no_extra.h"
#include "interfaces/v4_protocol_v1_2_no_extra_schema.h"
#ifdef TIME_TESTER
#include "time_metric_observer.h"
#endif  // TIME_TESTER

#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/prioritized_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/lock.h"
#include "utils/singleton.h"
#include "utils/data_accessor.h"



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

enum VRTTSSessionChanging {
  kVRSessionChanging = 0,
  kTTSSessionChanging = 1
};

struct CommandParametersPermissions;

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
  explicit MessageFromMobile(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {
  }
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

struct MessageToMobile: public utils::SharedPtr<Message> {
  explicit MessageToMobile(const utils::SharedPtr<Message>& message,
                           bool final_message)
      : utils::SharedPtr<Message>(message),
        is_final(final_message) {
  }
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
  // Signals if connection to mobile must be closed after sending this message
  bool is_final;
};

struct MessageFromHmi: public utils::SharedPtr<Message> {
  explicit MessageFromHmi(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {
  }
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

struct MessageToHmi: public utils::SharedPtr<Message> {
  explicit MessageToHmi(const utils::SharedPtr<Message>& message)
      : utils::SharedPtr<Message>(message) {
  }
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const {
    return (*this)->Priority().OrderingValue();
  }
};

// Short type names for prioritized message queues
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromMobile> > FromMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToMobile> > ToMobileQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageFromHmi> > FromHmiQueue;
typedef threads::MessageLoopThread<utils::PrioritizedQueue<MessageToHmi> > ToHmiQueue;

// AudioPassThru
typedef struct  {
std::vector<uint8_t> binary_data;
int32_t              session_key;
} AudioData;
typedef std::queue<AudioData>                          RawAudioDataQueue;
typedef threads::MessageLoopThread<RawAudioDataQueue>  AudioPassThruQueue;
}

typedef std::vector<std::string> RPCParams;

class ApplicationManagerImpl : public ApplicationManager,
  public hmi_message_handler::HMIMessageObserver,
  public protocol_handler::ProtocolObserver,
  public connection_handler::ConnectionHandlerObserver,
  public policy::PolicyHandlerObserver,
  public impl::FromMobileQueue::Handler, public impl::ToMobileQueue::Handler,
  public impl::FromHmiQueue::Handler, public impl::ToHmiQueue::Handler,
  public impl::AudioPassThruQueue::Handler,
  public utils::Singleton<ApplicationManagerImpl> {

    friend class ResumeCtrl;
    friend class CommandImpl;

 public:
    ~ApplicationManagerImpl();

    /**
     * Inits application manager
     */
    virtual bool Init();

    /**
     * @brief Stop work.
     *
     * @return TRUE on success otherwise FALSE.
     **/
    virtual bool Stop();

    /////////////////////////////////////////////////////

    ApplicationSharedPtr application(uint32_t app_id) const;
    ApplicationSharedPtr application_by_policy_id(
        const std::string& policy_app_id) const;
    ApplicationSharedPtr active_application() const;
    std::vector<ApplicationSharedPtr> applications_by_button(uint32_t button);
    std::vector<ApplicationSharedPtr> applications_by_ivi(uint32_t vehicle_info);
    std::vector<ApplicationSharedPtr> applications_with_navi();

    /**
     * @brief Returns media application with LIMITED HMI Level if exist.
     *
     * @return Shared pointer to application if application does not
     * exist returns empty shared pointer.
     */
    ApplicationSharedPtr get_limited_media_application() const;

    /**
     * @brief Returns navigation application with LIMITED HMI Level if exist.
     *
     * @return Shared pointer to application if application does not
     * exist returns empty shared pointer
     */
    ApplicationSharedPtr get_limited_navi_application() const;

    /**
     * @brief Returns voice communication application with LIMITED HMI Level if exist.
     *
     * @return Shared pointer to application if application does not
     * exist returns empty shared pointer
     */
    ApplicationSharedPtr get_limited_voice_application() const;

    /**
     * @brief Check's if there are audio(media, voice communication or navi) applications
     *        exist in HMI_FULL or HMI_LIMITED level with same audible HMI type.
     *        Used for resumption.
     *
     * @param app Pointer to application to compare with
     *
     * @return true if exist otherwise false
     */
    bool DoesAudioAppWithSameHMITypeExistInFullOrLimited(ApplicationSharedPtr app) const;

    /**
     * @brief Notifies all components interested in Vehicle Data update
     * i.e. new value of odometer etc and returns list of applications
     * subscribed for event.
     * @param vehicle_info Enum value of type of vehicle data
     * @param new value (for integer values currently) of vehicle data
     */
    std::vector<ApplicationSharedPtr> IviInfoUpdated(
      VehicleDataType vehicle_info, int value);

    /////////////////////////////////////////////////////

    HMICapabilities& hmi_capabilities();

    /**
     * @brief ProcessQueryApp executes logic related to QUERY_APP system request.
     *
     * @param sm_object smart object wich is actually parsed json obtained within
     * system request.
     */
    void ProcessQueryApp(const smart_objects::SmartObject& sm_object);

#ifdef TIME_TESTER
    /**
     * @brief Setup observer for time metric.
     *
     * @param observer - pointer to observer
     */
    void SetTimeMetricObserver(AMMetricObserver* observer);
#endif  // TIME_TESTER

    ApplicationSharedPtr RegisterApplication(
      const utils::SharedPtr<smart_objects::SmartObject>& request_for_registration);
    /*
     * @brief Closes application by id
     *
     * @param app_id Application id
     * @param reason reason of unregistering application
     * @param is_resuming describes - is this unregister
     *        is normal or need to be resumed\
     * @param is_unexpected_disconnect
     * Indicates if connection was unexpectedly lost(TM layer, HB)
     */
    void UnregisterApplication(const uint32_t& app_id,
                               mobile_apis::Result::eType reason,
                               bool is_resuming = false,
                               bool is_unexpected_disconnect = false);

    /**
    * @brief Handle sequence for unauthorized application
    * @param app_id Application id
    */
    void OnAppUnauthorized(const uint32_t& app_id);

    /*
     * @brief Sets unregister reason for closing all registered applications
     * duringHU switching off
     *
     * @param reason Describes the reason for HU switching off
     */
    void SetUnregisterAllApplicationsReason(
      mobile_api::AppInterfaceUnregisteredReason::eType reason);

    /*
     * @brief Called on Master_reset or Factory_defaults
     * when User chooses to reset HU.
     * Resets Policy Table if applicable.
     */
    void HeadUnitReset(
        mobile_api::AppInterfaceUnregisteredReason::eType reason);

    /*
     * @brief Closes all registered applications
     */
    void UnregisterAllApplications();

    bool RemoveAppDataFromHMI(ApplicationSharedPtr app);
    bool LoadAppDataToHMI(ApplicationSharedPtr app);
    bool ActivateApplication(ApplicationSharedPtr app);

    /**
     * @brief Put application in FULL HMI Level if possible,
     *        otherwise put applicatuion other HMI level.
     *        do not send any notifications to mobile
     * @param app, application, that need to be puted in FULL
     * @return seted HMI Level
     */
    mobile_api::HMILevel::eType IsHmiLevelFullAllowed(ApplicationSharedPtr app);

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


    /**
     * @brief Notification from PolicyHandler about PTU.
     * Compares AppHMIType between saved in app and received from PTU. If they are different method sends:
     * UI_ChangeRegistration with list new AppHMIType;
     * ActivateApp with HMI level BACKGROUND;
     * OnHMIStatus notification;
     * for app with HMI level FULL or LIMITED.
     * method sends:
     * UI_ChangeRegistration with list new AppHMIType
     * for app with HMI level BACKGROUND.
     */
    virtual void OnUpdateHMIAppType(std::map<std::string, std::vector<std::string> > app_hmi_types);

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

    /*
     * @brief Creates AudioPassThru data chunk and inserts it
     * to audio_pass_thru_messages_
     *
     * @param session_key Id of application for which
     * audio pass thru should be sent
     *
     * @param binary_data AudioPassThru data chunk
     */
    void SendAudioPassThroughNotification(uint32_t session_key,
                                          std::vector<uint8_t>& binary_data);

    std::string GetDeviceName(connection_handler::DeviceHandle handle);

    /////////////////////////////////////////////////////

    void set_hmi_message_handler(hmi_message_handler::HMIMessageHandler* handler);
    void set_connection_handler(connection_handler::ConnectionHandler* handler);
    void set_protocol_handler(protocol_handler::ProtocolHandler* handler);

    ///////////////////////////////////////////////////////

    void StartDevicesDiscovery();

    // Put message to the queue to be sent to mobile.
    // if |final_message| parameter is set connection to mobile will be closed
    // after processing this message
    void SendMessageToMobile(const commands::MessageSharedPtr message,
                             bool final_message = false);

    bool ManageMobileCommand(
            const commands::MessageSharedPtr message,
            commands::Command::CommandOrigin origin =
            commands::Command::ORIGIN_SDL);
    void SendMessageToHMI(const commands::MessageSharedPtr message);
    bool ManageHMICommand(const commands::MessageSharedPtr message);

    /////////////////////////////////////////////////////////
    // Overriden ProtocolObserver method
    virtual void OnMessageReceived(
        const ::protocol_handler::RawMessagePtr message) OVERRIDE;
    virtual void OnMobileMessageSent(
        const ::protocol_handler::RawMessagePtr message) OVERRIDE;

    // Overriden HMIMessageObserver method
    void OnMessageReceived(hmi_message_handler::MessageSharedPointer message) OVERRIDE;
    void OnErrorSending(hmi_message_handler::MessageSharedPointer message) OVERRIDE;

    // Overriden ConnectionHandlerObserver method
    void OnDeviceListUpdated(const connection_handler::DeviceMap& device_list) OVERRIDE;
    //TODO (EZamakhov): fix all indentations in this file
    void OnFindNewApplicationsRequest() OVERRIDE;
    void RemoveDevice(const connection_handler::DeviceHandle& device_handle) OVERRIDE;
    bool OnServiceStartedCallback(
        const connection_handler::DeviceHandle& device_handle,
        const int32_t& session_key, const protocol_handler::ServiceType& type) OVERRIDE;
    void OnServiceEndedCallback(const int32_t& session_key,
                                const protocol_handler::ServiceType& type) OVERRIDE;
    void OnApplicationFloodCallBack(const uint32_t& connection_key) OVERRIDE;
    void OnMalformedMessageCallback(const uint32_t& connection_key) OVERRIDE;
    /**
     * @ Add notification to collection
     *
     * @param ptr Reference to shared pointer that point on hmi notification
     */
    void addNotification(const CommandSharedPtr ptr);

    /**
     * @ Add notification to collection
     *
     * @param ptr Reference to shared pointer that point on hmi notification
     */
    void removeNotification(const commands::Command* notification);

    /**
     * @ Updates request timeout
     *
     * @param connection_key Connection key of application
     * @param mobile_correlation_id Correlation ID of the mobile request
     * @param new_timeout_value New timeout in milliseconds to be set
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
    void set_application_id(const int32_t correlation_id, const uint32_t app_id);

    /**
     * @brief AddPolicyObserver allows to subscribe needed component to events
     * from policy.
     *
     * @param listener the component to subscribe.
     */
    void AddPolicyObserver(PolicyHandlerObserver* listener);

    /**
     * @brief RemovePolicyObserver allows to remove observer from collection.
     *
     * @param listener observer to remove.
     */
    void RemovePolicyObserver(PolicyHandlerObserver* listener);

    /*
     * @brief Change AudioStreamingState for all application according to
     * system audio-mixing capabilities (NOT_AUDIBLE/ATTENUATED) and
     * send notification for this changes
     * @param If changing_state == kVRSessionChanging function is used by
     * on_vr_started_notification, if changing_state == kTTSSessionChanging
     * function is used by on_tts_started_notification
     */
    void Mute(VRTTSSessionChanging changing_state);

    /*
     * @brief Change AudioStreamingState for all application to AUDIBLE and
     * send notification for this changes
     * @param If changing_state == kVRSessionChanging function is used by
     * on_vr_stopped_notification, if changing_state == kTTSSessionChanging
     * function is used by on_tts_stopped_notification
     */
    void Unmute(VRTTSSessionChanging changing_state);

    /*
     * @brief Checks HMI level and returns true if audio streaming is allowed
     */
    bool IsAudioStreamingAllowed(uint32_t connection_key) const;

    /*
     * @brief Checks HMI level and returns true if video streaming is allowed
     */
    bool IsVideoStreamingAllowed(uint32_t connection_key) const;

    /**
     * @brief CanAppStream allows to check whether application is permited for
     * data streaming.
     *
     * In case streaming for app is disallowed the method will send EndService to mobile.
     *
     * @param app_id the application id which should be checked.
     *
     * @return true in case streaming is allowed, false otherwise.
     */
    bool CanAppStream(uint32_t app_id) const;

    /**
     * @brief StreamingEnded Callback called from MediaManager when it decide that
     * streaming has been ended
     *
     * @param app_id the id of application that stops stream.
     */
    void StreamingEnded(uint32_t app_id);

    /**
     * @brief ForbidStreaming forbid the stream over the certain application.
     *
     * @param app_id the application's id which should stop streaming.
     */
    void ForbidStreaming(uint32_t app_id);

    mobile_api::HMILevel::eType GetDefaultHmiLevel(
        ApplicationSharedPtr application) const;

    /**
      * Getter for resume_controller
      * @return Resume Controller
      */
    ResumeCtrl& resume_controller() {
      return resume_ctrl_;
    }

    /**
     * Generate grammar ID
     *
     * @return New grammar ID
     */
    uint32_t GenerateGrammarID();

    /**
     * Generate new HMI application ID
     *
     * @return New HMI application ID
     */
    uint32_t GenerateNewHMIAppID();

    /**
     * @brief Parse smartObject and replace mobile app Id by HMI app ID
     *
     * @param message Smartobject to be parsed
     */
    void ReplaceMobileByHMIAppId(
        smart_objects::SmartObject& message);

    /**
     * @brief Parse smartObject and replace HMI app ID by mobile app Id
     *
     * @param message Smartobject to be parsed
     */
    void ReplaceHMIByMobileAppId(
        smart_objects::SmartObject& message);

    /*
     * @brief Save binary data to specified directory
     *
     * @param binary data
     * @param path for saving data
     * @param file_name File name
     * @param offset for saving data to existing file with offset.
     *        If offset is 0 - create new file ( overrite existing )
     *
     *
     * @return SUCCESS if file was saved, other code otherwise
     */
    mobile_apis::Result::eType SaveBinary(
        const std::vector<uint8_t>& binary_data,
        const std::string& file_path,
        const std::string& file_name,
        const int64_t offset);

    /**
     * @brief Get available app space
     * @param name of the app folder(make + mobile app id)
     * @return free app space.
     */
    uint32_t GetAvailableSpaceForApp(const std::string& folder_name);

    /*
     * @brief returns true if HMI is cooperating
     */
    bool IsHMICooperating() const;

    /**
     * @brief Method used to send default app tts globalProperties
     * in case they were not provided from mobile side after defined time
     */
    void OnTimerSendTTSGlobalProperties();

    /**
     * @brief method adds application
     * to tts_global_properties_app_list_
     * @param app_id contains application which will
     * send TTS global properties after timeout
     */
    void AddAppToTTSGlobalPropertiesList(const uint32_t app_id);

    /**
     * @brief method removes application
     * from tts_global_properties_app_list_
     * @param app_id contains application which will
     * send TTS global properties after timeout
     */
    void RemoveAppFromTTSGlobalPropertiesList(const uint32_t app_id);

    /**
     * @brief method adds application in FULL and LIMITED state
     * to on_phone_call_app_list_.
     * Also OnHMIStateNotification with BACKGROUND state sent for these apps
     */
    void CreatePhoneCallAppList();

    /**
     * @brief method removes application from on_phone_call_app_list_.
     *
     * Also OnHMIStateNotification with previous HMI state sent for these apps
     */
    void ResetPhoneCallAppList();

    /**
     * @brief ChangeAppsHMILevel the function that will change application's
     * hmi level.
     *
     * @param app_id id of the application whose hmi level should be changed.
     *
     * @param level new hmi level for certain application.
     */
    void ChangeAppsHMILevel(uint32_t app_id, mobile_apis::HMILevel::eType level);

    /**
     * @brief MakeAppNotAudible allows to make certain application not audible.
     *
     * @param app_id applicatin's id whose audible state should be changed.
     */
    void MakeAppNotAudible(uint32_t app_id);

    /**
     * @brief MakeAppFullScreen allows ti change application's properties
     * in order to make it full screen.
     *
     * @param app_id the id of application which should be in full screen  mode.
     *
     * @return true if operation was success, false otherwise.
     */
    bool MakeAppFullScreen(uint32_t app_id);

    /**
     * Function used only by HMI request/response/notification base classes
     * to change HMI app id to Mobile app id and vice versa.
     * Dot use it inside Core
     */
    ApplicationSharedPtr application_by_hmi_app(uint32_t hmi_app_id) const;

    // TODO(AOleynik): Temporary added, to fix build. Should be reworked.
    connection_handler::ConnectionHandler* connection_handler();

    /**
     * @brief Checks, if given RPC is allowed at current HMI level for specific
     * application in policy table
     * @param policy_app_id Application id
     * @param hmi_level Current HMI level of application
     * @param function_id FunctionID of RPC
     * @param params_permissions Permissions for RPC parameters (e.g.
     * SubscribeVehicleData) defined in policy table
     * @return SUCCESS, if allowed, otherwise result code of check
     */
    mobile_apis::Result::eType CheckPolicyPermissions(
        const std::string& policy_app_id,
        mobile_apis::HMILevel::eType hmi_level,
        mobile_apis::FunctionID::eType function_id,
        const RPCParams& rpc_params,
        CommandParametersPermissions* params_permissions = NULL);
    /*
     * @brief Function Should be called when Low Voltage is occured
     */
    void OnLowVoltage();

    /*
     * @brief Function Should be called when WakeUp occures after Low Voltage
     */
    void OnWakeUp();

    struct ApplicationsAppIdSorter {
      bool operator() (const ApplicationSharedPtr lhs,
                       const ApplicationSharedPtr rhs) {
        return lhs->app_id() < rhs->app_id();
      }
    };

    struct ApplicationsMobileAppIdSorter {
      bool operator() (const ApplicationSharedPtr lhs,
                       const ApplicationSharedPtr rhs) {
        return lhs->mobile_app_id() < rhs->mobile_app_id();
      }
    };

    // typedef for Applications list
    typedef std::set<ApplicationSharedPtr,
                     ApplicationsAppIdSorter> ApplictionSet;

    typedef std::set<ApplicationSharedPtr,
                     ApplicationsMobileAppIdSorter> AppsWaitRegistrationSet;

    // typedef for Applications list iterator
    typedef ApplictionSet::iterator ApplictionSetIt;

    // typedef for Applications list const iterator
    typedef ApplictionSet::const_iterator ApplictionSetConstIt;


    /**
     * Class for thread-safe access to applications list
     */
    class ApplicationListAccessor: public DataAccessor<ApplictionSet> {
     public:

      /**
       * @brief ApplicationListAccessor class constructor
       */
      ApplicationListAccessor() :
        DataAccessor<ApplictionSet>(ApplicationManagerImpl::instance()->applications_,
                     ApplicationManagerImpl::instance()->applications_list_lock_) {
      }

      ~ApplicationListAccessor();

      /**
       * @brief thread-safe getter for applications
       * @return applications list
       */
      const ApplictionSet& applications() const {
        return GetData();
      }

      ApplictionSetConstIt begin() {
        return applications().begin();
      }

      ApplictionSetConstIt end() {
        return applications().end();
      }

      template<class UnaryPredicate>
      ApplicationSharedPtr Find(UnaryPredicate finder) {
        ApplicationSharedPtr result;
        ApplictionSetConstIt it = std::find_if(begin(), end(), finder);
        if (it != end()) {
          result = *it;
        }
        return result;
      }

      template<class UnaryPredicate>
      std::vector<ApplicationSharedPtr> FindAll(UnaryPredicate finder) {
        std::vector<ApplicationSharedPtr> result;
        ApplictionSetConstIt it = std::find_if(begin(), end(), finder);
        while (it != end()) {
          result.push_back(*it);
          it  = std::find_if(++it, end(), finder);
        }
        return result;
      }

      void Erase(ApplicationSharedPtr app_to_remove) {
        ApplicationManagerImpl::instance()->applications_.erase(app_to_remove);
      }

      void Insert(ApplicationSharedPtr app_to_insert) {
        ApplicationManagerImpl::instance()->applications_.insert(app_to_insert);
      }

      bool Empty() {
        return ApplicationManagerImpl::instance()->applications_.empty();
      }

     private:
      DISALLOW_COPY_AND_ASSIGN(ApplicationListAccessor);
    };

    friend class ApplicationListAccessor;

    struct AppIdPredicate {
      uint32_t app_id_;
      AppIdPredicate(uint32_t app_id): app_id_(app_id) {}
      bool operator () (const ApplicationSharedPtr app) const {
        return app ? app_id_ == app->app_id() : false;
      }
    };

    struct HmiAppIdPredicate {
      uint32_t hmi_app_id_;
      HmiAppIdPredicate(uint32_t hmi_app_id): hmi_app_id_(hmi_app_id) {}
      bool operator () (const ApplicationSharedPtr app) const {
        return app ? hmi_app_id_ == app->hmi_app_id() : false;
      }
    };

    struct MobileAppIdPredicate {
      std::string policy_app_id_;
      MobileAppIdPredicate(const std::string& policy_app_id):
        policy_app_id_(policy_app_id) {}
      bool operator () (const ApplicationSharedPtr app) const {
        return app ? policy_app_id_ == app->mobile_app_id() : false;
      }
    };

    struct SubscribedToButtonPredicate {
      mobile_apis::ButtonName::eType button_;
      SubscribedToButtonPredicate(mobile_apis::ButtonName::eType button)
        : button_(button) {}
      bool operator () (const ApplicationSharedPtr app) const {
        return app ? app->IsSubscribedToButton(button_) : false;
      }
    };

    struct SubscribedToIVIPredicate {
      int32_t vehicle_info_;
      SubscribedToIVIPredicate(int32_t  vehicle_info)
        : vehicle_info_(vehicle_info) {}
      bool operator () (const ApplicationSharedPtr app) const {
        return app ? app->IsSubscribedToIVI(vehicle_info_) : false;
      }
    };

  private:
    ApplicationManagerImpl();

    /**
     * @brief Method transforms string to AppHMIType
     * @param str contains string AppHMIType
     * @return enum AppHMIType
     */
    mobile_apis::AppHMIType::eType StringToAppHMIType(std::string str);

    /**
     * @brief Method compares arrays of app HMI type
     * @param from_policy contains app HMI type from policy
     * @param from_application contains app HMI type from application
     * @return return TRUE if arrays of appHMIType equal, otherwise return FALSE
     */
    bool CompareAppHMIType (const smart_objects::SmartObject& from_policy,
                            const smart_objects::SmartObject& from_application);

    hmi_apis::HMI_API& hmi_so_factory();
    mobile_apis::MOBILE_API& mobile_so_factory();

    bool ConvertMessageToSO(const Message& message,
                            smart_objects::SmartObject& output);
    bool ConvertSOtoMessage(const smart_objects::SmartObject& message,
                            Message& output);
    utils::SharedPtr<Message> ConvertRawMsgToMessage(
      const ::protocol_handler::RawMessagePtr message);

    void ProcessMessageFromMobile(const utils::SharedPtr<Message> message);
    void ProcessMessageFromHMI(const utils::SharedPtr<Message> message);

    // threads::MessageLoopThread<*>::Handler implementations
    /*
     * @brief Handles for threads pumping different types
     * of messages. Beware, each is called on different thread!
     */
    // CALLED ON messages_from_mobile_ thread!
    virtual void Handle(const impl::MessageFromMobile message) OVERRIDE;

    // CALLED ON messages_to_mobile_ thread!
    virtual void Handle(const impl::MessageToMobile message) OVERRIDE;

    // CALLED ON messages_from_hmi_ thread!
    virtual void Handle(const impl::MessageFromHmi message) OVERRIDE;

    // CALLED ON messages_to_hmi_ thread!
    virtual void Handle(const impl::MessageToHmi message) OVERRIDE;

    // CALLED ON audio_pass_thru_messages_ thread!
    virtual void Handle(const impl::AudioData message) OVERRIDE;

    void SendUpdateAppList();

    template<typename ApplicationList>
    void PrepareApplicationListSO(ApplicationList app_list,
                                  smart_objects::SmartObject& applications) {
      CREATE_LOGGERPTR_LOCAL(logger_, "ApplicatinManagerImpl");

      uint32_t app_count = 0;
      typename ApplicationList::const_iterator it;
      for (it = app_list.begin(); it != app_list.end(); ++it) {
        if (!it->valid()) {
          LOG4CXX_ERROR(logger_, "Application not found ");
          continue;
        }

        smart_objects::SmartObject hmi_application(smart_objects::SmartType_Map);;
        if (MessageHelper::CreateHMIApplicationStruct(*it, hmi_application)) {
          applications[app_count++] = hmi_application;
        } else {
          LOG4CXX_DEBUG(logger_, "Can't CreateHMIApplicationStruct ");
        }
      }

      if (0 == app_count) {
        LOG4CXX_WARN(logger_, "Empty applications list");
      }
    }

    void OnApplicationListUpdateTimer();

    /**
     * @brief CreateApplications creates aplpication adds it to application list
     * and prepare data for sending AppIcon request.
     *
     * @param obj_array applications array.
     *
     * @param app_icon_dir application icons directory
     *
     * @param apps_with_icon container which store application and it's icon path.
     */
    void CreateApplications(smart_objects::SmartArray& obj_array);

    /*
     * @brief Function is called on IGN_OFF, Master_reset or Factory_defaults
     * to notify HMI that SDL is shutting down.
     */
    void SendOnSDLClose();

    /*
     * @brief returns true if low voltage state is active
     */
    bool IsLowVoltage();

  private:

    /**
     * @brief OnHMILevelChanged the callback that allows SDL to react when
     * application's HMILeval has been changed.
     *
     * @param app_id application identifier for which HMILevel has been chaned.
     *
     * @param from previous HMILevel.
     * @param to current HMILevel.
     */
    void OnHMILevelChanged(uint32_t app_id,
                           mobile_apis::HMILevel::eType from,
                           mobile_apis::HMILevel::eType to);

    /**
     * @brief EndNaviServices either send EndService to mobile or proceed
     * unregister application procedure.
     */
    void EndNaviServices();

    /**
     * @brief CloseNaviApp allows to unregister application in case the EndServiceEndedAck
     * didn't come for at least one of services(audio or video).
     */
    void CloseNaviApp();

    /**
     * @brief AckReceived allows to distinguish if ack for appropriate service
     * has been received (means EndServiceAck).
     *
     * @param type service type.
     *
     * @return in case EndService has been sent and appropriate ack has been
     * received it returns true. In case no EndService for appropriate serevice type
     * has been sent and no ack has been received it returns true as well.
     * Otherwise it will return false.
     *
     */
    bool AckReceived(protocol_handler::ServiceType type);

    /**
     * @brief NaviAppChangeLevel the callback which reacts on case when applications
     * hmi level has been changed.
     */
    void NaviAppChangeLevel(mobile_apis::HMILevel::eType new_level);

    /**
     * @brief ChangeStreamStatus allows to process streaming state.
     *
     * @param app_id id of application whose stream state has been changed.
     *
     * @param can_stream streaming state if true - streaming active, if false
     * streaming is not active.
     */
    void ChangeStreamStatus(uint32_t app_id, bool can_stream);

    /**
     * @brief ProcessNaviService allows to start navi service
     *
     * @param type service type.
     *
     * @param connection_key the application id.
     */
    bool ProcessNaviService(protocol_handler::ServiceType type, uint32_t connection_key);

    /**
     * @brief NaviAppStreamStatus allows to handle case when navi streaming state
     * has ben changed from streaming to non streaming and vise versa.
     *
     * @param stream_active the stream's state - is it streams or not.
     */
    void NaviAppStreamStatus(bool stream_active);


    /**
     * @brief Function returns supported SDL Protocol Version
     * @return protocol version depends on parameters from smartDeviceLink.ini.
     */
    ProtocolVersion SupportedSDLVersion() const;

    // members

    /**
     * @brief List of applications
     */
    ApplictionSet applications_;
    AppsWaitRegistrationSet apps_to_register_;

    // Lock for applications list
    mutable sync_primitives::Lock applications_list_lock_;
    mutable sync_primitives::Lock apps_to_register_list_lock_;

    /**
     * @brief Map of correlation id  and associated application id.
     */
    std::map<const int32_t, const uint32_t> appID_list_;

    /**
     * @brief Map contains applications which
     * will send TTS global properties to HMI after timeout
     */
    std::map<uint32_t, TimevalStruct> tts_global_properties_app_list_;


    struct AppState {
      AppState(const mobile_apis::HMILevel::eType& level,
               const mobile_apis::AudioStreamingState::eType& streaming_state,
               const mobile_apis::SystemContext::eType& context)
      : hmi_level(level),
        audio_streaming_state(streaming_state),
        system_context(context) { }

      mobile_apis::HMILevel::eType            hmi_level;
      mobile_apis::AudioStreamingState::eType audio_streaming_state;
      mobile_apis::SystemContext::eType       system_context;
    };

    /**
     * @brief Map contains apps with HMI state before incoming call
     * After incoming call ends previous HMI state must restore
     *
     */
    std::map<uint32_t, AppState> on_phone_call_app_list_;

    bool audio_pass_thru_active_;
    sync_primitives::Lock audio_pass_thru_lock_;
    sync_primitives::Lock tts_global_properties_app_list_lock_;
    bool is_distracting_driver_;
    bool is_vr_session_strated_;
    bool hmi_cooperating_;
    bool is_all_apps_allowed_;
    media_manager::MediaManager* media_manager_;

    hmi_message_handler::HMIMessageHandler* hmi_handler_;
    connection_handler::ConnectionHandler*  connection_handler_;
    protocol_handler::ProtocolHandler*      protocol_handler_;
    request_controller::RequestController   request_ctrl_;

    hmi_apis::HMI_API*                      hmi_so_factory_;
    mobile_apis::MOBILE_API*                mobile_so_factory_;

    static uint32_t corelation_id_;
    static const uint32_t max_corelation_id_;


    // Construct message threads when everything is already created

    // Thread that pumps messages coming from mobile side.
    impl::FromMobileQueue messages_from_mobile_;
    // Thread that pumps messages being passed to mobile side.
    impl::ToMobileQueue messages_to_mobile_;
    // Thread that pumps messages coming from HMI.
    impl::FromHmiQueue messages_from_hmi_;
    // Thread that pumps messages being passed to HMI.
    impl::ToHmiQueue messages_to_hmi_;
    // Thread that pumps messages audio pass thru to mobile.
    impl::AudioPassThruQueue audio_pass_thru_messages_;


    HMICapabilities                         hmi_capabilities_;
    // The reason of HU shutdown
    mobile_api::AppInterfaceUnregisteredReason::eType unregister_reason_;

    /**
     * @brief Resume controler is responcible for save and load information
     * about persistent application data on disk, and save session ID for resuming
     * application in case INGITION_OFF or MASTER_RESSET
     */
    ResumeCtrl resume_ctrl_;

    // The map contains service type as a key and pair as a value.
    // The pair meaning is: first item shows if EndService has been sent and
    // the second one shows if appropriate ACK has been received.
    std::map<protocol_handler::ServiceType, std::pair<bool, bool> > service_status_;

    timer::TimerThread<ApplicationManagerImpl> end_services_timer;
    uint32_t wait_end_service_timeout_;
    uint32_t navi_app_to_stop_;


#ifdef TIME_TESTER
    AMMetricObserver* metric_observer_;
#endif  // TIME_TESTER

    class ApplicationListUpdateTimer : public timer::TimerThread<ApplicationManagerImpl> {
     public:
      ApplicationListUpdateTimer(ApplicationManagerImpl* callee) :
          timer::TimerThread<ApplicationManagerImpl>("AM ListUpdater",
              callee, &ApplicationManagerImpl::OnApplicationListUpdateTimer) {
      }
    };
    typedef utils::SharedPtr<ApplicationListUpdateTimer> ApplicationListUpdateTimerSptr;
    ApplicationListUpdateTimerSptr application_list_update_timer_;

    timer::TimerThread<ApplicationManagerImpl>  tts_global_properties_timer_;

    bool is_low_voltage_;
    DISALLOW_COPY_AND_ASSIGN(ApplicationManagerImpl);

    FRIEND_BASE_SINGLETON_CLASS(ApplicationManagerImpl);
};

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
