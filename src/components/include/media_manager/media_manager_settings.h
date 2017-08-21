/*
 * Copyright (c) 2016 Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_SETTINGS_H_
#include <string>
#include <stdint.h>

namespace media_manager {

/**
 * \class MediaManagerSettings
 * \brief Interface for media manager component settings.
 */
class MediaManagerSettings {
 public:
  virtual ~MediaManagerSettings() {}

  virtual const std::string& video_server_type() const = 0;
  virtual const std::string& audio_server_type() const = 0;
  virtual const std::string& server_address() const = 0;
  virtual const uint16_t video_streaming_port() const = 0;
  virtual const uint16_t audio_streaming_port() const = 0;
  virtual const std::string& named_video_pipe_path() const = 0;
  virtual const std::string& named_audio_pipe_path() const = 0;
  virtual const std::string& video_stream_file() const = 0;
  virtual const std::string& audio_stream_file() const = 0;
  virtual const std::string& app_storage_folder() const = 0;
  virtual const std::string& app_resource_folder() const = 0;
  virtual const std::string& recording_file_source() const = 0;
};

}  // namespace media_manager
#endif  // SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_SETTINGS_H_
