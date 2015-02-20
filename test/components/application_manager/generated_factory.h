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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_

#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#include "application_manager/mobile_command_factory.h"
#include "utils/shared_ptr.h"

//#include "formatters/CFormatterJsonSDLRPCv2.hpp"
//#include "formatters/formatter_json_rpc.h"

#include "interfaces/MOBILE_API.h"
#include "interfaces/MOBILE_API_schema.h"
#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"

#include "utils/threads/thread.h"

namespace test_command {

class SomeClass : public threads::ThreadDelegate {
  public:
    void threadMain();
};

void SomeClass::threadMain() {
  hmi_apis::HMI_API factory;
  while (1) {
    std::cout << "1" << std::endl;
  }
}

}
/*namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace sos = NsSmartDeviceLink::NsJSONHandler::strings;
*/
TEST(generated_factory, create) {
  threads::Thread* thread = new threads::Thread(
    "application_manager::SomeClass",
    new test_command::SomeClass);

  if (!thread->start(
        threads::ThreadOptions(16384))) {
    std::cout << "Something went wrong with thread." << std::endl;
    return;
  }

  sleep(2);
  thread->stop();

}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_FORMATTERS_COMMANDS_H_
