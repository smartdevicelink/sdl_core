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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_

#include <string.h>
#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace application_manager {

class Application;

namespace commands {

/**
 * @brief Register app interface request  command class
 **/
class RegisterAppInterfaceRequest : public CommandRequestImpl {
 public:
  /**
   * \brief RegisterAppInterfaceRequest class constructor
   **/
  explicit RegisterAppInterfaceRequest(const MessageSharedPtr& message);

  /**
   * @brief RegisterAppInterfaceRequest class destructor
   **/
  virtual ~RegisterAppInterfaceRequest();

  /**
   * @brief Init required by command resources
   **/
  virtual bool Init();

  /**
   * @brief Execute command
   **/
  virtual void Run();
  // virtual void cleanUp() = 0;

  /**
   * @brief Sends RegisterAppInterface response to mobile
   *
   *@param application_impl application
   *
   **/
  void SendRegisterAppInterfaceResponseToMobile(
      const Application& application_impl);

 private:

  /*
   * @brief Check new ID along with known mobile application ID
   *
   * return TRUE if ID is known already, otherwise - FALSE
   */
  bool IsApplicationRegistered();

  /*
   * @brief Check for some request param. names restrictions, e.g. for
   * newline characters
   *
   * return SUCCESS if param name pass the check, otherwise - error code
   * will be returned
   */
  mobile_apis::Result::eType CheckRestrictions() const;

  /*
   * @brief Removes hidden symbols and spaces
   *
   * return cleared copy of param name
   */
  std::string ClearParamName(std::string param_name) const;


  /*
   * @brief Check new application parameters (name, tts, vr) for
   * coincidence with already known parameters of registered applications
   *
   * return SUCCESS if there is no coincidence of app.name/TTS/VR synonyms,
   * otherwise appropriate error code returns
  */
  mobile_apis::Result::eType CheckCoincidence();

  /*
   * @brief Predicate for using with CheckCoincidence method to compare with TTS SO
   *
   * return TRUE if there is coincidence of TTS text field, otherwise FALSE
   */
  struct CoincidencePredicateTTS {
    explicit CoincidencePredicateTTS(const std::string &newItem)
    :newItem_(newItem)
    {};

    bool operator()(smart_objects::SmartObject obj) {
      const std::string text = obj[strings::text].asString();
      return !(strcasecmp(text.c_str(), newItem_.c_str()));
    };

    const std::string &newItem_;
  };

  /*
  * @brief Predicate for using with CheckCoincidence method to compare with VR synonym SO
  *
  * return TRUE if there is coincidence of VR, otherwise FALSE
  */
  struct CoincidencePredicateVR {
      explicit CoincidencePredicateVR(const std::string &newItem)
      :newItem_(newItem)
      {};

      bool operator()(smart_objects::SmartObject obj) {
        const std::string vr_synonym = obj.asString();
        return !(strcasecmp(vr_synonym.c_str(), newItem_.c_str()));
      };

      const std::string &newItem_;
    };


  DISALLOW_COPY_AND_ASSIGN(RegisterAppInterfaceRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
