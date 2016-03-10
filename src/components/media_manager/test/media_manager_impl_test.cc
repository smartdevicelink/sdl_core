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

#include "gmock/gmock.h"
#include "media_manager/media_manager_impl.h"
#include "include/media_adapter_mock.h"
#include "include/media_adapter_listener_mock.h"
#include "include/media_adapter_impl_mock.h"
#include "protocol_handler/mock_protocol_handler.h"

namespace test {
namespace components {
namespace media_manager_test {

using ::testing::_;
using protocol_handler::ServiceType;

TEST(MediaManagerImplTest, PlayA2DPSource) {
  MediaManagerImpl* mediaManagerImpl = MediaManagerImpl::instance();
  EXPECT_TRUE(mediaManagerImpl->exists());
  int32_t application_key = 1;

  MediaAdapterMock media_mock;
  mediaManagerImpl->set_mock_a2dp_player(&media_mock);
  EXPECT_CALL(media_mock, StartActivity(application_key));
  mediaManagerImpl->PlayA2DPSource(application_key);
}

TEST(MediaManagerImplTest, StopA2DPSource) {
  MediaManagerImpl* mediaManagerImpl = MediaManagerImpl::instance();
  EXPECT_TRUE(mediaManagerImpl->exists());
  int32_t application_key = 1;

  MediaAdapterMock media_mock;
  mediaManagerImpl->set_mock_a2dp_player(&media_mock);
  EXPECT_CALL(media_mock, StopActivity(application_key));
  mediaManagerImpl->StopA2DPSource(application_key);
}

TEST(MediaManagerImplTest, StopMicrophoneRecording) {
  MediaManagerImpl* mediaManagerImpl = MediaManagerImpl::instance();
  int32_t application_key = 1;
  MediaAdapterListenerMock media_adapter_listener_mock;

  mediaManagerImpl->set_mock_mic_listener(&media_adapter_listener_mock);
#ifdef EXTENDED_MEDIA_MODE
  MediaAdapterImplMock media_adapter_recorder_mock;
  mediaManagerImpl->set_mock_mic_recorder(&media_adapter_recorder_mock);
  EXPECT_CALL(media_adapter_recorder_mock, StopActivity(application_key));
#endif  // EXTENDED_MEDIA_MODE
  EXPECT_CALL(media_adapter_listener_mock, OnActivityEnded(application_key));
#ifdef EXTENDED_MEDIA_MODE
  EXPECT_CALL(media_adapter_recorder_mock, RemoveListener(_));
#endif  // EXTENDED_MEDIA_MODE
  mediaManagerImpl->StopMicrophoneRecording(application_key);
}

TEST(MediaManagerImplTest, StartStopStreaming) {
  MediaManagerImpl* mediaManagerImpl = MediaManagerImpl::instance();

  int32_t application_key = 1;
  MediaAdapterImplMock mock_audio_media_streamer;
  mediaManagerImpl->set_mock_streamer(ServiceType::kAudio,
                                      &mock_audio_media_streamer);
  MediaAdapterImplMock mock_nav_media_streamer;
  mediaManagerImpl->set_mock_streamer(ServiceType::kMobileNav,
                                      &mock_nav_media_streamer);

  EXPECT_CALL(mock_audio_media_streamer, StartActivity(application_key));
  mediaManagerImpl->StartStreaming(application_key, ServiceType::kAudio);

  EXPECT_CALL(mock_nav_media_streamer, StartActivity(application_key));
  mediaManagerImpl->StartStreaming(application_key, ServiceType::kMobileNav);

  EXPECT_CALL(mock_audio_media_streamer, StopActivity(application_key));
  mediaManagerImpl->StopStreaming(application_key, ServiceType::kAudio);

  EXPECT_CALL(mock_nav_media_streamer, StopActivity(application_key));
  mediaManagerImpl->StopStreaming(application_key, ServiceType::kMobileNav);
}

TEST(MediaManagerImplTest, CheckFramesProcessed) {
  MediaManagerImpl* mediaManagerImpl = MediaManagerImpl::instance();
  protocol_handler_test::MockProtocolHandler mock_protocol_handler;
  mediaManagerImpl->SetProtocolHandler(&mock_protocol_handler);
  int32_t application_key = 1;
  int32_t frame_number = 10;

  EXPECT_CALL(mock_protocol_handler,
              SendFramesNumber(application_key, frame_number));
  mediaManagerImpl->FramesProcessed(application_key, frame_number);
}

}  // namespace media_manager_test
}  // namespace components
}  // namespace test
