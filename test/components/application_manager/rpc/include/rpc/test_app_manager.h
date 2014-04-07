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
#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_TEST_APP_MANAGER_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_TEST_APP_MANAGER_H_

#include <string>

#include "rpc/application_manager_impl_test.h"
#include "rpc/init_hmi.h"

#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "application_manager/mobile_message_handler.h"

#include "life_cycle.h"

namespace am = application_manager;
namespace smart = NsSmartDeviceLink::NsSmartObjects;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;

// void RegistrSO(utils::SharedPtr<smart::SmartObject> AppRegRequest);

// utils::SharedPtr<protocol_handler::RawMessage>
//    ConvertSOToRawMess(utils::SharedPtr<smart::SmartObject> so);

namespace test {

TEST(TestApplicationManager, RegApp) {
  /****************************************************************************/
  /**
   * Test without ConnectionHandle Device
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes;
  am::ApplicationSharedPtr sh_app;
  am::ApplicationSharedPtr temp_sh_app;
  test::ApplicationManagerImplTest app_manager_test;

  am::ApplicationManagerImpl* app_manager =
  am::ApplicationManagerImpl::instance();

  RegistrSO(so_reg);

  raw_mes = ConvertSOToRawMess(so_reg);

  app_manager->OnMessageReceived(raw_mes);
  sleep(3);
  sh_app = app_manager->application(65546);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Valid add app
   */
  AddDevice(10);

  app_manager->OnMessageReceived(raw_mes);

  sleep(3);
  sh_app = app_manager->application(65546);

  EXPECT_TRUE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is DUPLICATE_NAME
   */
  utils::SharedPtr<smart::SmartObject> so_reg2(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes2;

  RegistrSO(so_reg2);

  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";
  (*so_reg2)[jsn::S_PARAMS][am::strings::connection_key] = 65547;
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
      "VR SyncProxyTester2";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  AddDevice(11);
  sleep(1);
  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is INVALID_DATA - Failed to parse string to smart object
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is INVALID_DATA - Param names restrictions check failed
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "\n";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test tts_name is INVALID_DATA -Failed to parse string to smart object
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester2";

  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][0]["text"] = "";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][0]["type"] = "TEXT";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test tts_name is INVALID_DATA - Param names restrictions check failed
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][0]["text"] = "\n";


  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test tts_name is INVALID_DATA -Failed to parse string to smart object
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][0]["text"] =
      "SyncProxyTesterTTS2";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][1]["text"] = "";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][1]["type"] = "TEXT";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test tts_name is INVALID_DATA - Param names restrictions check failed
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][1]["text"] =
      "\n";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test vr_synonyms is DUPLICATE_NAME - Some VR synonyms are known already
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][1]["text"] =
      "TTSName";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
      "VR SyncProxyTester";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_id is INVALID_DATA - duplicate
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
      "VR SyncProxyTester2";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65537";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Valid add app
   */

  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(65547);

  EXPECT_TRUE(sh_app.valid());

}
}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_TEST_APP_MANAGER_H_
