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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_SMOKE_TEST_EXP_MEDIA_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_SMOKE_TEST_EXP_MEDIA_H_

#include <stdio.h>
#include <libxml/xmlreader.h>
#include <string>

#include "rpc/xml_parser.h"
#include "rpc/add_test.h"
#include "rpc/init_start_data.h"
#include "rpc/protocol_handler_mockup.h"
#include "rpc/hmi_message_handler_tester.h"

#include "smart_objects/smart_object.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/application_impl.h"

namespace am = application_manager;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;
namespace smart = NsSmartDeviceLink::NsSmartObjects;

namespace test {
  /**
   * @brief Function read test from xml files and create Gtest to verify Mobile
   * response and HMI request
   *
   * @param Patch to folder has xml files
   *
   */
  void SmokeTestExpMedia(const char* patch);

  /**
   * @brief Function checks whether remains untested HMI request
   *
   * @return true if not request or false otherwise
   *
   */
  bool CheckNotTestedHMIRequest();
}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_SMOKE_TEST_EXP_MEDIA_H_

