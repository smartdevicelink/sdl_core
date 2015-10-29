/*
 * Copyright (c) 2015, Ford Motor Company
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

#include <gtest/gtest.h>
#include "application_manager/hmi_state.h"
#include "application_manager/state_controller.h"
#include "application_manager/usage_statistics.h"
#include "application_manager_mock.h"
#include "application_mock.h"
#include "statistics_manager_mock.h"
#include "utils/lock.h"
#include "utils/data_accessor.h"
#include "utils/make_shared.h"
#include "application_manager/message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/smart_object_keys.h"

namespace am = application_manager;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnPointee;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::InSequence;
using ::testing::Truly;

class MessageHelperMock {
 public:
  MOCK_METHOD3(SendActivateAppToHMI,
               uint32_t(uint32_t const app_id,
                        hmi_apis::Common_HMILevel::eType level,
                        bool send_policy_priority));
  MOCK_METHOD1(SendOnResumeAudioSourceToHMI, void(const uint32_t app_id));
};

static MessageHelperMock* message_helper_mock_;

uint32_t application_manager::MessageHelper::SendActivateAppToHMI(
    uint32_t const app_id, hmi_apis::Common_HMILevel::eType level,
    bool send_policy_priority) {
  return message_helper_mock_->SendActivateAppToHMI(app_id, level,
                                                    send_policy_priority);
}

void application_manager::MessageHelper::SendOnResumeAudioSourceToHMI(
    const uint32_t app_id) {
  message_helper_mock_->SendOnResumeAudioSourceToHMI(app_id);
}

namespace state_controller_test {

struct HmiStatesComparator {
  mobile_apis::HMILevel::eType hmi_level_;
  mobile_apis::AudioStreamingState::eType audio_streaming_state_;
  mobile_apis::SystemContext::eType system_context_;

  HmiStatesComparator(
      mobile_apis::HMILevel::eType hmi_level,
      mobile_apis::AudioStreamingState::eType audio_streaming_state,
      mobile_apis::SystemContext::eType system_context)
      : hmi_level_(hmi_level),
        audio_streaming_state_(audio_streaming_state),
        system_context_(system_context) {}

  HmiStatesComparator(am::HmiStatePtr state_ptr)
      : hmi_level_(state_ptr->hmi_level()),
        audio_streaming_state_(state_ptr->audio_streaming_state()),
        system_context_(state_ptr->system_context()) {}

  bool operator()(am::HmiStatePtr state_ptr) const {
    return state_ptr->hmi_level() == hmi_level_ &&
           state_ptr->audio_streaming_state() == audio_streaming_state_ &&
           state_ptr->system_context() == system_context_;
  }
};

struct HmiStatesIDComparator {
  am::HmiState::StateID state_id_;

  HmiStatesIDComparator(am::HmiState::StateID state_id)
      : state_id_(state_id) {}

  bool operator()(am::HmiStatePtr state_ptr) const {
    return state_ptr->state_id() == state_id_;
  }
};

#define MEDIA true
#define NOT_MEDIA false
#define VC true
#define NOT_VC false
#define NAVI true
#define NOT_NAVI false

class StateControllerTest : public ::testing::Test {
 public:
  StateControllerTest()
      : ::testing::Test(),
        usage_stat("0", utils::SharedPtr<us::StatisticsManager>(
                            new StatisticsManagerMock)),
        applications_(application_set_, applications_lock_),
        state_ctrl_(&app_manager_mock_) {}
  NiceMock<ApplicationManagerMock> app_manager_mock_;

  am::UsageStatistics usage_stat;

  am::ApplicationSet application_set_;
  mutable sync_primitives::Lock applications_lock_;
  DataAccessor<am::ApplicationSet> applications_;
  am::StateController state_ctrl_;

  am::ApplicationSharedPtr simple_app_;
  NiceMock<ApplicationMock>* simple_app_ptr_;
  uint32_t simple_app_id_ = 1721;

  am::ApplicationSharedPtr navi_app_;
  NiceMock<ApplicationMock>* navi_app_ptr_;
  uint32_t navi_app_id_ = 1762;

  am::ApplicationSharedPtr media_app_;
  NiceMock<ApplicationMock>* media_app_ptr_;
  uint32_t media_app_id_ = 1801;

  am::ApplicationSharedPtr vc_app_;
  NiceMock<ApplicationMock>* vc_app_ptr_;
  uint32_t vc_app_id_ = 1825;

  am::ApplicationSharedPtr media_navi_app_;
  NiceMock<ApplicationMock>* media_navi_app_ptr_;
  uint32_t media_navi_app_id_ = 1855;

  am::ApplicationSharedPtr media_vc_app_;
  NiceMock<ApplicationMock>* media_vc_app_ptr_;
  uint32_t media_vc_app_id_ = 1881;

  am::ApplicationSharedPtr navi_vc_app_;
  NiceMock<ApplicationMock>* navi_vc_app_ptr_;
  uint32_t navi_vc_app_id_ = 1894;

  am::ApplicationSharedPtr media_navi_vc_app_;
  NiceMock<ApplicationMock>* media_navi_vc_app_ptr_;
  uint32_t media_navi_vc_app_id_ = 1922;

  std::vector<am::HmiStatePtr> valid_states_for_audio_app_;
  std::vector<am::HmiStatePtr> valid_states_for_not_audio_app_;
  std::vector<am::HmiStatePtr> common_invalid_states_;
  std::vector<am::HmiStatePtr> invalid_states_for_not_audio_app;
  std::vector<am::HmiStatePtr> invalid_states_for_audio_app;
  std::vector<am::HmiState::StateID> valid_state_ids_;

  am::HmiStatePtr createHmiState(
      mobile_apis::HMILevel::eType hmi_level,
      mobile_apis::AudioStreamingState::eType aidio_ss,
      mobile_apis::SystemContext::eType system_context) {
    namespace HMILevel = mobile_apis::HMILevel;
    namespace AudioStreamingState = mobile_apis::AudioStreamingState;
    namespace SystemContext = mobile_apis::SystemContext;

    am::HmiStatePtr state =
        utils::MakeShared<am::HmiState>(simple_app_id_, &app_manager_mock_);
    state->set_hmi_level(hmi_level);
    state->set_audio_streaming_state(aidio_ss);
    state->set_system_context(system_context);
    return state;
  }

 protected:
  am::ApplicationSharedPtr ConfigureApp(NiceMock<ApplicationMock>** app_mock,
                                        uint32_t app_id, bool media, bool navi,
                                        bool vc) {
    *app_mock = new NiceMock<ApplicationMock>;

    Mock::AllowLeak(*app_mock);  // WorkAround for gogletest bug
    am::ApplicationSharedPtr app(*app_mock);

    ON_CALL(**app_mock, app_id()).WillByDefault(Return(app_id));
    ON_CALL(**app_mock, is_media_application()).WillByDefault(Return(media));
    ON_CALL(**app_mock, is_navi()).WillByDefault(Return(navi));
    ON_CALL(**app_mock, is_voice_communication_supported())
        .WillByDefault(Return(vc));
    ON_CALL(**app_mock, IsAudioApplication())
        .WillByDefault(Return(media || navi || vc));

    EXPECT_CALL(**app_mock, usage_report())
        .WillRepeatedly(ReturnRef(usage_stat));

    return app;
  }

  void FillStatesLists() {
    namespace HMILevel = mobile_apis::HMILevel;
    namespace AudioStreamingState = mobile_apis::AudioStreamingState;
    namespace SystemContext = mobile_apis::SystemContext;
    // Valid states for not audio app
    message_helper_mock_ = new MessageHelperMock;
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_VRSESSION));
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MENU));
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_HMI_OBSCURED));
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_ALERT));
    valid_states_for_not_audio_app_.push_back(createHmiState(
        HMILevel::HMI_BACKGROUND, AudioStreamingState::NOT_AUDIBLE,
        SystemContext::SYSCTXT_MAIN));
    valid_states_for_not_audio_app_.push_back(
        createHmiState(HMILevel::HMI_FULL, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));

    // Valid states audio app
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_VRSESSION));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MENU));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_HMI_OBSCURED));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_ALERT));
    valid_states_for_audio_app_.push_back(createHmiState(
        HMILevel::HMI_BACKGROUND, AudioStreamingState::NOT_AUDIBLE,
        SystemContext::SYSCTXT_MAIN));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::ATTENUATED,
                       SystemContext::SYSCTXT_MAIN));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_FULL, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    valid_states_for_audio_app_.push_back(
        createHmiState(HMILevel::HMI_FULL, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));

    // Common Invalid States
    common_invalid_states_.push_back(
        createHmiState(HMILevel::INVALID_ENUM, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    common_invalid_states_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::INVALID_ENUM,
                       SystemContext::SYSCTXT_MAIN));
    common_invalid_states_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::INVALID_ENUM));
    common_invalid_states_.push_back(createHmiState(
        HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
        SystemContext::SYSCTXT_MAIN));
    common_invalid_states_.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::INVALID_ENUM,
                       SystemContext::INVALID_ENUM));
    common_invalid_states_.push_back(createHmiState(
        HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
        SystemContext::INVALID_ENUM));
    common_invalid_states_.push_back(createHmiState(
        HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
        SystemContext::INVALID_ENUM));
    // Invalid States for audio apps
    invalid_states_for_audio_app.push_back(
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::NOT_AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_audio_app.push_back(
        createHmiState(HMILevel::HMI_BACKGROUND, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_audio_app.push_back(createHmiState(
        HMILevel::HMI_BACKGROUND, AudioStreamingState::ATTENUATED,
        SystemContext::SYSCTXT_MAIN));
    invalid_states_for_audio_app.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_audio_app.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::ATTENUATED,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_audio_app.push_back(
        createHmiState(HMILevel::HMI_NONE, AudioStreamingState::ATTENUATED,
                       SystemContext::SYSCTXT_MAIN));
    // Invalid States for not audio apps
    invalid_states_for_not_audio_app.push_back(
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::ATTENUATED,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_not_audio_app.push_back(
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_not_audio_app.push_back(
        createHmiState(HMILevel::HMI_FULL, AudioStreamingState::ATTENUATED,
                       SystemContext::SYSCTXT_MAIN));
    invalid_states_for_not_audio_app.push_back(
        createHmiState(HMILevel::HMI_FULL, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN));

    // Valid state ids
    valid_state_ids_.push_back(am::HmiState::StateID::STATE_ID_VR_SESSION);
    valid_state_ids_.push_back(am::HmiState::StateID::STATE_ID_TTS_SESSION);
    valid_state_ids_.push_back(am::HmiState::StateID::STATE_ID_PHONE_CALL);
    valid_state_ids_.push_back(am::HmiState::StateID::STATE_ID_SAFETY_MODE);
    valid_state_ids_.push_back(am::HmiState::StateID::STATE_ID_NAVI_STREAMING);
  }

  void ConfigureApps() {
    simple_app_ = ConfigureApp(&simple_app_ptr_, simple_app_id_, NOT_MEDIA,
                               NOT_NAVI, NOT_VC);
    media_app_ =
        ConfigureApp(&media_app_ptr_, media_app_id_, MEDIA, NOT_NAVI, NOT_VC);
    navi_app_ =
        ConfigureApp(&navi_app_ptr_, navi_app_id_, NOT_MEDIA, NAVI, NOT_VC);
    vc_app_ = ConfigureApp(&vc_app_ptr_, vc_app_id_, NOT_MEDIA, NOT_NAVI, VC);
    media_navi_app_ = ConfigureApp(&media_navi_app_ptr_, media_navi_app_id_,
                                   MEDIA, NAVI, NOT_VC);
    media_vc_app_ =
        ConfigureApp(&media_vc_app_ptr_, media_vc_app_id_, MEDIA, NOT_NAVI, VC);
    navi_vc_app_ =
        ConfigureApp(&navi_vc_app_ptr_, navi_vc_app_id_, NOT_MEDIA, NAVI, VC);
    media_navi_vc_app_ = ConfigureApp(&media_navi_vc_app_ptr_,
                                      media_navi_vc_app_id_, MEDIA, NAVI, VC);
  }
  void CheckAppConfiguration() {
    ASSERT_EQ(simple_app_.get(), simple_app_ptr_);
    ASSERT_EQ(media_app_.get(), media_app_ptr_);
    ASSERT_EQ(navi_app_.get(), navi_app_ptr_);
    ASSERT_EQ(vc_app_.get(), vc_app_ptr_);
    ASSERT_EQ(media_navi_app_.get(), media_navi_app_ptr_);
    ASSERT_EQ(media_vc_app_.get(), media_vc_app_ptr_);
    ASSERT_EQ(navi_vc_app_.get(), navi_vc_app_ptr_);
    ASSERT_EQ(media_navi_vc_app_.get(), media_navi_vc_app_ptr_);

    ASSERT_EQ(simple_app_->app_id(), simple_app_id_);
    ASSERT_EQ(media_app_->app_id(), media_app_id_);
    ASSERT_EQ(navi_app_->app_id(), navi_app_id_);
    ASSERT_EQ(vc_app_->app_id(), vc_app_id_);
    ASSERT_EQ(media_navi_app_->app_id(), media_navi_app_id_);
    ASSERT_EQ(media_vc_app_->app_id(), media_vc_app_id_);
    ASSERT_EQ(navi_vc_app_->app_id(), navi_vc_app_id_);
    ASSERT_EQ(media_navi_vc_app_->app_id(), media_navi_vc_app_id_);

    ASSERT_FALSE(simple_app_->IsAudioApplication());
    ASSERT_TRUE(media_app_->IsAudioApplication());
    ASSERT_TRUE(navi_app_->IsAudioApplication());
    ASSERT_TRUE(vc_app_->IsAudioApplication());
    ASSERT_TRUE(media_navi_app_->IsAudioApplication());
    ASSERT_TRUE(media_vc_app_->IsAudioApplication());
    ASSERT_TRUE(navi_vc_app_->IsAudioApplication());
    ASSERT_TRUE(media_navi_vc_app_->IsAudioApplication());

    ASSERT_FALSE(simple_app_->is_media_application());
    ASSERT_TRUE(media_app_->is_media_application());
    ASSERT_FALSE(navi_app_->is_media_application());
    ASSERT_FALSE(vc_app_->is_media_application());
    ASSERT_TRUE(media_navi_app_->is_media_application());
    ASSERT_TRUE(media_vc_app_->is_media_application());
    ASSERT_FALSE(navi_vc_app_->is_media_application());
    ASSERT_TRUE(media_navi_vc_app_->is_media_application());

    ASSERT_FALSE(simple_app_->is_navi());
    ASSERT_TRUE(navi_app_->is_navi());
    ASSERT_FALSE(media_app_->is_navi());
    ASSERT_FALSE(vc_app_->is_navi());
    ASSERT_TRUE(media_navi_app_->is_navi());
    ASSERT_FALSE(media_vc_app_->is_navi());
    ASSERT_TRUE(navi_vc_app_->is_navi());
    ASSERT_TRUE(media_navi_vc_app_->is_navi());

    ASSERT_FALSE(simple_app_->is_voice_communication_supported());
    ASSERT_FALSE(navi_app_->is_voice_communication_supported());
    ASSERT_FALSE(media_app_->is_voice_communication_supported());
    ASSERT_TRUE(vc_app_->is_voice_communication_supported());
    ASSERT_FALSE(media_navi_app_->is_voice_communication_supported());
    ASSERT_TRUE(media_vc_app_->is_voice_communication_supported());
    ASSERT_TRUE(navi_vc_app_->is_voice_communication_supported());
    ASSERT_TRUE(media_navi_vc_app_->is_voice_communication_supported());
  }

  void SetUp() {
    ON_CALL(app_manager_mock_, applications())
        .WillByDefault(Return(applications_));
    ConfigureApps();
    CheckAppConfiguration();
    FillStatesLists();
  }

  void TearDown() { delete message_helper_mock_; }

  void ExpectSuccesfullSetHmiState(am::ApplicationSharedPtr app,
                                   NiceMock<ApplicationMock>* app_mock,
                                   am::HmiStatePtr old_state,
                                   am::HmiStatePtr new_state) {
    EXPECT_CALL(*app_mock, CurrentHmiState())
        .WillOnce(Return(old_state))
        .WillOnce(Return(new_state));
    EXPECT_CALL(*app_mock,
                SetRegularState(Truly(HmiStatesComparator(new_state))));
    if (!HmiStatesComparator(old_state)(new_state)) {
      EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(app));
      EXPECT_CALL(app_manager_mock_,
                  OnHMILevelChanged(app->app_id(), old_state->hmi_level(),
                                    new_state->hmi_level()));
    }
  }

  void ExpectAppChangeHmiStateDueToConflictResolving(
      am::ApplicationSharedPtr app, NiceMock<ApplicationMock>* app_mock,
      am::HmiStatePtr old_state, am::HmiStatePtr new_state) {
    EXPECT_CALL(*app_mock, RegularHmiState())
        .WillOnce(Return(old_state))
        .WillOnce(Return(old_state));
    ExpectSuccesfullSetHmiState(app, app_mock, old_state, new_state);
  }

  void ExpectAppWontChangeHmiStateDueToConflictResolving(
      am::ApplicationSharedPtr app, NiceMock<ApplicationMock>* app_mock,
      am::HmiStatePtr state) {
    EXPECT_CALL(*app_mock, RegularHmiState()).WillOnce(Return(state));
    EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(app)).Times(0);
    EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(app->app_id(), _, _))
        .Times(0);
  }

  void InsertApplication(am::ApplicationSharedPtr app) {
    application_set_.insert(app);
    ON_CALL(app_manager_mock_, application(app->app_id()))
        .WillByDefault(Return(app));
  }

  am::HmiStatePtr NoneNotAudibleState() {
    return createHmiState(mobile_apis::HMILevel::HMI_NONE,
                          mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                          mobile_apis::SystemContext::SYSCTXT_MAIN);
  }

  am::HmiStatePtr FullAudibleState() {
    return createHmiState(mobile_apis::HMILevel::HMI_FULL,
                          mobile_apis::AudioStreamingState::AUDIBLE,
                          mobile_apis::SystemContext::SYSCTXT_MAIN);
  }

  am::HmiStatePtr FullNotAudibleState() {
    return createHmiState(mobile_apis::HMILevel::HMI_FULL,
                          mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                          mobile_apis::SystemContext::SYSCTXT_MAIN);
  }

  am::HmiStatePtr LimitedState() {
    return createHmiState(mobile_apis::HMILevel::HMI_LIMITED,
                          mobile_apis::AudioStreamingState::AUDIBLE,
                          mobile_apis::SystemContext::SYSCTXT_MAIN);
  }

  am::HmiStatePtr BackgroundState() {
    return createHmiState(mobile_apis::HMILevel::HMI_BACKGROUND,
                          mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                          mobile_apis::SystemContext::SYSCTXT_MAIN);
  }

  void CheckStateApplyingForApplication(
      NiceMock<ApplicationMock>& application,
      std::vector<am::HmiState::StateID>& state_ids) {
    using smart_objects::SmartObject;
    using am::event_engine::Event;
    namespace FunctionID = hmi_apis::FunctionID;

    EXPECT_CALL(application, CurrentHmiState())
        .WillRepeatedly(Return(NoneNotAudibleState()));

    for (uint32_t i = 0; i < state_ids.size(); ++i) {
      am::HmiState::StateID state_id = state_ids[i];
      EXPECT_CALL(application,
          AddHMIState(Truly(HmiStatesIDComparator(state_id)))).Times(1);

      switch (state_id) {
        case am::HmiState::StateID::STATE_ID_VR_SESSION: {
          Event vr_start_event(FunctionID::VR_Started);
          state_ctrl_.on_event(vr_start_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_TTS_SESSION: {
          Event tts_start_event(FunctionID::TTS_Started);
          state_ctrl_.on_event(tts_start_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_PHONE_CALL: {
          Event phone_call_event(
              FunctionID::BasicCommunication_OnPhoneCall);
          SmartObject message;
          message[am::strings::msg_params][am::hmi_notification::is_active] = true;
          phone_call_event.set_smart_object(message);
          state_ctrl_.on_event(phone_call_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_SAFETY_MODE: {
          Event emergency_event(
              FunctionID::BasicCommunication_OnEmergencyEvent);
          SmartObject message;
          message[am::strings::msg_params][am::hmi_response::enabled] = true;
          emergency_event.set_smart_object(message);
          state_ctrl_.on_event(emergency_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_NAVI_STREAMING: {
          state_ctrl_.OnNaviStreamingStarted();
          break;
        }
        default:
          break;
      }

      EXPECT_CALL(application, AddHMIState(_)).Times(0);
    }

    for (uint32_t i = 0; i < state_ids.size(); ++i) {
      am::HmiState::StateID state_id = state_ids[i];
      EXPECT_CALL(application, RemoveHMIState(state_id)).Times(1);

      switch (state_id) {
        case am::HmiState::StateID::STATE_ID_VR_SESSION: {
          Event vr_stop_event(FunctionID::VR_Stopped);
          state_ctrl_.on_event(vr_stop_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_TTS_SESSION: {
          Event tts_stop_event(FunctionID::TTS_Stopped);
          state_ctrl_.on_event(tts_stop_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_PHONE_CALL: {
          Event phone_call_event(
              FunctionID::BasicCommunication_OnPhoneCall);
          SmartObject message;
          message[am::strings::msg_params][am::hmi_notification::is_active] = false;
          phone_call_event.set_smart_object(message);
          state_ctrl_.on_event(phone_call_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_SAFETY_MODE: {
          Event emergency_event(
              FunctionID::BasicCommunication_OnEmergencyEvent);
          SmartObject message;
          message[am::strings::msg_params][am::hmi_response::enabled] = false;
          emergency_event.set_smart_object(message);
          state_ctrl_.on_event(emergency_event);
          break;
        }
        case am::HmiState::StateID::STATE_ID_NAVI_STREAMING: {
          state_ctrl_.OnNaviStreamingStopped();
          break;
        }
        default:
          break;
      }

      EXPECT_CALL(application, RemoveHMIState(_)).Times(0);
    }
  }
};

TEST_F(StateControllerTest, OnStateChangedWithEqualStates) {
  EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(_)).Times(0);
  EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
  EXPECT_CALL(*simple_app_ptr_, ResetDataInNone()).Times(0);

  for (uint32_t i = 0; i < valid_states_for_not_audio_app_.size(); ++i) {
    state_ctrl_.OnStateChanged(simple_app_,
                               valid_states_for_not_audio_app_[i],
                               valid_states_for_not_audio_app_[i]);
  }
}

TEST_F(StateControllerTest, OnStateChangedWithDifferentStates) {
  for (uint32_t i = 0; i < valid_states_for_not_audio_app_.size(); ++i) {
    for (uint32_t j = 0; j < valid_states_for_not_audio_app_.size(); ++j) {
      HmiStatesComparator comp(valid_states_for_not_audio_app_[i]);
      if (!comp(valid_states_for_not_audio_app_[j])) {
        EXPECT_CALL(app_manager_mock_,
            SendHMIStatusNotification(simple_app_)).Times(1);
        EXPECT_CALL(app_manager_mock_,
            OnHMILevelChanged(
                simple_app_id_,
                valid_states_for_not_audio_app_[i]->hmi_level(),
                valid_states_for_not_audio_app_[j]->hmi_level())).Times(1);
        if (mobile_apis::HMILevel::HMI_NONE ==
            valid_states_for_not_audio_app_[j]->hmi_level()) {
          EXPECT_CALL(*simple_app_ptr_, ResetDataInNone()).Times(1);
        }
        state_ctrl_.OnStateChanged(simple_app_,
                                   valid_states_for_not_audio_app_[i],
                                   valid_states_for_not_audio_app_[j]);

        EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(_)).Times(0);
        EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
        EXPECT_CALL(*simple_app_ptr_, ResetDataInNone()).Times(0);
      }
    }
  }
}

TEST_F(StateControllerTest, OnStateChangedToNone) {
  using namespace am;
  using namespace mobile_apis;

  HmiStatePtr none_state = createHmiState(HMILevel::HMI_NONE,
                                          AudioStreamingState::NOT_AUDIBLE,
                                          SystemContext::SYSCTXT_MAIN);
  HmiStatePtr not_none_state = createHmiState(HMILevel::HMI_FULL,
                                              AudioStreamingState::NOT_AUDIBLE,
                                              SystemContext::SYSCTXT_MAIN);

  EXPECT_CALL(*simple_app_ptr_, ResetDataInNone()).Times(0);
  state_ctrl_.OnStateChanged(simple_app_, none_state, not_none_state);

  EXPECT_CALL(*simple_app_ptr_, ResetDataInNone()).Times(1);
  state_ctrl_.OnStateChanged(simple_app_, not_none_state, none_state);
}

TEST_F(StateControllerTest, MoveSimpleAppToValidStates) {
  using am::HmiState;
  using am::HmiStatePtr;
  using am::UsageStatistics;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  HmiStatePtr initial_state =
      createHmiState(HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
                     SystemContext::INVALID_ENUM);

  for (std::vector<HmiStatePtr>::iterator it =
           valid_states_for_not_audio_app_.begin();
       it != valid_states_for_not_audio_app_.end(); ++it) {
    HmiStatePtr state_to_setup = *it;
    EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
        .WillOnce(Return(initial_state))
        .WillOnce(Return(state_to_setup));
    EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(simple_app_));
    EXPECT_CALL(app_manager_mock_,
                OnHMILevelChanged(simple_app_id_, initial_state->hmi_level(),
                                  state_to_setup->hmi_level()));

    EXPECT_CALL(*simple_app_ptr_,
                SetRegularState(Truly(HmiStatesComparator(state_to_setup))));
    state_ctrl_.SetRegularState<false>(simple_app_, state_to_setup);
    initial_state = state_to_setup;
  }
}

TEST_F(StateControllerTest, MoveAudioAppAppToValidStates) {
  using am::HmiState;
  using am::HmiStatePtr;
  using am::UsageStatistics;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  am::ApplicationSharedPtr audio_app = media_navi_vc_app_;
  NiceMock<ApplicationMock>* audio_app_mock = media_navi_vc_app_ptr_;

  HmiStatePtr initial_state =
      createHmiState(HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
                     SystemContext::INVALID_ENUM);

  for (std::vector<HmiStatePtr>::iterator it =
           valid_states_for_audio_app_.begin();
       it != valid_states_for_audio_app_.end(); ++it) {
    HmiStatePtr state_to_setup = *it;
    EXPECT_CALL(*audio_app_mock, CurrentHmiState())
        .WillOnce(Return(initial_state))
        .WillOnce(Return(state_to_setup));
    EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(audio_app));
    EXPECT_CALL(
        app_manager_mock_,
        OnHMILevelChanged(audio_app->app_id(), initial_state->hmi_level(),
                          state_to_setup->hmi_level()));

    EXPECT_CALL(*audio_app_mock,
                SetRegularState(Truly(HmiStatesComparator(state_to_setup))));
    state_ctrl_.SetRegularState<false>(media_navi_vc_app_, state_to_setup);
    initial_state = state_to_setup;
  }
}
/*
TEST_F(StateControllerTest, MoveAppFromValidStateToInvalid) {
  using am::HmiState;
  using am::HmiStatePtr;
  using am::UsageStatistics;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  for (std::vector<HmiStatePtr>::iterator valid_state_it =
           valid_states_for_not_audio_app_.begin();
       valid_state_it != valid_states_for_not_audio_app_.end();
       ++valid_state_it) {
    for (std::vector<HmiStatePtr>::iterator invalid_state_it =
             common_invalid_states_.begin();
         invalid_state_it != common_invalid_states_.end(); ++invalid_state_it) {
      HmiStatePtr initial_state = *valid_state_it;
      HmiStatePtr invalid_state = *invalid_state_it;
      EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
          .WillOnce(Return(initial_state));
      EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
      EXPECT_CALL(*simple_app_ptr_, SetRegularState(_)).Times(0);
      state_ctrl_.SetRegularState<false>(simple_app_, invalid_state);
    }
  }

  NiceMock<ApplicationMock>* audio_app_mock = media_navi_vc_app_ptr_;
  am::ApplicationSharedPtr audio_app = media_navi_vc_app_;
  for (std::vector<HmiStatePtr>::iterator valid_state_it =
           valid_states_for_audio_app_.begin();
       valid_state_it != valid_states_for_audio_app_.end(); ++valid_state_it) {
    for (std::vector<HmiStatePtr>::iterator invalid_state_it =
             invalid_states_for_audio_app.begin();
         invalid_state_it != invalid_states_for_audio_app.end();
         ++invalid_state_it) {
      HmiStatePtr initial_state = *valid_state_it;
      HmiStatePtr invalid_state = *invalid_state_it;
      EXPECT_CALL(*audio_app_mock, CurrentHmiState())
          .WillOnce(Return(initial_state));
      EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
      EXPECT_CALL(*audio_app_mock, SetRegularState(_)).Times(0);
      state_ctrl_.SetRegularState<false>(audio_app, invalid_state);
    }
  }

  for (std::vector<HmiStatePtr>::iterator valid_state_it =
           valid_states_for_not_audio_app_.begin();
       valid_state_it != valid_states_for_not_audio_app_.end();
       ++valid_state_it) {
    for (std::vector<HmiStatePtr>::iterator invalid_state_it =
             invalid_states_for_not_audio_app.begin();
         invalid_state_it != invalid_states_for_not_audio_app.end();
         ++invalid_state_it) {
      HmiStatePtr initial_state = *valid_state_it;
      HmiStatePtr invalid_state = *invalid_state_it;
      EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
          .WillOnce(Return(initial_state));
      EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
      EXPECT_CALL(*simple_app_ptr_, SetRegularState(_)).Times(0);
      state_ctrl_.SetRegularState<false>(simple_app_, invalid_state);
    }
  }
}

TEST_F(StateControllerTest, MoveAppFromInValidStateToValid) {
  using am::HmiState;
  using am::HmiStatePtr;
  using am::UsageStatistics;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  NiceMock<ApplicationMock>* audio_app_mock = media_navi_vc_app_ptr_;
  am::ApplicationSharedPtr audio_app = media_navi_vc_app_;
  HmiStatePtr invalid_state =
      createHmiState(HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
                     SystemContext::INVALID_ENUM);

  for (std::vector<HmiStatePtr>::iterator it =
           valid_states_for_audio_app_.begin();
       it != valid_states_for_audio_app_.end(); ++it) {
    HmiStatePtr initial_state = *it;
    EXPECT_CALL(*audio_app_mock, CurrentHmiState())
        .WillOnce(Return(initial_state));
    EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
    EXPECT_CALL(*audio_app_mock, SetRegularState(_)).Times(0);
    state_ctrl_.SetRegularState<false>(audio_app, invalid_state);
  }

  for (std::vector<HmiStatePtr>::iterator it =
           valid_states_for_not_audio_app_.begin();
       it != valid_states_for_not_audio_app_.end(); ++it) {
    HmiStatePtr initial_state = *it;
    EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
        .WillOnce(Return(initial_state));
    EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(_, _, _)).Times(0);
    EXPECT_CALL(*simple_app_ptr_, SetRegularState(_)).Times(0);
    state_ctrl_.SetRegularState<false>(simple_app_, invalid_state);
  }
}

TEST_F(StateControllerTest, MoveAppFromInValidStateToInvalid) {
  using am::HmiState;
  using am::HmiStatePtr;
  using am::UsageStatistics;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  NiceMock<ApplicationMock>* audio_app_mock = media_navi_vc_app_ptr_;
  am::ApplicationSharedPtr audio_app = media_navi_vc_app_;
  HmiStatePtr initial_invalid_state =
      createHmiState(HMILevel::INVALID_ENUM, AudioStreamingState::INVALID_ENUM,
                     SystemContext::INVALID_ENUM);

  am::ApplicationConstSharedPtr const_audio_app(audio_app);

  for (std::vector<HmiStatePtr>::iterator it =
           invalid_states_for_audio_app.begin();
       it != invalid_states_for_audio_app.end(); ++it) {
    HmiStatePtr state_to_setup = *it;
    HmiStatePtr default_state =
        createHmiState(HMILevel::HMI_LIMITED, AudioStreamingState::AUDIBLE,
                       SystemContext::SYSCTXT_MAIN);
    EXPECT_CALL(app_manager_mock_, GetDefaultHmiLevel(const_audio_app))
        .WillOnce(Return(HMILevel::HMI_LIMITED));
    EXPECT_CALL(*audio_app_mock, CurrentHmiState())
        .WillOnce(Return(initial_invalid_state))
        .WillOnce(Return(initial_invalid_state))
        .WillOnce(Return(default_state));
    EXPECT_CALL(app_manager_mock_,
                OnHMILevelChanged(audio_app->app_id(),
                                  initial_invalid_state->hmi_level(),
                                  default_state->hmi_level()));
    EXPECT_CALL(*audio_app_mock,
                SetRegularState(Truly(HmiStatesComparator(default_state))));
    state_ctrl_.SetRegularState<false>(audio_app, state_to_setup);
  }

  for (std::vector<HmiStatePtr>::iterator it =
           invalid_states_for_not_audio_app.begin();
       it != invalid_states_for_not_audio_app.end(); ++it) {
    HmiStatePtr state_to_setup = *it;
    HmiStatePtr default_state = createHmiState(HMILevel::HMI_BACKGROUND,
                                               AudioStreamingState::NOT_AUDIBLE,
                                               SystemContext::SYSCTXT_MAIN);
    am::ApplicationConstSharedPtr const_simple_app(simple_app_);
    EXPECT_CALL(app_manager_mock_, GetDefaultHmiLevel(const_simple_app))
        .WillOnce(Return(HMILevel::HMI_BACKGROUND));
    EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
        .WillOnce(Return(initial_invalid_state))
        .WillOnce(Return(initial_invalid_state))
        .WillOnce(Return(default_state));
    EXPECT_CALL(app_manager_mock_,
                OnHMILevelChanged(simple_app_ptr_->app_id(),
                                  initial_invalid_state->hmi_level(),
                                  default_state->hmi_level()));
    EXPECT_CALL(*simple_app_ptr_,
                SetRegularState(Truly(HmiStatesComparator(default_state))));
    state_ctrl_.SetRegularState<false>(simple_app_, state_to_setup);
  }
}
*/
TEST_F(StateControllerTest, SetFullToSimpleAppWhileAnotherSimpleAppIsInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  am::ApplicationSharedPtr app_in_full;
  NiceMock<ApplicationMock>* app_in_full_mock;

  am::ApplicationSharedPtr app_moved_to_full;
  NiceMock<ApplicationMock>* app_moved_to_full_mock;

  app_in_full =
      ConfigureApp(&app_in_full_mock, 1761, NOT_MEDIA, NOT_NAVI, NOT_VC);
  app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1796, NOT_MEDIA, NOT_NAVI, NOT_VC);

  InsertApplication(app_in_full);
  InsertApplication(app_moved_to_full);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullNotAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_full, app_in_full_mock, FullNotAudibleState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullNotAudibleState());
}

TEST_F(StateControllerTest, SetFullToSimpleAppWhileAudioAppAppIsInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  am::ApplicationSharedPtr app_in_full = media_navi_vc_app_;
  NiceMock<ApplicationMock>* app_in_full_mock = media_navi_vc_app_ptr_;

  am::ApplicationSharedPtr app_moved_to_full = simple_app_;
  NiceMock<ApplicationMock>* app_moved_to_full_mock = simple_app_ptr_;

  InsertApplication(app_in_full);
  InsertApplication(app_moved_to_full);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullNotAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_full, app_in_full_mock, FullAudibleState(), LimitedState());
  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullNotAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToAudioAppAppWhileAnotherTypeAudioAppAppIsInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  am::ApplicationSharedPtr app_in_full = media_app_;
  NiceMock<ApplicationMock>* app_in_full_mock = media_app_ptr_;

  am::ApplicationSharedPtr app_moved_to_full = navi_app_;
  NiceMock<ApplicationMock>* app_moved_to_full_mock = navi_app_ptr_;

  InsertApplication(app_in_full);
  InsertApplication(app_moved_to_full);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_full, app_in_full_mock, FullAudibleState(), LimitedState());
  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToAudioAppAppWhileSameTypeAudioAppAppIsInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  NiceMock<ApplicationMock>* app_in_full_mock;
  am::ApplicationSharedPtr app_in_full =
      ConfigureApp(&app_in_full_mock, 1761, MEDIA, NOT_NAVI, NOT_VC);

  NiceMock<ApplicationMock>* app_moved_to_full_mock;
  am::ApplicationSharedPtr app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1796, MEDIA, NOT_NAVI, NOT_VC);

  InsertApplication(app_in_full);
  InsertApplication(app_moved_to_full);
  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_full, app_in_full_mock, FullAudibleState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToAudioAppAppWhileSameTypeAudioAppAppIsInLimited) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  NiceMock<ApplicationMock>* app_in_limited_mock;
  am::ApplicationSharedPtr app_in_limited =
      ConfigureApp(&app_in_limited_mock, 1761, NOT_MEDIA, NAVI, NOT_VC);

  NiceMock<ApplicationMock>* app_moved_to_full_mock;
  am::ApplicationSharedPtr app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1796, NOT_MEDIA, NAVI, VC);

  InsertApplication(app_in_limited);
  InsertApplication(app_moved_to_full);
  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_limited, app_in_limited_mock, LimitedState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullAudibleState());
}

TEST_F(StateControllerTest,
       SetLimitedToAudioAppAppWhileSameTypeAudioAppAppIsInLimited) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  NiceMock<ApplicationMock>* app_in_limited_mock;
  am::ApplicationSharedPtr app_in_limited =
      ConfigureApp(&app_in_limited_mock, 1761, NOT_MEDIA, NOT_NAVI, VC);

  NiceMock<ApplicationMock>* app_moved_to_limited_mock;
  am::ApplicationSharedPtr app_moved_to_limited =
      ConfigureApp(&app_moved_to_limited_mock, 1796, NOT_MEDIA, NOT_NAVI, VC);

  InsertApplication(app_in_limited);
  InsertApplication(app_moved_to_limited);

  ExpectSuccesfullSetHmiState(app_moved_to_limited, app_moved_to_limited_mock,
                              BackgroundState(), LimitedState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      app_in_limited, app_in_limited_mock, LimitedState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_limited, LimitedState());
}

TEST_F(StateControllerTest,
       SetLimitedToAudioAppAppWhileOtherTypeAudioAppAppIsInLimited) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  am::ApplicationSharedPtr app_in_limited = navi_app_;
  NiceMock<ApplicationMock>* app_in_limited_mock = navi_app_ptr_;

  am::ApplicationSharedPtr app_moved_to_limited = vc_app_;
  NiceMock<ApplicationMock>* app_moved_to_limited_mock = vc_app_ptr_;

  InsertApplication(app_in_limited);
  InsertApplication(app_moved_to_limited);
  ExpectSuccesfullSetHmiState(app_moved_to_limited, app_moved_to_limited_mock,
                              BackgroundState(), LimitedState());
  ExpectAppWontChangeHmiStateDueToConflictResolving(
      app_in_limited, app_in_limited_mock, LimitedState());
  state_ctrl_.SetRegularState<false>(app_moved_to_limited, LimitedState());
}

TEST_F(StateControllerTest,
       SetLimitedToAudioAppAppWhileOtherTypeAudioAppAppIsInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;
  am::ApplicationSharedPtr app_in_full = navi_app_;
  NiceMock<ApplicationMock>* app_in_full_mock = navi_app_ptr_;

  am::ApplicationSharedPtr app_moved_to_limited = vc_app_;
  NiceMock<ApplicationMock>* app_moved_to_limited_mock = vc_app_ptr_;

  InsertApplication(app_in_full);
  InsertApplication(app_moved_to_limited);

  ExpectSuccesfullSetHmiState(app_moved_to_limited, app_moved_to_limited_mock,
                              BackgroundState(), LimitedState());

  ExpectAppWontChangeHmiStateDueToConflictResolving(
      app_in_full, app_in_full_mock, FullAudibleState());
  state_ctrl_.SetRegularState<false>(app_moved_to_limited, LimitedState());
}

TEST_F(StateControllerTest, SetFullToSimpleAppWhile2AudioAppsInLimited) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  am::ApplicationSharedPtr app_moved_to_full = simple_app_;
  NiceMock<ApplicationMock>* app_moved_to_full_mock = simple_app_ptr_;

  am::ApplicationSharedPtr limited_app1 = media_app_;
  NiceMock<ApplicationMock>* limited_app1_mock = media_app_ptr_;

  am::ApplicationSharedPtr limited_app2 = navi_vc_app_;
  NiceMock<ApplicationMock>* limited_app2_mock = navi_vc_app_ptr_;

  InsertApplication(app_moved_to_full);
  InsertApplication(limited_app1);
  InsertApplication(limited_app2);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullNotAudibleState());

  ExpectAppWontChangeHmiStateDueToConflictResolving(
      limited_app1, limited_app1_mock, LimitedState());
  ExpectAppWontChangeHmiStateDueToConflictResolving(
      limited_app2, limited_app2_mock, LimitedState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullNotAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToSimpleAppWhile1AudioAppInLimitedAnd1AudioAppInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  am::ApplicationSharedPtr app_moved_to_full = simple_app_;
  NiceMock<ApplicationMock>* app_moved_to_full_mock = simple_app_ptr_;

  am::ApplicationSharedPtr limited_app = media_app_;
  NiceMock<ApplicationMock>* limited_app_mock = media_app_ptr_;

  am::ApplicationSharedPtr full_app = navi_vc_app_;
  NiceMock<ApplicationMock>* full_app_mock = navi_vc_app_ptr_;

  InsertApplication(app_moved_to_full);
  InsertApplication(limited_app);
  InsertApplication(full_app);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullNotAudibleState());

  ExpectAppWontChangeHmiStateDueToConflictResolving(
      limited_app, limited_app_mock, LimitedState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      full_app, full_app_mock, FullAudibleState(), LimitedState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullNotAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToSimpleAppWhile1AudioAppInLimitedAnd1SimpleAppInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  NiceMock<ApplicationMock>* app_moved_to_full_mock;
  am::ApplicationSharedPtr app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1761, NOT_MEDIA, NOT_NAVI, NOT_VC);

  am::ApplicationSharedPtr limited_app = media_app_;
  NiceMock<ApplicationMock>* limited_app_mock = media_app_ptr_;

  NiceMock<ApplicationMock>* full_app_mock;
  am::ApplicationSharedPtr full_app =
      ConfigureApp(&full_app_mock, 1796, NOT_MEDIA, NOT_NAVI, NOT_VC);

  InsertApplication(app_moved_to_full);
  InsertApplication(limited_app);
  InsertApplication(full_app);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullNotAudibleState());

  ExpectAppWontChangeHmiStateDueToConflictResolving(
      limited_app, limited_app_mock, LimitedState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      full_app, full_app_mock, FullNotAudibleState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullNotAudibleState());
}

TEST_F(
    StateControllerTest,
    SetFullToAudioAppWhile1AudioAppWithSameTypeInLimitedAnd1SimpleAppInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  NiceMock<ApplicationMock>* app_moved_to_full_mock;
  am::ApplicationSharedPtr app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1761, MEDIA, NOT_NAVI, NOT_VC);

  NiceMock<ApplicationMock>* limited_app_mock;
  am::ApplicationSharedPtr limited_app =
      ConfigureApp(&limited_app_mock, 1762, MEDIA, NOT_NAVI, NOT_VC);

  NiceMock<ApplicationMock>* full_app_mock;
  am::ApplicationSharedPtr full_app =
      ConfigureApp(&full_app_mock, 1796, NOT_MEDIA, NOT_NAVI, NOT_VC);

  InsertApplication(app_moved_to_full);
  InsertApplication(limited_app);
  InsertApplication(full_app);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      limited_app, limited_app_mock, LimitedState(), BackgroundState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      full_app, full_app_mock, FullNotAudibleState(), BackgroundState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullAudibleState());
}

TEST_F(
    StateControllerTest,
    SetFullToAudioAppWhileAudioAppWithSameTypeInLimitedAndAudioAppWithOtherTypeInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  NiceMock<ApplicationMock>* app_moved_to_full_mock;
  am::ApplicationSharedPtr app_moved_to_full =
      ConfigureApp(&app_moved_to_full_mock, 1761, MEDIA, NOT_NAVI, NOT_VC);

  NiceMock<ApplicationMock>* limited_app_mock;
  am::ApplicationSharedPtr limited_app =
      ConfigureApp(&limited_app_mock, 1762, MEDIA, NOT_NAVI, NOT_VC);

  NiceMock<ApplicationMock>* full_app_mock;
  am::ApplicationSharedPtr full_app =
      ConfigureApp(&full_app_mock, 1796, NOT_MEDIA, NAVI, NOT_VC);

  InsertApplication(app_moved_to_full);
  InsertApplication(limited_app);
  InsertApplication(full_app);

  ExpectSuccesfullSetHmiState(app_moved_to_full, app_moved_to_full_mock,
                              BackgroundState(), FullAudibleState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      limited_app, limited_app_mock, LimitedState(), BackgroundState());

  ExpectAppChangeHmiStateDueToConflictResolving(
      full_app, full_app_mock, FullAudibleState(), LimitedState());

  state_ctrl_.SetRegularState<false>(app_moved_to_full, FullAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToAudioAppWhile3AudioAppsWithSameTypeInLimited) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  InsertApplication(media_navi_vc_app_);
  InsertApplication(media_app_);
  InsertApplication(navi_app_);
  InsertApplication(vc_app_);
  ExpectSuccesfullSetHmiState(media_navi_vc_app_, media_navi_vc_app_ptr_,
                              BackgroundState(), FullAudibleState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      media_app_, media_app_ptr_, LimitedState(), BackgroundState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      navi_app_, navi_app_ptr_, LimitedState(), BackgroundState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      vc_app_, vc_app_ptr_, LimitedState(), BackgroundState());
  state_ctrl_.SetRegularState<false>(media_navi_vc_app_, FullAudibleState());
}

TEST_F(StateControllerTest,
       SetFullToAudioAppWhile2AudioAppsWithSameTypeInLimitedAndOneInFull) {
  using am::HmiState;
  using am::HmiStatePtr;
  namespace HMILevel = mobile_apis::HMILevel;
  namespace AudioStreamingState = mobile_apis::AudioStreamingState;
  namespace SystemContext = mobile_apis::SystemContext;

  InsertApplication(media_navi_vc_app_);
  InsertApplication(media_app_);
  InsertApplication(navi_app_);
  InsertApplication(vc_app_);
  ExpectSuccesfullSetHmiState(media_navi_vc_app_, media_navi_vc_app_ptr_,
                              BackgroundState(), FullAudibleState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      media_app_, media_app_ptr_, LimitedState(), BackgroundState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      navi_app_, navi_app_ptr_, LimitedState(), BackgroundState());
  ExpectAppChangeHmiStateDueToConflictResolving(
      vc_app_, vc_app_ptr_, FullAudibleState(), BackgroundState());
  state_ctrl_.SetRegularState<false>(media_navi_vc_app_, FullAudibleState());
}

TEST_F(StateControllerTest, ActivateAppSuccessReceivedFromHMI) {
  using namespace hmi_apis;
  using namespace mobile_apis;

  const uint32_t corr_id = 314;
  const uint32_t hmi_app_id = 2718;
  typedef std::pair<am::HmiStatePtr, Common_HMILevel::eType> StateLevelPair;
  std::vector<StateLevelPair> hmi_states;
  hmi_states.push_back(
      StateLevelPair(FullAudibleState(), Common_HMILevel::FULL));
  hmi_states.push_back(
      StateLevelPair(FullNotAudibleState(), Common_HMILevel::FULL));
  hmi_states.push_back(
      StateLevelPair(LimitedState(), Common_HMILevel::LIMITED));
  hmi_states.push_back(
      StateLevelPair(BackgroundState(), Common_HMILevel::BACKGROUND));
  hmi_states.push_back(StateLevelPair(
      createHmiState(HMILevel::HMI_NONE, AudioStreamingState::NOT_AUDIBLE,
                     SystemContext::SYSCTXT_MAIN),
      Common_HMILevel::NONE));
  std::vector<StateLevelPair> initial_hmi_states = hmi_states;
  std::vector<StateLevelPair>::iterator it = hmi_states.begin();
  std::vector<StateLevelPair>::iterator it2 = initial_hmi_states.begin();
  for (; it != hmi_states.end(); ++it) {
    for (; it2 != initial_hmi_states.end(); ++it2) {
      am::HmiStatePtr hmi_state = it->first;
      am::HmiStatePtr initial_hmi_state = it->first;
      Common_HMILevel::eType hmi_level = it->second;

      EXPECT_CALL(*message_helper_mock_,
                  SendActivateAppToHMI(media_app_->app_id(), hmi_level, _))
          .WillOnce(Return(corr_id));
      EXPECT_CALL(app_manager_mock_, application_id(corr_id))
          .WillOnce(Return(hmi_app_id));
      EXPECT_CALL(app_manager_mock_, application_by_hmi_app(hmi_app_id))
          .WillOnce(Return(media_app_));
      ExpectSuccesfullSetHmiState(media_app_, media_app_ptr_, initial_hmi_state,
                                  hmi_state);
      state_ctrl_.SetRegularState<true>(media_app_, hmi_state);
      smart_objects::SmartObject message;
      message[am::strings::params][am::hmi_response::code] =
          Common_Result::SUCCESS;
      message[am::strings::params][am::strings::correlation_id] = corr_id;
      am::event_engine::Event event(
          hmi_apis::FunctionID::BasicCommunication_ActivateApp);
      event.set_smart_object(message);
      state_ctrl_.on_event(event);
    }
  }
}
/*
TEST_F(StateControllerTest, ActivateAppErrorReceivedFromHMI) {
  using namespace hmi_apis;
  const uint32_t corr_id = 314;
  const uint32_t hmi_app_id = 2718;
  std::vector<Common_Result::eType> hmi_results;
  hmi_results.push_back(Common_Result::ABORTED);
  hmi_results.push_back(Common_Result::APPLICATION_NOT_REGISTERED);
  hmi_results.push_back(Common_Result::CHAR_LIMIT_EXCEEDED);
  hmi_results.push_back(Common_Result::DATA_NOT_AVAILABLE);
  hmi_results.push_back(Common_Result::DISALLOWED);
  hmi_results.push_back(Common_Result::DUPLICATE_NAME);
  hmi_results.push_back(Common_Result::GENERIC_ERROR);
  hmi_results.push_back(Common_Result::IGNORED);
  hmi_results.push_back(Common_Result::INVALID_DATA);
  hmi_results.push_back(Common_Result::INVALID_ENUM);
  hmi_results.push_back(Common_Result::INVALID_ID);
  hmi_results.push_back(Common_Result::IN_USE);
  hmi_results.push_back(Common_Result::NO_APPS_REGISTERED);
  hmi_results.push_back(Common_Result::NO_DEVICES_CONNECTED);
  hmi_results.push_back(Common_Result::OUT_OF_MEMORY);
  hmi_results.push_back(Common_Result::REJECTED);
  hmi_results.push_back(Common_Result::RETRY);
  hmi_results.push_back(Common_Result::TIMED_OUT);
  hmi_results.push_back(Common_Result::TOO_MANY_PENDING_REQUESTS);
  hmi_results.push_back(Common_Result::TRUNCATED_DATA);
  hmi_results.push_back(Common_Result::UNSUPPORTED_REQUEST);
  hmi_results.push_back(Common_Result::UNSUPPORTED_RESOURCE);
  hmi_results.push_back(Common_Result::USER_DISALLOWED);
  hmi_results.push_back(Common_Result::WARNINGS);
  hmi_results.push_back(Common_Result::WRONG_LANGUAGE);

  std::vector<Common_Result::eType>::iterator it = hmi_results.begin();
  for (; it != hmi_results.end(); ++it) {
    EXPECT_CALL(
        *message_helper_mock_,
        SendActivateAppToHMI(simple_app_->app_id(), Common_HMILevel::FULL, _))
        .WillOnce(Return(corr_id));
    EXPECT_CALL(app_manager_mock_, application_id(corr_id))
        .WillOnce(Return(hmi_app_id));
    EXPECT_CALL(app_manager_mock_, application_by_hmi_app(hmi_app_id))
        .WillOnce(Return(simple_app_));
    EXPECT_CALL(*simple_app_ptr_, RegularHmiState())
        .WillOnce(Return(BackgroundState()));
    EXPECT_CALL(*simple_app_ptr_, CurrentHmiState())
        .WillOnce(Return(BackgroundState()))
        .WillOnce(Return(BackgroundState()));
    EXPECT_CALL(*simple_app_ptr_,
                SetRegularState(Truly(HmiStatesComparator(BackgroundState()))));
    EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(simple_app_))
        .Times(0);
    EXPECT_CALL(app_manager_mock_,
                OnHMILevelChanged(simple_app_->app_id(), _, _)).Times(0);
    state_ctrl_.SetRegularState<true>(simple_app_, FullNotAudibleState());
    smart_objects::SmartObject message;
    message[am::strings::params][am::hmi_response::code] = *it;
    message[am::strings::params][am::strings::correlation_id] = corr_id;
    am::event_engine::Event event(FunctionID::BasicCommunication_ActivateApp);
    event.set_smart_object(message);
    state_ctrl_.on_event(event);
  }
}
*/
TEST_F(StateControllerTest, ActivateAppInvalidCorrelationId) {
  using namespace hmi_apis;
  const uint32_t corr_id = 314;
  const uint32_t hmi_app_id = 2718;
  EXPECT_CALL(*message_helper_mock_,
              SendActivateAppToHMI(simple_app_->app_id(), Common_HMILevel::FULL,
                                   _)).WillOnce(Return(hmi_app_id));
  EXPECT_CALL(app_manager_mock_, application_id(corr_id))
      .WillOnce(Return(hmi_app_id));
  EXPECT_CALL(app_manager_mock_, application_by_hmi_app(hmi_app_id))
      .WillOnce(Return(am::ApplicationSharedPtr()));
  EXPECT_CALL(*simple_app_ptr_, SetRegularState(_)).Times(0);
  EXPECT_CALL(app_manager_mock_, SendHMIStatusNotification(simple_app_))
      .Times(0);
  EXPECT_CALL(app_manager_mock_, OnHMILevelChanged(simple_app_->app_id(), _, _))
      .Times(0);
  state_ctrl_.SetRegularState<true>(simple_app_, FullNotAudibleState());
  smart_objects::SmartObject message;
  message[am::strings::params][am::hmi_response::code] = Common_Result::SUCCESS;
  message[am::strings::params][am::strings::correlation_id] = corr_id;
  am::event_engine::Event event(FunctionID::BasicCommunication_ActivateApp);
  event.set_smart_object(message);
  state_ctrl_.on_event(event);
}
/*
TEST_F(StateControllerTest, ApplyTempStatesForSimpleApp) {
  InsertApplication(simple_app_);
  CheckStateApplyingForApplication(*simple_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForMediaApp) {
  InsertApplication(media_app_);
  CheckStateApplyingForApplication(*media_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForNaviApp) {
  InsertApplication(navi_app_);
  CheckStateApplyingForApplication(*navi_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForVCApp) {
  InsertApplication(vc_app_);
  CheckStateApplyingForApplication(*vc_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForMediaNaviApp) {
  InsertApplication(media_navi_app_);
  CheckStateApplyingForApplication(*media_navi_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForMediaVCApp) {
  InsertApplication(media_vc_app_);
  CheckStateApplyingForApplication(*media_vc_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForNaviVCApp) {
  InsertApplication(navi_vc_app_);
  CheckStateApplyingForApplication(*navi_vc_app_ptr_, valid_state_ids_);
}

TEST_F(StateControllerTest, ApplyTempStatesForMediaNaviVCApp) {
  InsertApplication(media_navi_vc_app_);
  CheckStateApplyingForApplication(*media_navi_vc_app_ptr_, valid_state_ids_);
}
*/
}  // namespace state_controller_test
