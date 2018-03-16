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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_STATE_CONTROLLER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_STATE_CONTROLLER_H_

#include "gmock/gmock.h"
#include "application_manager/application.h"
#include "application_manager/hmi_state.h"
#include "application_manager/state_controller.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/application.h"

namespace test {
namespace components {
namespace application_manager_test {
namespace am = application_manager;

class MockStateController : public am::StateController {
 public:
  MOCK_METHOD3(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    am::HmiStatePtr state,
                    const bool SendActivateApp));
  MOCK_METHOD5(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::HMILevel::eType hmi_level,
                    const mobile_apis::AudioStreamingState::eType audio_state,
                    const mobile_apis::VideoStreamingState::eType video_state,
                    const bool SendActivateApp));
  MOCK_METHOD3(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::HMILevel::eType hmi_level,
                    const bool SendActivateApp));
  MOCK_METHOD6(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::HMILevel::eType hmi_level,
                    const mobile_apis::AudioStreamingState::eType audio_state,
                    const mobile_apis::VideoStreamingState::eType video_state,
                    const mobile_apis::SystemContext::eType system_context,
                    const bool SendActivateApp));
  MOCK_METHOD2(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::HMILevel::eType hmi_level));
  MOCK_METHOD3(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::AudioStreamingState::eType audio_state,
                    const mobile_apis::VideoStreamingState::eType video_state));
  MOCK_METHOD2(SetRegularState,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::SystemContext::eType system_context));
  MOCK_METHOD2(SetRegularState,
               void(am::ApplicationSharedPtr app, am::HmiStatePtr state));
  MOCK_METHOD2(OnApplicationRegistered,
               void(am::ApplicationSharedPtr app,
                    const mobile_apis::HMILevel::eType default_level));
  MOCK_METHOD3(SendBCActivateApp,
               int64_t(am::ApplicationConstSharedPtr app,
                       hmi_apis::Common_HMILevel::eType level,
                       bool send_policy_priority));
  MOCK_METHOD0(OnNaviStreamingStarted, void());
  MOCK_METHOD0(OnNaviStreamingStopped, void());
  MOCK_METHOD3(OnStateChanged,
               void(am::ApplicationSharedPtr app,
                    am::HmiStatePtr old_state,
                    am::HmiStatePtr new_state));
  MOCK_CONST_METHOD1(IsStateActive, bool(am::HmiState::StateID state_id));
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_STATE_CONTROLLER_H_
