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

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_TEST_INCLUDE_HMI_MESSAGE_HANDLER_MOCK_DBUS_MESSAGE_CONTROLLER_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_TEST_INCLUDE_HMI_MESSAGE_HANDLER_MOCK_DBUS_MESSAGE_CONTROLLER_H_

#include <pthread.h>
#include "hmi_message_handler/dbus_message_controller.h"

namespace test {
namespace components {
namespace hmi_message_handler_test {

class MockDBusMessageController
    : public ::hmi_message_handler::DBusMessageController {
 public:
  MOCK_METHOD1(Recv, void(std::string&));

  MockDBusMessageController(const std::string& serviceName,
                            const std::string& path)
      : DBusMessageController(serviceName, path), thread_() {}

  virtual void processResponse(std::string method, Json::Value& root) {}
  virtual void processRequest(Json::Value& root) {}
  virtual void processNotification(Json::Value& root) {}

  bool Init() {
    return ::hmi_message_handler::DBusMessageController::Init() &&
           pthread_create(&thread_, 0, &Run, this) == 0;
  }

 private:
  pthread_t thread_;
  static void* Run(void* data) {
    if (NULL != data) {
      static_cast<MockDBusMessageController*>(data)
          ->MethodForReceiverThread(nullptr);
    }
    return 0;
  }
};

}  // namespace hmi_message_handler_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_TEST_INCLUDE_HMI_MESSAGE_HANDLER_MOCK_DBUS_MESSAGE_CONTROLLER_H_
