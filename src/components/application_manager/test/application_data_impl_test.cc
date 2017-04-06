
/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <stdint.h>
#include <string>
#include <algorithm>

#include "gtest/gtest.h"
#include "application_manager/application_data_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "utils/make_shared.h"
#include "utils/data_accessor.h"
#include "application_manager/hmi_state.h"

namespace test {
namespace components {
namespace application_manager_test {
namespace application_data_impl_test {

using namespace application_manager;  // NOLINT(build/namespaces)
namespace custom_str = utils::custom_string;
namespace strings = application_manager::strings;
namespace HMILevel = mobile_apis::HMILevel;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using usage_statistics_test::MockStatisticsManager;

namespace {
const std::string kMenuName = "menu_name";
const uint32_t kCmdId = 10u;
}  // namespace

class ApplicationDataImplTestBase : public ::testing::Test {
 public:
  ApplicationDataImplTestBase()
      : mock_application_manager_settings_()
      , mock_application_manager_()
      , app_id_(kCmdId)
      , policy_app_id_("policy_app_id")
      , mac_address_("mac_address")
      , app_name_("app_name")
      , state_id_(HmiState::STATE_ID_REGULAR)
      , test_hmi_state_(CreateTestHmiState())
      , test_lvl_(HMILevel::INVALID_ENUM)
      , audiostate_(mobile_apis::AudioStreamingState::NOT_AUDIBLE)
      , syst_context_(mobile_apis::SystemContext::SYSCTXT_MAIN)
      , dummy_object_(smart_objects::SmartType_Map)
      , second_dummy_object_(smart_objects::SmartType_Map) {}

 protected:
  void SetUp() OVERRIDE {
    EXPECT_CALL(mock_application_manager_, CreateRegularState(app_id_, _, _, _))
        .WillOnce(Return(test_hmi_state_));
    EXPECT_CALL(mock_application_manager_, get_settings())
        .WillRepeatedly(ReturnRef(mock_application_manager_settings_));
    EXPECT_CALL(mock_application_manager_settings_, app_icons_folder())
        .WillRepeatedly(ReturnRef(directory_name_));
    EXPECT_CALL(mock_application_manager_settings_, app_storage_folder())
        .WillRepeatedly(ReturnRef(directory_name_));
    EXPECT_CALL(mock_application_manager_settings_,
                audio_data_stopped_timeout()).WillOnce(Return(0));
    EXPECT_CALL(mock_application_manager_settings_,
                video_data_stopped_timeout()).WillOnce(Return(0));
    app_impl_ = new ApplicationImpl(app_id_,
                                    policy_app_id_,
                                    mac_address_,
                                    app_name_,
                                    utils::MakeShared<MockStatisticsManager>(),
                                    mock_application_manager_);
  }

  HmiStatePtr CreateTestHmiState() {
    HmiStatePtr testState = utils::MakeShared<HmiState>(
        app_id_, mock_application_manager_, state_id_);
    testState->set_hmi_level(test_lvl_);
    testState->set_audio_streaming_state(audiostate_);
    testState->set_system_context(syst_context_);
    return testState;
  }

  template <class T>
  void CheckSetterAndGetter(
      void (T::*setter)(const smart_objects::SmartObject&),
      const smart_objects::SmartObject* (T::*getter)() const) {
    // Set and get correctly
    ((*app_impl_).*setter)(dummy_object_);
    EXPECT_EQ(dummy_object_, *((*app_impl_).*getter)());
    // Check reseting
    ((*app_impl_).*setter)(second_dummy_object_);
    EXPECT_EQ(second_dummy_object_, *((*app_impl_).*getter)());
  }

  MockApplicationManagerSettings mock_application_manager_settings_;
  MockApplicationManager mock_application_manager_;
  uint32_t app_id_;
  std::string policy_app_id_;
  std::string mac_address_;
  custom_str::CustomString app_name_;
  const std::string directory_name_ = "./test_storage";
  HmiState::StateID state_id_;
  HmiStatePtr test_hmi_state_;
  HMILevel::eType test_lvl_;
  mobile_apis::AudioStreamingState::eType audiostate_;
  mobile_apis::SystemContext::eType syst_context_;
  utils::SharedPtr<ApplicationImpl> app_impl_;
  utils::SharedPtr<MockStatisticsManager> mock_statistics_manager_;
  smart_objects::SmartObject dummy_object_;
  smart_objects::SmartObject second_dummy_object_;
};

class InitialApplicationDataImplTest : public ApplicationDataImplTestBase {};

TEST_F(InitialApplicationDataImplTest, SetAndGetAppHMITypes_Success) {
  CheckSetterAndGetter<InitialApplicationDataImpl>(
      &InitialApplicationDataImpl::set_app_types,
      &InitialApplicationDataImpl::app_types);
}

TEST_F(InitialApplicationDataImplTest, SetAndGetVRSynonyms_Success) {
  CheckSetterAndGetter<InitialApplicationDataImpl>(
      &InitialApplicationDataImpl::set_vr_synonyms,
      &InitialApplicationDataImpl::vr_synonyms);
}

TEST_F(InitialApplicationDataImplTest, SetAndGetTTSName_Success) {
  CheckSetterAndGetter<InitialApplicationDataImpl>(
      &InitialApplicationDataImpl::set_tts_name,
      &InitialApplicationDataImpl::tts_name);
}

TEST_F(InitialApplicationDataImplTest, SetAndGetNGNName_Success) {
  CheckSetterAndGetter<InitialApplicationDataImpl>(
      &InitialApplicationDataImpl::set_ngn_media_screen_name,
      &InitialApplicationDataImpl::ngn_media_screen_name);
}

TEST_F(InitialApplicationDataImplTest, SetAndGetLanguage_Success) {
  const mobile_apis::Language::eType language = mobile_apis::Language::EN_GB;
  app_impl_->set_language(language);
  EXPECT_EQ(language, app_impl_->language());
}

TEST_F(InitialApplicationDataImplTest, SetAndGetUILanguage_Success) {
  const mobile_apis::Language::eType ui_language = mobile_apis::Language::EN_GB;
  app_impl_->set_ui_language(ui_language);
  EXPECT_EQ(ui_language, app_impl_->ui_language());
}

class DynamicApplicationDataImplTest : public ApplicationDataImplTestBase {
 protected:
  void LoadGlobalPropertiesSetupHelper(
      const smart_objects::SmartObject& value) {
    smart_objects::SmartObject properties_so(smart_objects::SmartType_Map);
    properties_so[strings::vr_help_title] = value;
    properties_so[strings::vr_help] = value;
    properties_so[strings::timeout_prompt] = value;
    properties_so[strings::help_prompt] = value;
    properties_so[strings::keyboard_properties] = value;
    properties_so[strings::menu_title] = value;
    properties_so[strings::menu_icon] = value;
    app_impl_->load_global_properties(properties_so);
  }

  uint32_t AddItemWithCheck(
      void (DynamicApplicationDataImpl::*add)(
          const uint32_t, const smart_objects::SmartObject&),
      smart_objects::SmartObject* (DynamicApplicationDataImpl::*find)(
          const uint32_t)) {
    const uint32_t internal_consecutive_number = 1u;
    dummy_object_[strings::cmd_id] = kCmdId;
    // Add and find it correctly
    ((*app_impl_).*add)(internal_consecutive_number, dummy_object_);
    EXPECT_EQ(dummy_object_, *(((*app_impl_).*find)(kCmdId)));

    return kCmdId;
  }

  uint32_t AddChoiceSetItemWithCheck(
      void (DynamicApplicationDataImpl::*add)(
          const uint32_t, const smart_objects::SmartObject&),
      smart_objects::SmartObject* (DynamicApplicationDataImpl::*find)(
          const uint32_t)) {
    const uint32_t id = 1u;
    // Add and find it correctly
    ((*app_impl_).*add)(id, dummy_object_);
    EXPECT_EQ(dummy_object_, *(((*app_impl_).*find)(id)));

    return id;
  }

  uint32_t AddSubMenuHelper(const std::string& menu_name) {
    smart_objects::SmartObject menu(smart_objects::SmartType_Map);
    menu[strings::menu_name] = menu_name;
    const uint32_t sub_menu_id = 1u;
    app_impl_->AddSubMenu(sub_menu_id, menu);
    EXPECT_EQ(menu, *(app_impl_->FindSubMenu(sub_menu_id)));

    return sub_menu_id;
  }

  void AddPerformInteractionChoiceSetSetupHelper(const uint32_t correlation_id,
                                                 const uint32_t choice_set_id) {
    app_impl_->AddPerformInteractionChoiceSet(
        correlation_id, choice_set_id, dummy_object_);
    DataAccessor<PerformChoiceSetMap>
        performinteraction_choice_set_map_accessor =
            app_impl_->performinteraction_choice_set_map();
    PerformChoiceSetMap performinteraction_choice_set_map =
        performinteraction_choice_set_map_accessor.GetData();
    PerformChoice::const_iterator it =
        performinteraction_choice_set_map[correlation_id].find(choice_set_id);
    EXPECT_EQ(dummy_object_, *(it->second));
  }
};

TEST_F(DynamicApplicationDataImplTest, SetAndGetHelpPrompt_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_help_prompt,
      &DynamicApplicationDataImpl::help_prompt);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetTimeoutPrompt_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_timeout_prompt,
      &DynamicApplicationDataImpl::timeout_prompt);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetVRHelpTitle_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_vr_help_title,
      &DynamicApplicationDataImpl::vr_help_title);
}

TEST_F(DynamicApplicationDataImplTest, ResetVRHelpTitle_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_vr_help_title,
      &DynamicApplicationDataImpl::vr_help_title);
  app_impl_->reset_vr_help_title();
  EXPECT_EQ(NULL, app_impl_->vr_help_title());
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetVRHelp_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_vr_help,
      &DynamicApplicationDataImpl::vr_help);
}

TEST_F(DynamicApplicationDataImplTest, ResetVRHelp_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_vr_help,
      &DynamicApplicationDataImpl::vr_help);
  app_impl_->reset_vr_help();
  EXPECT_EQ(NULL, app_impl_->vr_help_title());
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetTBTState_Success) {
  const mobile_apis::TBTState::eType tbt_state_value =
      mobile_apis::TBTState::ETA_REQUEST;
  app_impl_->set_tbt_state(tbt_state_value);
  EXPECT_EQ(tbt_state_value, app_impl_->tbt_state());
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetShowCommand_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_show_command,
      &DynamicApplicationDataImpl::show_command);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetTBTShowCommand_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_tbt_show_command,
      &DynamicApplicationDataImpl::tbt_show_command);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetKeyboardProps_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_keyboard_props,
      &DynamicApplicationDataImpl::keyboard_props);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetMenuTitle_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_menu_title,
      &DynamicApplicationDataImpl::menu_title);
}

TEST_F(DynamicApplicationDataImplTest, SetAndGetMenuIcon_Success) {
  CheckSetterAndGetter<DynamicApplicationDataImpl>(
      &DynamicApplicationDataImpl::set_menu_icon,
      &DynamicApplicationDataImpl::menu_icon);
}

TEST_F(DynamicApplicationDataImplTest,
       LoadGlobalProperties_ValidObjects_Success) {
  LoadGlobalPropertiesSetupHelper(dummy_object_);
  EXPECT_EQ(dummy_object_, *(app_impl_->vr_help_title()));
  EXPECT_EQ(dummy_object_, *(app_impl_->vr_help()));
  EXPECT_EQ(dummy_object_, *(app_impl_->timeout_prompt()));
  EXPECT_EQ(dummy_object_, *(app_impl_->help_prompt()));
  EXPECT_EQ(dummy_object_, *(app_impl_->keyboard_props()));
  EXPECT_EQ(dummy_object_, *(app_impl_->menu_title()));
  EXPECT_EQ(dummy_object_, *(app_impl_->menu_icon()));
}

TEST_F(DynamicApplicationDataImplTest,
       LoadGlobalProperties_NullObjects_Success) {
  smart_objects::SmartObject null_so(smart_objects::SmartType_Null);
  LoadGlobalPropertiesSetupHelper(null_so);
  EXPECT_EQ(NULL, app_impl_->vr_help_title());
  EXPECT_EQ(NULL, app_impl_->vr_help());
  EXPECT_EQ(NULL, app_impl_->timeout_prompt());
  EXPECT_EQ(NULL, app_impl_->help_prompt());
  EXPECT_EQ(NULL, app_impl_->keyboard_props());
  EXPECT_EQ(NULL, app_impl_->menu_title());
  EXPECT_EQ(NULL, app_impl_->menu_icon());
}

TEST_F(DynamicApplicationDataImplTest, AddSubMenu_Success) {
  AddSubMenuHelper(kMenuName);
}

TEST_F(DynamicApplicationDataImplTest, RemoveSubMenu_Success) {
  const uint32_t sub_menu_id = AddSubMenuHelper(kMenuName);
  app_impl_->RemoveSubMenu(sub_menu_id);
  EXPECT_EQ(NULL, app_impl_->FindSubMenu(sub_menu_id));
}

TEST_F(DynamicApplicationDataImplTest, IsSubMenuNameAlreadyExist_Found) {
  AddSubMenuHelper(kMenuName);
  EXPECT_TRUE(app_impl_->IsSubMenuNameAlreadyExist(kMenuName));
}

TEST_F(DynamicApplicationDataImplTest, IsSubMenuNameAlreadyExist_NotFound) {
  AddSubMenuHelper(kMenuName);
  const std::string other_menu_name("other_name");
  EXPECT_FALSE(app_impl_->IsSubMenuNameAlreadyExist(other_menu_name));
}

TEST_F(DynamicApplicationDataImplTest, AddChoiceSet_Success) {
  AddChoiceSetItemWithCheck(&DynamicApplicationDataImpl::AddChoiceSet,
                            &DynamicApplicationDataImpl::FindChoiceSet);
}

TEST_F(DynamicApplicationDataImplTest, RemoveChoiceSet_Success) {
  const uint32_t choice_set_id =
      AddChoiceSetItemWithCheck(&DynamicApplicationDataImpl::AddChoiceSet,
                                &DynamicApplicationDataImpl::FindChoiceSet);
  app_impl_->RemoveChoiceSet(choice_set_id);
  EXPECT_EQ(NULL, app_impl_->FindChoiceSet(choice_set_id));
}

TEST_F(DynamicApplicationDataImplTest, AddPerformInteractionChoiceSet_Success) {
  const uint32_t correlation_id = 1u;
  const uint32_t choice_set_id = 2u;
  AddPerformInteractionChoiceSetSetupHelper(correlation_id, choice_set_id);
}

TEST_F(DynamicApplicationDataImplTest,
       DeletePerformInteractionChoiceSet_Success) {
  const uint32_t correlation_id = 1u;
  const uint32_t choice_set_id = 2u;
  AddPerformInteractionChoiceSetSetupHelper(correlation_id, choice_set_id);
  app_impl_->DeletePerformInteractionChoiceSet(correlation_id);
  DataAccessor<PerformChoiceSetMap> performinteraction_choice_set_map_accessor =
      app_impl_->performinteraction_choice_set_map();
  PerformChoiceSetMap performinteraction_choice_set_map =
      performinteraction_choice_set_map_accessor.GetData();
  PerformChoiceSetMap::iterator it =
      performinteraction_choice_set_map.find(correlation_id);
  EXPECT_EQ(performinteraction_choice_set_map.end(), it);
}

TEST_F(DynamicApplicationDataImplTest, SetPerformInteractionActive_Success) {
  const uint32_t active = 1u;
  app_impl_->set_perform_interaction_active(active);
  EXPECT_EQ(active, app_impl_->is_perform_interaction_active());
}

TEST_F(DynamicApplicationDataImplTest, SetPerformInteractionLayout_Success) {
  const mobile_apis::LayoutMode::eType layout =
      mobile_apis::LayoutMode::INVALID_ENUM;
  app_impl_->set_perform_interaction_layout(layout);
  EXPECT_EQ(layout, app_impl_->perform_interaction_layout());
}

TEST_F(DynamicApplicationDataImplTest, SetPerformInteractionMode_Success) {
  const int32_t mode = 1;
  app_impl_->set_perform_interaction_mode(mode);
  EXPECT_EQ(mode, app_impl_->perform_interaction_mode());
}

}  // namespace application_data_impl_test
}  // namespace application_manager_test
}  // namespace components
}  // namespace test
