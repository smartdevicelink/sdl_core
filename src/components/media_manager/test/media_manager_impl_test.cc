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

#include "gmock/gmock.h"
#include "media_manager/media_manager_impl.h"
#include "media_manager/mock_media_adapter.h"
#include "media_manager/mock_media_adapter_impl.h"
#include "media_manager/mock_media_adapter_listener.h"
#include "media_manager/mock_media_manager_settings.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/message.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/state_controller.h"
#include "protocol_handler/mock_protocol_handler.h"
#include "protocol/common.h"
#include "utils/make_shared.h"
#include "utils/shared_ptr.h"
#include "utils/file_system.h"
#include "utils/scope_guard.h"

namespace test {
namespace components {
namespace media_manager_test {

using namespace file_system;
using namespace ::media_manager;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::utils::SharedPtr;
using ::utils::ScopeGuard;
using ::utils::MakeGuard;
using ::testing::NiceMock;
using ::protocol_handler::ServiceType;
using ::protocol_handler::RawMessagePtr;
using application_manager::ApplicationSharedPtr;
using application_manager::BinaryData;

namespace {
const uint16_t kVideoStreamingPort = 8901u;
const uint16_t kAudioStreamingPort = 8000u;
const int32_t kApplicationKey = 1;
const int32_t kDuration = 1000;
const std::string kStorageFolder = "test_storage_folder";
const std::string kOutputFile = "test_output_file";
const std::string kResourceFolder = "test_resource_folder";
const std::string kRecordingFileSource = "test_recording_file_source";
const std::string kNamedVideoPipePath = "named_video_pipe_path";
const std::string kNamedAudioPipePath = "named_audio_pipe_path";
const std::string kVideoStreamFile = "video_stream_file";
const std::string kAudioStreamFile = "audio_stream_file";
const std::string kServerAddress = "server_address";
const std::string kSocketValue = "socket";
const std::string kPipeValue = "pipe";
const std::string kFileValue = "file";
const std::string kDefaultValue = "";
const std::string kOutputFilePath = kStorageFolder + "/" + kOutputFile;
const uint32_t kProtocolVersion = ::protocol_handler::PROTOCOL_VERSION_2;
const uint32_t kConnectionKey = 1u;

void dealloc_data(uint8_t* data) {
  delete[] data;
}

void dealloc_file(std::ofstream* test_file) {
  delete test_file;
}
}  // namespace

typedef NiceMock<application_manager_test::MockApplication> MockApp;
typedef SharedPtr<MockApp> MockAppPtr;
typedef SharedPtr<MockMediaAdapterImpl> MockMediaAdapterImplPtr;

class MediaManagerImplTest : public ::testing::Test {
 public:
  // media_adapter_mock_ will be deleted in media_manager_impl (dtor)
  MediaManagerImplTest() : media_adapter_mock_(new MockMediaAdapter()) {
    media_adapter_listener_mock_ =
        utils::MakeShared<MockMediaAdapterListener>();
    ON_CALL(mock_media_manager_settings_, video_server_type())
        .WillByDefault(ReturnRef(kDefaultValue));
    ON_CALL(mock_media_manager_settings_, audio_server_type())
        .WillByDefault(ReturnRef(kDefaultValue));
    mock_app_ = ::utils::MakeShared<MockApp>();
    media_manager_impl_.reset(
        new MediaManagerImpl(app_mngr_, mock_media_manager_settings_));
  }

 protected:
  void StartMicrophoneCheckHelper() {
    EXPECT_CALL(app_mngr_, application(kApplicationKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_media_manager_settings_, app_storage_folder())
        .WillOnce(ReturnRef(kStorageFolder));
    EXPECT_CALL(mock_media_manager_settings_, app_resource_folder())
        .WillOnce(ReturnRef(kResourceFolder));
    EXPECT_CALL(mock_media_manager_settings_, recording_file_source())
        .WillOnce(ReturnRef(kRecordingFileSource));
  }

  void InitMediaManagerPrecondition(const std::string& server_type) {
    EXPECT_CALL(mock_media_manager_settings_, video_server_type())
        .WillRepeatedly(ReturnRef(server_type));
    EXPECT_CALL(mock_media_manager_settings_, audio_server_type())
        .WillRepeatedly(ReturnRef(server_type));
  }

  void InitMediaManagerSocketServerType() {
    InitMediaManagerPrecondition(kSocketValue);
    EXPECT_CALL(mock_media_manager_settings_, server_address())
        .WillRepeatedly(ReturnRef(kServerAddress));
    EXPECT_CALL(mock_media_manager_settings_, video_streaming_port())
        .WillOnce(Return(kVideoStreamingPort));
    EXPECT_CALL(mock_media_manager_settings_, audio_streaming_port())
        .WillOnce(Return(kAudioStreamingPort));
    media_manager_impl_.reset(
        new MediaManagerImpl(app_mngr_, mock_media_manager_settings_));
  }

  void InitMediaManagerPipeServerType() {
    InitMediaManagerPrecondition(kPipeValue);
    EXPECT_CALL(mock_media_manager_settings_, named_video_pipe_path())
        .WillOnce(ReturnRef(kNamedAudioPipePath));
    EXPECT_CALL(mock_media_manager_settings_, named_audio_pipe_path())
        .WillOnce(ReturnRef(kNamedAudioPipePath));
    EXPECT_CALL(mock_media_manager_settings_, app_storage_folder())
        .WillRepeatedly(ReturnRef(kStorageFolder));
    media_manager_impl_.reset(
        new MediaManagerImpl(app_mngr_, mock_media_manager_settings_));
  }

  void InitMediaManagerFileServerType() {
    InitMediaManagerPrecondition(kFileValue);
    EXPECT_CALL(mock_media_manager_settings_, video_stream_file())
        .WillOnce(ReturnRef(kVideoStreamFile));
    EXPECT_CALL(mock_media_manager_settings_, audio_stream_file())
        .WillOnce(ReturnRef(kAudioStreamFile));
    EXPECT_CALL(mock_media_manager_settings_, app_storage_folder())
        .WillRepeatedly(ReturnRef(kStorageFolder));
    media_manager_impl_.reset(
        new MediaManagerImpl(app_mngr_, mock_media_manager_settings_));
  }

  void ReceivedMessage(const ServiceType service_type) {
    EXPECT_CALL(app_mngr_, CanAppStream(kConnectionKey, service_type))
        .WillOnce(Return(true));
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, WakeUpStreaming(service_type));
    MockMediaAdapterImplPtr mock_media_streamer =
        utils::MakeShared<MockMediaAdapterImpl>();
    media_manager_impl_->set_mock_streamer(service_type, mock_media_streamer);
    media_manager_impl_->set_mock_streamer_listener(
        service_type, media_adapter_listener_mock_);
    EXPECT_CALL(*mock_media_streamer, SendData(kConnectionKey, _));
    EmulateMobileMessage(service_type);
  }

  void EmulateMobileMessage(const ServiceType serviceType) {
    const uint32_t data_sending_size = 3u;
    unsigned char data_sending[data_sending_size] = {0x20, 0x07, 0x01};
    const RawMessagePtr raw_message_ptr(
        new ::protocol_handler::RawMessage(kConnectionKey,
                                           kProtocolVersion,
                                           data_sending,
                                           data_sending_size,
                                           serviceType));
    media_manager_impl_->OnMessageReceived(raw_message_ptr);
    media_manager_impl_->OnMobileMessageSent(raw_message_ptr);
  }

  application_manager_test::MockApplicationManager app_mngr_;
  MockAppPtr mock_app_;
  SharedPtr<MockMediaAdapterListener> media_adapter_listener_mock_;
  MockMediaAdapter* media_adapter_mock_;
  const ::testing::NiceMock<MockMediaManagerSettings>
      mock_media_manager_settings_;
  SharedPtr<MediaManagerImpl> media_manager_impl_;
};

TEST_F(MediaManagerImplTest,
       OnMessageReceived_WithUnsupportedServiceType_Cancelled) {
  EXPECT_CALL(app_mngr_, CanAppStream(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EmulateMobileMessage(ServiceType::kInvalidServiceType);
}

TEST_F(MediaManagerImplTest,
       OnMessageReceived_WithAudioServiceType_ApplicationNotFound_Cancelled) {
  const ServiceType audio_type = ServiceType::kAudio;
  EXPECT_CALL(app_mngr_, CanAppStream(kConnectionKey, audio_type))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));
  EmulateMobileMessage(audio_type);
}

TEST_F(MediaManagerImplTest,
       OnMessageReceived_WithVideoServiceType_ApplicationNotFound_Cancelled) {
  const ServiceType video_type = ServiceType::kMobileNav;
  EXPECT_CALL(app_mngr_, CanAppStream(kConnectionKey, video_type))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));
  EmulateMobileMessage(video_type);
}

TEST_F(MediaManagerImplTest,
       OnMessageReceived_WithAudioServiceType_ForbidStreaming_Cancelled) {
  const ServiceType audio_type = ServiceType::kAudio;
  EXPECT_CALL(app_mngr_, CanAppStream(kConnectionKey, audio_type))
      .WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, ForbidStreaming(kConnectionKey));
  EmulateMobileMessage(audio_type);
}

TEST_F(MediaManagerImplTest,
       OnMessageReceived_WithVideoServiceType_ForbidStreaming_Cancelled) {
  const ServiceType video_type = ServiceType::kMobileNav;
  EXPECT_CALL(app_mngr_, CanAppStream(kConnectionKey, video_type))
      .WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, ForbidStreaming(kConnectionKey));
  EmulateMobileMessage(video_type);
}

TEST_F(MediaManagerImplTest, OnMessageReceived_WithAudioServiceType_SUCCESS) {
  ReceivedMessage(ServiceType::kAudio);
}

TEST_F(MediaManagerImplTest, OnMessageReceived_WithVideoServiceType_SUCCESS) {
  ReceivedMessage(ServiceType::kMobileNav);
}

TEST_F(MediaManagerImplTest, Init_Settings_ExpectSocketValue) {
  InitMediaManagerSocketServerType();
}

TEST_F(MediaManagerImplTest, Init_Settings_ExpectPipeValue) {
  InitMediaManagerPipeServerType();
}

TEST_F(MediaManagerImplTest, Init_Settings_ExpectFileValue) {
  InitMediaManagerFileServerType();
}

TEST_F(MediaManagerImplTest, PlayA2DPSource_WithCorrectA2DP_SUCCESS) {
  media_manager_impl_->set_mock_a2dp_player(media_adapter_mock_);
  EXPECT_CALL(*media_adapter_mock_, StartActivity(kApplicationKey));
  media_manager_impl_->PlayA2DPSource(kApplicationKey);
}

TEST_F(MediaManagerImplTest, StopA2DPSource_WithCorrectA2DP_SUCCESS) {
  media_manager_impl_->set_mock_a2dp_player(media_adapter_mock_);
  EXPECT_CALL(*media_adapter_mock_, StopActivity(kApplicationKey));
  media_manager_impl_->StopA2DPSource(kApplicationKey);
}

TEST_F(MediaManagerImplTest,
       StartMicrophoneRecording_SourceFileIsWritable_ExpectTrue) {
  StartMicrophoneCheckHelper();
  if (!DirectoryExists(kStorageFolder)) {
    CreateDirectory(kStorageFolder);
  }
  EXPECT_FALSE(FileExists(kOutputFilePath));
  EXPECT_TRUE(CreateFile(kOutputFilePath));

  const std::string record_file_path =
      kResourceFolder + "/" + kRecordingFileSource;
  if (!DirectoryExists(kResourceFolder)) {
    CreateDirectory(kResourceFolder);
  }
  EXPECT_FALSE(FileExists(record_file_path));
  EXPECT_TRUE(CreateFile(record_file_path));
  std::ofstream* test_file = Open(record_file_path);
  ScopeGuard file_guard = MakeGuard(dealloc_file, test_file);
  UNUSED(file_guard);
  EXPECT_TRUE(test_file->is_open());
  const uint32_t data_size = 4u;
  uint8_t* data = new uint8_t[data_size];
  ScopeGuard data_guard = MakeGuard(dealloc_data, data);
  UNUSED(data_guard);
  for (uint32_t i = 0u; i < data_size; ++i) {
    data[i] = i;
  }
  EXPECT_TRUE(Write(test_file, data, data_size));
  Close(test_file);
  EXPECT_FALSE(test_file->is_open());
  BinaryData result;
  EXPECT_TRUE(ReadBinaryFile(record_file_path, result));
  EXPECT_FALSE(result.empty());
  for (uint32_t i = 0u; i < data_size; ++i) {
    EXPECT_EQ(data[i], result[i]);
  }
  media_manager_impl_->StartMicrophoneRecording(
      kApplicationKey, kOutputFile, kDuration);
  EXPECT_TRUE(RemoveDirectory(kResourceFolder, true));
  EXPECT_TRUE(RemoveDirectory(kStorageFolder, true));
}

TEST_F(MediaManagerImplTest,
       StartMicrophoneRecording_OutputFileNotExists_ExpectFalse) {
  StartMicrophoneCheckHelper();
  media_manager_impl_->set_mock_mic_listener(media_adapter_listener_mock_);
  EXPECT_FALSE(FileExists(kOutputFilePath));
  media_manager_impl_->StartMicrophoneRecording(
      kApplicationKey, kOutputFile, kDuration);
}

TEST_F(MediaManagerImplTest,
       StartMicrophoneRecording_OutputFileCouldNotDeleted_ExpectTrue) {
  StartMicrophoneCheckHelper();
  if (!DirectoryExists(kStorageFolder)) {
    CreateDirectory(kStorageFolder);
  }
  EXPECT_FALSE(FileExists(kOutputFilePath));
  EXPECT_TRUE(CreateFile(kOutputFilePath));
  chmod(kOutputFilePath.c_str(), S_IRUSR);
  EXPECT_FALSE(DeleteFile(kOutputFilePath));
  media_manager_impl_->set_mock_mic_listener(media_adapter_listener_mock_);
  EXPECT_TRUE(FileExists(kOutputFilePath));
  media_manager_impl_->StartMicrophoneRecording(
      kApplicationKey, kOutputFile, kDuration);
  chmod(kOutputFilePath.c_str(), S_IWUSR);
  EXPECT_TRUE(RemoveDirectory(kStorageFolder, true));
}

TEST_F(MediaManagerImplTest, StopMicrophoneRecording_SUCCESS) {
  media_manager_impl_->set_mock_mic_listener(media_adapter_listener_mock_);
#ifdef EXTENDED_MEDIA_MODE
  MockMediaAdapterImpl* media_adapter_recorder_mock =
      new MockMediaAdapterImpl();
  media_manager_impl_->set_mock_mic_recorder(media_adapter_recorder_mock);
  EXPECT_CALL(*media_adapter_recorder_mock, StopActivity(kApplicationKey));
#endif  // EXTENDED_MEDIA_MODE
  EXPECT_CALL(*media_adapter_listener_mock_, OnActivityEnded(kApplicationKey));
#ifdef EXTENDED_MEDIA_MODE
  EXPECT_CALL(*media_adapter_recorder_mock, RemoveListener(_));
#endif  // EXTENDED_MEDIA_MODE
  media_manager_impl_->StopMicrophoneRecording(kApplicationKey);
}

TEST_F(MediaManagerImplTest,
       StartStopStreaming_AudioAndVideoServiceType_SUCCESS) {
  MockMediaAdapterImplPtr mock_audio_media_streamer =
      utils::MakeShared<MockMediaAdapterImpl>();
  media_manager_impl_->set_mock_streamer(ServiceType::kAudio,
                                         mock_audio_media_streamer);
  MockMediaAdapterImplPtr mock_nav_media_streamer =
      utils::MakeShared<MockMediaAdapterImpl>();
  media_manager_impl_->set_mock_streamer(ServiceType::kMobileNav,
                                         mock_nav_media_streamer);

  EXPECT_CALL(*mock_audio_media_streamer, StartActivity(kApplicationKey));
  media_manager_impl_->StartStreaming(kApplicationKey, ServiceType::kAudio);
  EXPECT_CALL(*mock_nav_media_streamer, StartActivity(kApplicationKey));
  media_manager_impl_->StartStreaming(kApplicationKey, ServiceType::kMobileNav);
  EXPECT_CALL(*mock_audio_media_streamer, StopActivity(kApplicationKey));
  media_manager_impl_->StopStreaming(kApplicationKey, ServiceType::kAudio);
  EXPECT_CALL(*mock_nav_media_streamer, StopActivity(kApplicationKey));
  media_manager_impl_->StopStreaming(kApplicationKey, ServiceType::kMobileNav);
}

TEST_F(MediaManagerImplTest,
       CheckFramesProcessed_WithCorrectFramesNumber_SUCCESS) {
  ON_CALL(mock_media_manager_settings_, video_server_type())
      .WillByDefault(ReturnRef(kDefaultValue));
  ON_CALL(mock_media_manager_settings_, audio_server_type())
      .WillByDefault(ReturnRef(kDefaultValue));
  protocol_handler_test::MockProtocolHandler mock_protocol_handler;
  media_manager_impl_->SetProtocolHandler(&mock_protocol_handler);
  const int32_t frame_number = 10;
  EXPECT_CALL(mock_protocol_handler,
              SendFramesNumber(kApplicationKey, frame_number));
  media_manager_impl_->FramesProcessed(kApplicationKey, frame_number);
}

}  // namespace media_manager_test
}  // namespace components
}  // namespace test
