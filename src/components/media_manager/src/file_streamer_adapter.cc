/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "utils/logger.h"
#include "utils/file_system.h"
#include "media_manager/file_streamer_adapter.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "FileStreamerAdapter")

FileStreamerAdapter::FileStreamerAdapter(
    const std::string& file_name, const std::string& app_storage_folder)
  : StreamerAdapter(new FileStreamer(this, file_name, app_storage_folder)) {
}

FileStreamerAdapter::~FileStreamerAdapter() {
}

FileStreamerAdapter::FileStreamer::FileStreamer(
    FileStreamerAdapter* const adapter,
    const std::string& file_name,
    const std::string& app_storage_folder)
  : Streamer(adapter),
    file_name_(file_name),
    app_storage_folder_(app_storage_folder),
    file_stream_(NULL) {
}

FileStreamerAdapter::FileStreamer::~FileStreamer() {
}

bool FileStreamerAdapter::FileStreamer::Connect() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!file_system::CreateDirectoryRecursively(app_storage_folder_)) {
    LOG4CXX_ERROR(logger_, "Cannot create app folder");
    return false;
  }

  file_stream_ = file_system::Open(file_name_);
  if (!file_stream_) {
    LOG4CXX_ERROR(logger_, "Cannot open file stream "
                  << file_name_);
    return false;
  }

  LOG4CXX_INFO(logger_, "File " << file_name_
                << " was successfuly opened");
  return true;
}

void FileStreamerAdapter::FileStreamer::Disconnect() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (file_stream_) {
    file_system::Close(file_stream_);
    delete file_stream_;
    file_stream_ = NULL;
  }
  file_system::DeleteFile(file_name_);
}

bool FileStreamerAdapter::FileStreamer::Send(
    protocol_handler::RawMessagePtr msg) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!file_stream_) {
    LOG4CXX_ERROR(logger_, "File stream not found "
                  << file_name_);
    return false;
  }

  if (!file_system::Write(file_stream_, msg->data(),
                          msg->data_size())) {
    LOG4CXX_ERROR(logger_, "Failed writing data to file "
                  << file_name_);
    return false;
  }

  LOG4CXX_INFO(logger_, "Streamer::sent " << msg->data_size());
  return true;
}

}  // namespace media_manager
