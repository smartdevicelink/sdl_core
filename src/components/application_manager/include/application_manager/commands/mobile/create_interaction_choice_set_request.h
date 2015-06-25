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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_CREATE_INTERACTION_CHOICE_SET_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_CREATE_INTERACTION_CHOICE_SET_REQUEST_H_
#include <map>
#include <string>

#include "application_manager/application.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/event_engine/event_observer.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"

namespace application_manager {

class Application;

namespace commands {

/**
 * @brief CreateInteractionChoiceSetRequest command class
 **/
class CreateInteractionChoiceSetRequest : public CommandRequestImpl {
 public:
    /**
     * @brief CreateInteractionChoiceSetRequest class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit CreateInteractionChoiceSetRequest(const MessageSharedPtr& message);

    /**
     * @brief CreateInteractionChoiceSetRequest class destructor
     **/
    virtual ~CreateInteractionChoiceSetRequest();

    /**
     * @brief Execute command
     **/
    virtual void Run();


 private:
    /**
     * @brief Interface method that is called whenever new event received
     *
     * @param event The received event
     */
    virtual void on_event(const event_engine::Event& event);

    /**
     * @brief Function is called by RequestController when request execution time
     * has exceed it's limit
     */
    virtual void onTimeOut();
    /**
     * @brief DeleteChoices allows to walk through the sent commands collection
     * in order to sent appropriate DeleteCommand request.
     */
    void DeleteChoices();

    /**
     * @brief Calls after all responses from HMI were received.
     * Terminates request and sends successful response to mobile
     * if all responses were SUCCESS or calls DeleteChoices in other case.
     */
    void OnAllHMIResponsesReceived();

    /**
     * @brief The VRCommand struct
     * Collect minimum information about sent VR commands, for correctly
     * processing deleting sent commands if error from HMI received
     */
    struct VRCommandInfo {
      VRCommandInfo() {}
      explicit VRCommandInfo(uint32_t cmd_id):
        cmd_id_(cmd_id),
        succesful_response_received_(false) {}
      uint32_t cmd_id_;
      bool succesful_response_received_;
    };

    typedef std::map<uint32_t, VRCommandInfo> SentCommandsMap;
    SentCommandsMap sent_commands_map_;

    int32_t choice_set_id_;
    size_t expected_chs_count_;
    size_t received_chs_count_;

    /**
     * @brief Flag for stop sending VR commands to HMI, in case one of responses
     * failed
     */
    volatile bool error_from_hmi_;
    sync_primitives::Lock error_from_hmi_lock_;

    /**
     * @brief Flag shows if request already was expired by timeout
     */
    volatile bool is_timed_out_;
    sync_primitives::Lock is_timed_out_lock_;

    sync_primitives::Lock vr_commands_lock_;
    /*
     * @brief Sends VR AddCommand request to HMI
     *
     * @param app_id Application ID
     *
     */
    void SendVRAddCommandRequests(ApplicationSharedPtr const app);

    /*
     * @brief Checks incoming choiseSet params.
     * @param app Registred mobile application
     *
     * @return Mobile result code
     */
    mobile_apis::Result::eType CheckChoiceSet(ApplicationConstSharedPtr app);

    /*
    * @brief Predicate for using with CheckChoiceSet method to compare choice ID param
    *
    * return TRUE if there is coincidence of choice ID, otherwise FALSE
    */
    struct CoincidencePredicateChoiceID {
      explicit CoincidencePredicateChoiceID(const uint32_t newItem)
        : newItem_(newItem)
      {}

      bool operator()(smart_objects::SmartObject obj) {
        return obj[strings::choice_id].asUInt() == newItem_;
      }

      const uint32_t newItem_;
    };

    /*
    * @brief Predicate for using with CheckChoiceSet method to compare menu name param
    *
    * return TRUE if there is coincidence of menu name, otherwise FALSE
    */
    struct CoincidencePredicateMenuName {
      explicit CoincidencePredicateMenuName(const std::string& newItem)
        : newItem_(newItem)
      {};

      bool operator()(smart_objects::SmartObject obj) {
        return obj[strings::menu_name].asString() == newItem_;
      }

      const std::string& newItem_;
    };

    /*
    * @brief Predicate for using with CheckChoiceSet method to compare VR commands param
    *
    * return TRUE if there is coincidence of VR commands, otherwise FALSE
    */
    struct CoincidencePredicateVRCommands {
      explicit CoincidencePredicateVRCommands(
            const smart_objects::SmartObject& newItem): newItem_(newItem) {}

      bool operator()(smart_objects::SmartObject obj) {
        return compareStr(obj, newItem_);
      }

      const smart_objects::SmartObject& newItem_;
    };

    /*
     * @brief Checks if incoming choice set doesn't has similar VR synonyms.
     *
     * @param choice1  Choice to compare
     * @param choice2  Choice to compare
     *
     * return Return TRUE if there are similar VR synonyms in choice set,
     * otherwise FALSE
    */
    bool compareSynonyms(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice1,
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice2);

    /*
     * @brief Checks VR synonyms ignoring differences in case.
     *
     * @param str1 VR synonym to compare
     * @param str2 VR synonym to compare
     *
     * return Return TRUE if there are similar VR synonyms in choice set,
     * otherwise FALSE
    */
    static bool compareStr(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& str1,
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& str2);

    /**
     * @brief Checks choice set params(menuName, tertiaryText, ...)
     * When type is String there is a check on the contents \t\n \\t \\n
     * @param choice_set which must check
     * @return if choice_set contains \t\n \\t \\n return TRUE, FALSE otherwise
     */
    bool IsWhiteSpaceExist(const smart_objects::SmartObject& choice_set);

    DISALLOW_COPY_AND_ASSIGN(CreateInteractionChoiceSetRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_CREATE_INTERACTION_CHOICE_SET_REQUEST_H_
