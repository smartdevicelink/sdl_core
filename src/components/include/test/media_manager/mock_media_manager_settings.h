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
#ifndef SRC_COMPONENTS_INCLUDE__TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE__TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_SETTINGS_H_

#include <gmock/gmock.h>
#include <string>
#include "media_manager/media_manager_settings.h"

namespace test {
namespace components {
namespace media_manager_test {



class MockMediaManagerSettings : public ::media_manager::MediaManagerSettings {
 public:
  MOCK_CONST_METHOD0(video_server_type, const std::string&());
  MOCK_CONST_METHOD0(audio_server_type, const std::string&());
  MOCK_CONST_METHOD0(server_address, const std::string&());
  MOCK_CONST_METHOD0(video_streaming_port, const std::uint16_t());
  MOCK_CONST_METHOD0(audio_streaming_port, const std::uint16_t());
  MOCK_CONST_METHOD0(named_video_pipe_path, const std::string&());
  MOCK_CONST_METHOD0(named_audio_pipe_path, const std::string&());
  MOCK_CONST_METHOD0(video_stream_file, const std::string&());
  MOCK_CONST_METHOD0(audio_stream_file, const std::string&());
  MOCK_CONST_METHOD0(app_storage_folder, const std::string&());
  MOCK_CONST_METHOD0(app_resource_folder, const std::string&());
  MOCK_CONST_METHOD0(recording_file_source, const std::string&());
};

}  // namespace media_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE__TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_SETTINGS_H_
