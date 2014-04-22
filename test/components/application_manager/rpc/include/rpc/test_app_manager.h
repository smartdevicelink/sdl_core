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
#include <list>
#include <vector>

#include "rpc/application_manager_impl_test.h"
#include "rpc/init_hmi.h"

#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "application_manager/mobile_message_handler.h"
#include "application_manager/commands/command_impl.h"

#include "life_cycle.h"

namespace am = application_manager;
namespace smart = NsSmartDeviceLink::NsSmartObjects;
namespace jsn = NsSmartDeviceLink::NsJSONHandler::strings;

namespace test {

namespace connect_key {

enum eType {
  FIRST_CONNECTION_KEY = 65546,
  SECOND_CONNECTION_KEY = 65547
};
}

namespace device_id {

enum eType {
  FIRST_DEVICE_ID = 10,
  SECOND_DEVICE_ID = 11
};
}

TEST(TestApplicationManager, application) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  /****************************************************************************/
  /**
   * Check app
   */
  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  EXPECT_TRUE(sh_app_first.valid());

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, active_application) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  /****************************************************************************/
  /**
   * Check active_application
   */
  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  (*sh_app_first).set_hmi_level(am::mobile_api::HMILevel::HMI_FULL);

  am::ApplicationSharedPtr sh_app_second(app_manager->active_application());

  EXPECT_EQ(sh_app_first, sh_app_second);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, applications_by_button) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  /****************************************************************************/
  /**
   * Check applications_by_button
   */
  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  (*sh_app_first).SubscribeToButton(mobile_apis::ButtonName::SEEKLEFT);

  std::vector<am::ApplicationSharedPtr> vect_app =
      app_manager->applications_by_button(
          mobile_apis::ButtonName::SEEKLEFT);

  EXPECT_EQ(vect_app[0], sh_app_first);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, IviInfoUpdated) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  /****************************************************************************/
  /**
   * Check IviInfoUpdated
   */
  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  (*sh_app_first).SubscribeToIVI(
      mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL);

  std::vector<am::ApplicationSharedPtr> vect_app = app_manager->IviInfoUpdated(
      am::VehicleDataType::FUELLEVEL, 0);

  EXPECT_EQ(vect_app[0], sh_app_first);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, applications_with_navi) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  /****************************************************************************/
  /**
   * Check applications_with_navi
   */
  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  (*sh_app_first).set_allowed_support_navigation(true);

  std::vector<am::ApplicationSharedPtr> vect_app =
      app_manager->applications_with_navi();

  EXPECT_EQ(vect_app[0], sh_app_first);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}


TEST(TestApplicationManager, RegApp) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes;
  am::ApplicationSharedPtr sh_app;
  am::ApplicationSharedPtr temp_sh_app;
  test::ApplicationManagerImplTest app_manager_test;

  am::ApplicationManagerImpl* app_manager =
  am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
    hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor =
    new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);
  sleep(1);

  /****************************************************************************/
  /**
   * Test without ConnectionHandle Device
   */
  RegistrSO(so_reg);

  raw_mes = ConvertSOToRawMess(so_reg);

  app_manager->OnMessageReceived(raw_mes);
  sleep(3);
  sh_app = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Valid add app
   */
  AddDevice(device_id::FIRST_DEVICE_ID);

  app_manager->OnMessageReceived(raw_mes);

  sleep(3);
  sh_app = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  EXPECT_TRUE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is DUPLICATE_NAME
   */
  utils::SharedPtr<smart::SmartObject> so_reg2(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes2;

  RegistrSO(so_reg2);

  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";
  (*so_reg2)[jsn::S_PARAMS][am::strings::connection_key] =
      connect_key::SECOND_CONNECTION_KEY;
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
  "VR SyncProxyTester2";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  AddDevice(device_id::SECOND_DEVICE_ID);
  sleep(1);
  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is INVALID_DATA - Failed to parse string to smart object
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_name is INVALID_DATA - Param names restrictions check failed
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_name] = "\n";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

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
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test tts_name is INVALID_DATA - Param names restrictions check failed
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::tts_name][0]["text"] = "\n";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

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
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

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
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

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
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Test app_id is INVALID_DATA - duplicate
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
  "VR SyncProxyTester2";
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65546";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_FALSE(sh_app.valid());

  /****************************************************************************/
  /**
   * Valid add app
   */
  (*so_reg2)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";

  raw_mes2 = ConvertSOToRawMess(so_reg2);

  app_manager->OnMessageReceived(raw_mes2);

  sleep(3);
  sh_app = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  EXPECT_TRUE(sh_app.valid());

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(2);
  app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(2);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  RemovedDevice(device_id::SECOND_DEVICE_ID);
  sleep(2);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, RemoveAppDataFromHMI) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();
  am::ApplicationSharedPtr app;

  EXPECT_TRUE(app_manager->RemoveAppDataFromHMI(app));
}

TEST(TestApplicationManager, LoadAppDataToHMI) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();
  am::ApplicationSharedPtr app;

  EXPECT_TRUE(app_manager->LoadAppDataToHMI(app));
}

TEST(TestApplicationManager, ActivateApplication) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes;
  am::ApplicationSharedPtr sh_app_first;
  am::ApplicationSharedPtr sh_app_second;

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);
  sleep(2);
  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";
  (*so_reg)[jsn::S_PARAMS][am::strings::connection_key] =
      connect_key::SECOND_CONNECTION_KEY;
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester2";
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
  "VR SyncProxyTester2";

  sh_app_second = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  if (sh_app_second.valid()) {
    app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }
  sh_app_second = app_manager->RegisterApplication(so_reg);

  if (!sh_app_second.valid()) {
    AddDevice(device_id::SECOND_DEVICE_ID);
    sleep(1);
    sh_app_second = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_second.valid());
  }

  am::mobile_api::HMILevel::eType hmi_limit =
      am::mobile_api::HMILevel::HMI_LIMITED;

  am::mobile_api::HMILevel::eType hmi_full = am::mobile_api::HMILevel::HMI_FULL;

  am::mobile_api::HMILevel::eType hmi_background =
      am::mobile_api::HMILevel::HMI_BACKGROUND;

  array_message->clear();
  /****************************************************************************/
  /**
   * app->app_id() == curr_app->app_id() - true
   * curr_app->IsFullscreen() - true
   */
  (*sh_app_first).MakeFullscreen();

  EXPECT_FALSE(app_manager->ActivateApplication(sh_app_first));

  /****************************************************************************/
  /**
   * curr_app->hmi_level() != HMI_LIMITED - true
   * curr_app->has_been_activated() - true
   */

  (*sh_app_first).set_hmi_level(hmi_background);

  app_manager->ActivateApplication(sh_app_first);
  sleep(2);

  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * is_new_app_media - true
   * curr_app->IsAudible() - true
   */

  (*sh_app_first).set_is_media_application(true);
  (*sh_app_first).set_hmi_level(hmi_background);

  (*sh_app_second).set_hmi_level(hmi_limit);

  app_manager->ActivateApplication(sh_app_first);
  sleep(2);

  EXPECT_EQ(array_message->size(), 2);

  /****************************************************************************/
  /**
   * curr_app->IsFullscreen() - true
   */

  (*sh_app_first).set_is_media_application(false);
  (*sh_app_first).set_hmi_level(hmi_background);

  (*sh_app_second).set_hmi_level(hmi_full);

  app_manager->ActivateApplication(sh_app_first);
  sleep(2);

  EXPECT_EQ(array_message->size(), 5);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);
  app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  RemovedDevice(11);
  sleep(2);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, PutApplicationInLimited) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes;
  am::ApplicationSharedPtr sh_app_first;
  am::ApplicationSharedPtr sh_app_second;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor =
      new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
  }

  AddDevice(device_id::FIRST_DEVICE_ID);

  RegistrSO(so_reg);

  sh_app_first = app_manager->RegisterApplication(so_reg);
  sleep(3);

  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";
  (*so_reg)[jsn::S_PARAMS][am::strings::connection_key] =
      connect_key::SECOND_CONNECTION_KEY;
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester2";
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
  "VR SyncProxyTester2";

  sh_app_first = app_manager->application(connect_key::SECOND_CONNECTION_KEY);
  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
  }

  AddDevice(device_id::SECOND_DEVICE_ID);

  sh_app_first = app_manager->RegisterApplication(so_reg);
  sleep(3);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  sh_app_second = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  am::mobile_api::HMILevel::eType hmi_limit =
  am::mobile_api::HMILevel::HMI_LIMITED;
  am::mobile_api::HMILevel::eType hmi_full = am::mobile_api::HMILevel::HMI_FULL;
  am::mobile_api::HMILevel::eType return_put;
  /****************************************************************************/
  /**
   * curr_app->hmi_level() - mobile_api::HMILevel::HMI_LIMITED
   */
  (*sh_app_first).set_is_media_application(true);
  (*sh_app_first).set_hmi_level(hmi_limit);

  return_put = app_manager->PutApplicationInLimited(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_BACKGROUND);

  /****************************************************************************/
  /**
   * curr_app->hmi_level() - mobile_api::HMILevel::HMI_FULL
   * curr_app->is_media_application() - true
   */
  (*sh_app_first).set_hmi_level(hmi_full);

  return_put = app_manager->PutApplicationInLimited(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_BACKGROUND);

  /****************************************************************************/
  /**
   * curr_app->hmi_level() - mobile_api::HMILevel::HMI_FULL
   * curr_app->is_media_application() - false
   */
  (*sh_app_first).set_is_media_application(false);

  return_put = app_manager->PutApplicationInLimited(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_LIMITED);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);
  app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  RemovedDevice(device_id::SECOND_DEVICE_ID);
  sleep(2);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, PutApplicationInFull) {
  /****************************************************************************/
  /**
   * Init params
   */

  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<protocol_handler::RawMessage> raw_mes;
  am::ApplicationSharedPtr sh_app_first;
  am::ApplicationSharedPtr sh_app_second;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor =
      new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
  }

  AddDevice(device_id::FIRST_DEVICE_ID);

  RegistrSO(so_reg);

  sh_app_first = app_manager->RegisterApplication(so_reg);
  sleep(3);

  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_id] = "65547";
  (*so_reg)[jsn::S_PARAMS][am::strings::connection_key] = 65547;
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::app_name] = "SyncProxyTester2";
  (*so_reg)[jsn::S_MSG_PARAMS][am::strings::vr_synonyms][0] =
  "VR SyncProxyTester2";

  sh_app_first = app_manager->application(connect_key::SECOND_CONNECTION_KEY);
  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::SECOND_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
  }

  AddDevice(device_id::SECOND_DEVICE_ID);

  sh_app_first = app_manager->RegisterApplication(so_reg);
  sleep(3);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  sh_app_second = app_manager->application(connect_key::SECOND_CONNECTION_KEY);

  am::mobile_api::HMILevel::eType hmi_limit =
  am::mobile_api::HMILevel::HMI_LIMITED;
  am::mobile_api::HMILevel::eType hmi_full = am::mobile_api::HMILevel::HMI_FULL;
  am::mobile_api::HMILevel::eType return_put;

  /****************************************************************************/
  /**
   * is_new_app_media - true
   * curr_app->is_media_application() - true
   * curr_app->hmi_level() -mobile_api::HMILevel::HMI_FULL
   */
  (*sh_app_first).set_is_media_application(true);
  (*sh_app_first).set_hmi_level(hmi_full);

  (*sh_app_second).set_is_media_application(true);
  return_put = app_manager->PutApplicationInFull(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_BACKGROUND);

  /****************************************************************************/
  /**
   * is_new_app_media - true
   * curr_app->is_media_application() - false
   * curr_app->hmi_level() -mobile_api::HMILevel::HMI_FULL
   */
  (*sh_app_first).set_is_media_application(false);

  return_put = app_manager->PutApplicationInFull(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_LIMITED);

  /****************************************************************************/
  /**
   * is_new_app_media - true
   * curr_app->is_media_application() - false
   * curr_app->hmi_level() -mobile_api::HMILevel::HMI_LIMITED
   */
  (*sh_app_first).set_hmi_level(hmi_limit);

  return_put = app_manager->PutApplicationInFull(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_BACKGROUND);

  /****************************************************************************/
  /**
   * is_new_app_media - false
   * curr_app->hmi_level() -mobile_api::HMILevel::HMI_FULL
   */
  (*sh_app_second).set_is_media_application(false);

  (*sh_app_first).set_hmi_level(hmi_full);

  return_put = app_manager->PutApplicationInFull(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_BACKGROUND);

  /****************************************************************************/
  /**
   * is_new_app_media - false
   * curr_app->hmi_level() -mobile_api::HMILevel::HMI_LIMITED
   */
  (*sh_app_first).set_hmi_level(hmi_limit);

  return_put = app_manager->PutApplicationInFull(sh_app_second);

  EXPECT_EQ(return_put, am::mobile_api::HMILevel::HMI_FULL);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);
  app_manager->UnregisterApplication(65547, mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  RemovedDevice(device_id::SECOND_DEVICE_ID);
  sleep(2);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, OnHMIStartedCooperation) {
  /****************************************************************************/
  /**
   * Init params
   */
  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  /****************************************************************************/

  app_manager->OnHMIStartedCooperation();
  sleep(3);
  EXPECT_EQ(array_message->size(), 6);

  /****************************************************************************/
  /**
   * Finish test
   */

  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, begin_end_audio_pass_thru) {
  /**
   * Init params
   */

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  /****************************************************************************/

  EXPECT_TRUE(app_manager->begin_audio_pass_thru());

  EXPECT_FALSE(app_manager->begin_audio_pass_thru());

  EXPECT_TRUE(app_manager->end_audio_pass_thru());

  EXPECT_FALSE(app_manager->end_audio_pass_thru());
}

TEST(TestApplicationManager, SendAudioPassThroughNotification) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  /****************************************************************************/

  uint32_t session_key = connect_key::FIRST_CONNECTION_KEY;
  std::vector<uint8_t> binaryData;
  app_manager->SendAudioPassThroughNotification(session_key, binaryData);

  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, GetDeviceName) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  AddDevice(device_id::FIRST_DEVICE_ID);

  /****************************************************************************/
  /**
   * nonexistent device
   */
  std::string str = app_manager->GetDeviceName(100500);

  EXPECT_TRUE(str.empty());

  /****************************************************************************/
  /**
   * existent device
   */
  str = app_manager->GetDeviceName(device_id::FIRST_DEVICE_ID);

  EXPECT_FALSE(str.empty());

  /****************************************************************************/
  /**
   * Finish test
   */
  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, OnMobileMessageSent) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();
  protocol_handler::RawMessagePtr raw_message;

  app_manager->OnMobileMessageSent(raw_message);
}

TEST(TestApplicationManager, OnErrorSending) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();
  hmi_message_handler::MessageSharedPointer hmi_message;

  app_manager->OnErrorSending(hmi_message);
}

TEST(TestApplicationManager, OnDeviceListUpdated) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);
  //sleep(1);
  array_message = hmi_message_handler_interceptor->GetMasMessage();

  connection_handler::DeviceList device_list;

  transport_manager::DeviceHandle device_handler_test = 10;
  std::string mac_address_test = "255.255.255.0";
  std::string name_test = "test_DeviceInfo";

  transport_manager::ConnectionUID connection_id_test = 10;

  transport_manager::DeviceInfo device_info(
      device_handler_test,
      mac_address_test,
      name_test);

  device_list.insert(connection_handler::DeviceList::value_type(
      device_info.device_handle(),
      connection_handler::Device(
          device_info.device_handle(),
          device_info.name(),
          device_info.mac_address())));
  sleep(1);

  /****************************************************************************/

  app_manager->OnDeviceListUpdated(device_list);
  sleep(2);

  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * Finish test
   */
  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, RemoveDevice) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  app_manager->RemoveDevice(0);
}

TEST(TestApplicationManager, IsAudioStreamingAllowed) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  am::mobile_api::HMILevel::eType hmi_limit =
      am::mobile_api::HMILevel::HMI_LIMITED;

  am::mobile_api::HMILevel::eType hmi_full = am::mobile_api::HMILevel::HMI_FULL;
  am::mobile_api::HMILevel::eType hmi_background =
      am::mobile_api::HMILevel::HMI_BACKGROUND;

  /****************************************************************************/
  /**
   * hmi_level - HMI_FULL
   */
  (*sh_app_first).set_hmi_level(hmi_full);

  EXPECT_TRUE(app_manager->IsAudioStreamingAllowed(
      connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * hmi_level - HMI_LIMITED
   */
  (*sh_app_first).set_hmi_level(hmi_limit);

  EXPECT_TRUE(app_manager->IsAudioStreamingAllowed(
      connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * hmi_level - HMI_BACKGROUND
   */
  (*sh_app_first).set_hmi_level(hmi_background);

  EXPECT_FALSE(app_manager->IsAudioStreamingAllowed(
      connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, IsVideoStreamingAllowed) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  am::mobile_api::HMILevel::eType hmi_limit =
      am::mobile_api::HMILevel::HMI_LIMITED;

  am::mobile_api::HMILevel::eType hmi_full = am::mobile_api::HMILevel::HMI_FULL;
  am::mobile_api::HMILevel::eType hmi_background =
      am::mobile_api::HMILevel::HMI_BACKGROUND;

  /****************************************************************************/
  /**
   * hmi_level - HMI_FULL
   * hmi_supports_navi_streaming - true
   */
  (*sh_app_first).set_hmi_level(hmi_full);
  (*sh_app_first).set_hmi_supports_navi_streaming(true);

  EXPECT_TRUE(app_manager->IsVideoStreamingAllowed(
      connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * hmi_level - HMI_LIMITED
   */
  (*sh_app_first).set_hmi_level(hmi_limit);

  EXPECT_FALSE(app_manager->IsVideoStreamingAllowed(
      connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, GenerateGrammarID) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  EXPECT_TRUE((app_manager->GenerateGrammarID() >= 0));
}

TEST(TestApplicationManager, OnServiceStartedCallback) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  connection_handler::DeviceHandle device_handle = 11;

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }

  /****************************************************************************/
  /**
   * protocol_handler::ServiceType = kRpc
   */
  EXPECT_TRUE(app_manager->OnServiceStartedCallback(
      device_handle,
      connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kRpc));

  /****************************************************************************/
  /**
   * protocol_handler::ServiceType = kMobileNav
   * set_allowed_support_navigation - true
   */
  (*sh_app_first).set_allowed_support_navigation(true);

  EXPECT_TRUE(app_manager->OnServiceStartedCallback(
      device_handle,
      connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kMobileNav));

  /****************************************************************************/
  /**
   * protocol_handler::ServiceType = kMobileNav
   * set_allowed_support_navigation - false
   */
  (*sh_app_first).set_allowed_support_navigation(false);

  EXPECT_FALSE(app_manager->OnServiceStartedCallback(
      device_handle, connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kMobileNav));

  /****************************************************************************/
  /**
   * protocol_handler::ServiceType = kAudio
   * set_allowed_support_navigation - true
   */
  (*sh_app_first).set_allowed_support_navigation(true);

  EXPECT_TRUE(app_manager->OnServiceStartedCallback(
      device_handle, connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kAudio));

  /****************************************************************************/
  /**
   * protocol_handler::ServiceType = kAudio
   * set_allowed_support_navigation - false
   */
  (*sh_app_first).set_allowed_support_navigation(false);

  EXPECT_FALSE(app_manager->OnServiceStartedCallback(
      device_handle,
      connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kAudio));

  /****************************************************************************/
  /**
   * type kInvalidServiceType
   *
   */
  (*sh_app_first).set_allowed_support_navigation(false);

  EXPECT_TRUE(app_manager->OnServiceStartedCallback(
      device_handle, connect_key::FIRST_CONNECTION_KEY,
      protocol_handler::kInvalidServiceType));

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, OnServiceEndedCallback) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  array_message->clear();

  /****************************************************************************/
  /**
   * protocol_handler::kRpc
   */
  app_manager->OnServiceEndedCallback(connect_key::FIRST_CONNECTION_KEY,
                                      protocol_handler::kRpc);
  sleep(1);
  EXPECT_FALSE(app_manager->application(connect_key::FIRST_CONNECTION_KEY));

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, SendMessageToMobile) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  std::list<protocol_handler::RawMessagePtr>* array_message;

  transport_manager::TransportManager* transport_manager_;
  transport_manager_ = transport_manager::TransportManagerDefault::instance();

  protocol_handler::ProtocolHandlerImpl* protocol_handler_ =
      new protocol_handler::ProtocolHandlerImpl(transport_manager_);

  ProtocolHandlerInterceptor* protocol_handler_interceptor =
      new ProtocolHandlerInterceptor(transport_manager_);

  app_manager->set_protocol_handler(protocol_handler_interceptor);

  array_message = protocol_handler_interceptor->GetMasRawMessage();

  utils::SharedPtr<smart::SmartObject> so_message(new smart::SmartObject);

  (*so_message)[am::strings::params][am::strings::function_id] =
      (am::mobile_api::FunctionID::OnHMIStatusID);

  (*so_message)[am::strings::params][am::strings::message_type] =
       mobile_apis::messageType::request;

  (*so_message)[jsn::S_PARAMS][am::strings::protocol_version] =
      am::commands::CommandImpl::protocol_version_;

  (*so_message)[jsn::S_PARAMS][am::strings::protocol_type] =
      am::commands::CommandImpl::hmi_protocol_type_;

  (*so_message)[am::strings::params][am::strings::connection_key] = 100500;

  (*so_message)[am::strings::msg_params][am::strings::hmi_level] =
      mobile_apis::HMILevel::HMI_BACKGROUND;

  (*so_message)[am::strings::msg_params][am::strings::audio_streaming_state] =
      1;

  (*so_message)[am::strings::msg_params][am::strings::system_context] = 1;

  /****************************************************************************/
  /**
   *protocol_handler_ - NULL;
   */

  app_manager->set_protocol_handler(NULL);

  app_manager->SendMessageToMobile(so_message);
  sleep(1);
  EXPECT_EQ(array_message->size(), 0);

  /****************************************************************************/
  /**
   * Can't send msg to Mobile: failed to create string
   */

  app_manager->set_protocol_handler(protocol_handler_interceptor);

  app_manager->SendMessageToMobile(so_message);
  sleep(1);
  EXPECT_EQ(array_message->size(), 0);

  /****************************************************************************/
  /**
   * No application associated with connection key
   */

  (*so_message)[jsn::S_PARAMS][am::strings::protocol_version] =
      am::kV2;
  (*so_message)[jsn::S_PARAMS][am::strings::protocol_type] =
     0;

  (*so_message)[am::strings::params][am::strings::message_type] =
      application_manager::MessageType::kNotification;

  app_manager->SendMessageToMobile(so_message);
  sleep(1);
  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->set_protocol_handler(protocol_handler_);

  delete protocol_handler_interceptor;
}

TEST(TestApplicationManager, ManageMobileCommand) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  utils::SharedPtr<smart::SmartObject> so_manage(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  /****************************************************************************/
  /**
   * APPLICATION_NOT_REGISTERED
   */

  (*so_reg)[jsn::S_PARAMS][am::strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_UpdateDeviceList;

  (*so_reg)[am::strings::params][am::strings::protocol_type] =
      am::commands::CommandImpl::mobile_protocol_type_;

  EXPECT_FALSE(app_manager->ManageMobileCommand(so_reg));
}

TEST(TestApplicationManager, SendMessageToHMI) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor
  = new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  array_message->clear();

  /****************************************************************************/
  /**
   * Send message to HMI
   */
  utils::SharedPtr<smart::SmartObject> so_to_hmi(new smart::SmartObject);
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::function_id] =
      hmi_apis::FunctionID::UI_Show;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::message_type] =
      mobile_apis::messageType::request;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::protocol_version] =
      am::commands::CommandImpl::protocol_version_;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::protocol_type] =
      am::commands::CommandImpl::hmi_protocol_type_;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::correlation_id] =
      app_manager->GetNextHMICorrelationID();

  app_manager->SendMessageToHMI(so_to_hmi);
  sleep(1);

  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, ManageHMICommand) {
  /****************************************************************************/
  /**
   * Init params
   */
  utils::SharedPtr<smart::SmartObject> so_reg(new smart::SmartObject);
  am::ApplicationSharedPtr sh_app_first;

  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  std::list<hmi_message_handler::MessageSharedPointer>* array_message;

  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler =
      hmi_message_handler::HMIMessageHandlerImpl::instance();

  HMIMessageHandlerInterceptor* hmi_message_handler_interceptor =
      new HMIMessageHandlerInterceptor;

  app_manager->set_hmi_message_handler(hmi_message_handler_interceptor);

  array_message = hmi_message_handler_interceptor->GetMasMessage();

  RegistrSO(so_reg);

  sh_app_first = app_manager->application(connect_key::FIRST_CONNECTION_KEY);

  if (sh_app_first.valid()) {
    app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                       mobile_apis::Result::SUCCESS);
    sleep(2);
  }

  sh_app_first = app_manager->RegisterApplication(so_reg);

  if (!sh_app_first.valid()) {
    AddDevice(device_id::FIRST_DEVICE_ID);
    sleep(1);
    sh_app_first = app_manager->RegisterApplication(so_reg);
    ASSERT_TRUE(sh_app_first.valid());
  }
  array_message->clear();

  /****************************************************************************/
  /**
   * Send message to HMI
   */
  utils::SharedPtr<smart::SmartObject> so_to_hmi(new smart::SmartObject);

  (*so_to_hmi)[jsn::S_PARAMS][am::strings::function_id] =
      hmi_apis::FunctionID::UI_Show;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::message_type] =
      mobile_apis::messageType::request;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::protocol_version] =
      am::commands::CommandImpl::protocol_version_;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::protocol_type] =
      am::commands::CommandImpl::hmi_protocol_type_;
  (*so_to_hmi)[jsn::S_PARAMS][am::strings::correlation_id] = app_manager
      ->GetNextHMICorrelationID();

  app_manager->SendMessageToHMI(so_to_hmi);
  sleep(1);

  EXPECT_EQ(array_message->size(), 1);

  /****************************************************************************/
  /**
   * Finish test
   */
  app_manager->UnregisterApplication(connect_key::FIRST_CONNECTION_KEY,
                                     mobile_apis::Result::SUCCESS);
  sleep(1);

  RemovedDevice(device_id::FIRST_DEVICE_ID);
  sleep(1);
  app_manager->set_hmi_message_handler(hmi_handler);

  delete hmi_message_handler_interceptor;
}

TEST(TestApplicationManager, set_application_id) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();
  bool bool_value = true;

  /****************************************************************************/
  /**
   * application_id - true
   */
  int32_t correlation_id = 5;
  int32_t app_id = 6;

  app_manager->set_application_id(correlation_id, app_id);

  bool_value = (app_manager->application_id(correlation_id) == app_id);
  EXPECT_TRUE(bool_value);

  /****************************************************************************/
  /**
   * application_id - false
   */

  bool_value = (app_manager->application_id(correlation_id) == app_id);
  EXPECT_FALSE(bool_value);
}

TEST(TestApplicationManager, SaveBinary) {
  /****************************************************************************/
  /**
   * Init params
   */
  am::ApplicationManagerImpl* app_manager =
      am::ApplicationManagerImpl::instance();

  std::vector<uint8_t> binary_data;
  std::string file_path = "";
  std::string file_name = "";
  uint32_t offset = 0;

  /****************************************************************************/
  /**
   * offset != 0
   * file_size != offset
   */
  offset = 1;

  EXPECT_EQ(app_manager->SaveBinary(binary_data, file_path, file_name, offset),
            mobile_apis::Result::INVALID_DATA);

  /****************************************************************************/
  /**
   * offset = 0
   */
  file_path = "./AppManagerImplTest/";
  file_name = "temp.jpg";
  offset = 0;
  binary_data.clear();

  file_system::ReadBinaryFile("./AppManagerImplTest/icon.png", binary_data);

  EXPECT_EQ(app_manager->SaveBinary(binary_data, file_path, file_name, offset),
            mobile_apis::Result::SUCCESS);

}

}  // namespace test

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_TEST_APP_MANAGER_H_
