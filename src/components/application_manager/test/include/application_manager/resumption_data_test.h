/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_RESUMPTION_DATA_TEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_RESUMPTION_DATA_TEST_H_

#include <algorithm>
#include <string>
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/mock_app_extension.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/state_controller.h"
#include "application_manager/usage_statistics.h"
#include "config_profile/profile.h"
#include "gtest/gtest.h"
#include "utils/data_accessor.h"

namespace test {
namespace components {
namespace resumption_test {

using ::testing::NiceMock;
namespace am = application_manager;
namespace sm = smart_objects;
using namespace Json;

using namespace resumption;
using namespace mobile_apis;

class ResumptionDataTest : public ::testing::Test {
 protected:
  ResumptionDataTest()
      : kCountOfCommands_(5u)
      , kCountOfChoice_(2u)
      , kCountOfChoiceSets_(4u)
      , kCountOfSubmenues_(3u)
      , kCountOfFiles_(8u)
      , kCountOfVrhelptitle_(2u)
      , kMacAddress_("12345")
      , sublock_ptr_(std::make_shared<sync_primitives::Lock>())
      , comlock_ptr_(std::make_shared<sync_primitives::Lock>())
      , setlock_ptr_(std::make_shared<sync_primitives::Lock>())
      , btnlock_ptr_(std::make_shared<sync_primitives::Lock>())
      , ivilock_ptr_(std::make_shared<sync_primitives::Lock>())
      , extensions_lock_(std::make_shared<sync_primitives::RecursiveLock>())
      , window_params_map_lock_ptr_(std::make_shared<sync_primitives::Lock>()) {
  }
  virtual ~ResumptionDataTest();
  // Check structure in saved application
  void CheckSavedApp(sm::SmartObject& saved_data);
  // Set data for resumption
  virtual void PrepareData();
  std::shared_ptr<NiceMock<application_manager_test::MockApplication> >
      app_mock;

  profile::Profile profile_;
  HMILevel::eType hmi_level_;
  size_t app_id_;
  size_t hmi_app_id_;
  std::string policy_app_id_;
  size_t ign_off_count_;
  const size_t tts_chunks_count = 4;

  size_t grammar_id_;
  std::string hash_;
  bool is_audio_;
  const connection_handler::DeviceHandle device_handle_ = 10;

  sm::SmartObjectSPtr help_prompt_;
  sm::SmartObjectSPtr timeout_prompt_;
  sm::SmartObjectSPtr vr_help_;
  sm::SmartObjectSPtr vr_help_title_;
  sm::SmartObjectSPtr vr_synonyms_;
  sm::SmartObjectSPtr keyboard_props_;
  sm::SmartObjectSPtr menu_title_;
  sm::SmartObjectSPtr menu_icon_;
  sm::SmartObjectSPtr menu_layout_;

  void SetCommands();
  void SetSubmenues();
  void SetChoiceSet();
  void SetAppFiles();
  void SetGlobalProporties();
  void SetKeyboardProperties();
  void SetMenuParams();
  void SetHelpAndTimeoutPrompt();
  void SetVRHelpTitle();
  void SetSubscriptions();
  void SetWindowsInfo();
  void SetDefaultCurrentHmiState();
  void SetDefaultWindowIds();

  void CheckCommands(sm::SmartObject& res_list);
  void CheckGlobalProporties(sm::SmartObject& res_list);
  void CheckSubmenues(sm::SmartObject& res_list);
  void CheckChoiceSet(sm::SmartObject& res_list);
  void CheckAppFiles(sm::SmartObject& res_list);
  void CheckKeyboardProperties(sm::SmartObject& res_list);
  void CheckMenuTitle(sm::SmartObject& res_list);
  void CheckMenuIcon(sm::SmartObject& res_list);
  void CheckMenuLayout(sm::SmartObject& res_list);
  void CheckHelpPrompt(sm::SmartObject& res_list);
  void CheckTimeoutPrompt(sm::SmartObject& res_list);
  void CheckVRHelp(
      ns_smart_device_link::ns_smart_objects::SmartObject& res_list);
  void CheckVRTitle(sm::SmartObject& res_list);
  void CheckSubscriptions(sm::SmartObject& res_list);
  void CheckWindowsInfo(sm::SmartObject& res_list);

  const size_t kCountOfCommands_;
  const size_t kCountOfChoice_;
  const size_t kCountOfChoiceSets_;
  const size_t kCountOfSubmenues_;
  const size_t kCountOfFiles_;
  const size_t kCountOfVrhelptitle_;
  const std::string kMacAddress_;

  am::CommandsMap test_commands_map;
  am::SubMenuMap test_submenu_map;
  am::ChoiceSetMap test_choiceset_map;
  am::AppFilesMap app_files_map_;
  smart_objects::SmartObject user_location_;

  am::ButtonSubscriptions btn_subscr;

  am::WindowParamsMap test_window_params_map_;

  std::shared_ptr<sync_primitives::Lock> sublock_ptr_;
  std::shared_ptr<sync_primitives::Lock> comlock_ptr_;
  std::shared_ptr<sync_primitives::Lock> setlock_ptr_;
  std::shared_ptr<sync_primitives::Lock> btnlock_ptr_;
  std::shared_ptr<sync_primitives::Lock> ivilock_ptr_;
  std::shared_ptr<sync_primitives::RecursiveLock> extensions_lock_;
  std::shared_ptr<sync_primitives::Lock> window_params_map_lock_ptr_;
  application_manager_test::MockApplicationManagerSettings
      mock_application_manager_settings_;
  NiceMock<application_manager_test::MockApplicationManager>
      mock_application_manager_;
  std::shared_ptr<NiceMock<application_manager_test::MockAppExtension> >
      mock_app_extension_;
  std::list<application_manager::AppExtensionPtr> extensions_;
};

}  // namespace resumption_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_RESUMPTION_DATA_TEST_H_
