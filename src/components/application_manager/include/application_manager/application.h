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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_

#include <string>

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace application_manager {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
typedef int ErrorCode;
// class Command;

enum APIVersion {
  kUnknownAPI = -1,
  kAPIV1 = 0,
  kAPIV2 = 1
};

struct Version {
  APIVersion min_supported_api_version;
  APIVersion max_supported_api_version;

  Version()
    : min_supported_api_version(APIVersion::kUnknownAPI),
      max_supported_api_version(APIVersion::kUnknownAPI) {
  }
};

class Application {
  public:
    virtual ~Application() {}
    /**
     * @brief Processes received pre-validated message: creates
     * appropriate command, if required adds to/removes from watchod
     * and sends to HMI/Mobile side.
     * @param message Points to message to be processed by Application
     */
    virtual void ProcessMessage(smart_objects::SmartObject* message) = 0;

    /**
     * @brief Called when on some stage of message processing outside
     * application error occured to generate command using factory.
     * @param message Message to be responded with error.
     * @param error_code Id of error occured.
     * TODO(PV): should we pass error_code along with possible error_string
     * in smartObject? If not need to add error_string
     */
    virtual void ReportError(smart_objects::SmartObject* message,
                             ErrorCode error_code) = 0;

    /**
     * @brief Returns message belonging to the application
     * that is currently executed (i.e. on HMI).
     * @return smart_objects::SmartObject * Active message
     */
    virtual const smart_objects::SmartObject* active_message() const = 0;
    virtual void CloseActiveMessage() = 0;
    virtual const Version& version() const = 0;
    virtual int app_id() const = 0;
    virtual const std::string& name() const = 0;
    virtual bool IsFullscreen() const = 0;
    virtual bool SupportsNavigation() const = 0;
    virtual void AllowNavigation(bool allow) = 0;

    virtual bool SubscribeToButton(unsigned int btn_name) = 0;
    virtual bool IsSubscribedToButton(unsigned int btn_name) = 0;
    virtual bool UnsubscribeFromButton(unsigned int btn_name) = 0;

    virtual bool SubscribeToIVI(unsigned int vehicle_info_type_) = 0;
    virtual bool IsSubscribedToIVI(unsigned int vehicle_info_type_) = 0;
    virtual bool UnsubscribeFromIVI(unsigned int vehicle_info_type_) = 0;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_H_
