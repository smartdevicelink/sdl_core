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

#include "resumption/last_state_impl.h"
#include "utils/file_system.h"
#include "utils/jsoncpp_reader_wrapper.h"
#include "utils/logger.h"

namespace resumption {

SDL_CREATE_LOG_VARIABLE("Resumption")

LastStateImpl::LastStateImpl(const std::string& app_storage_folder,
                             const std::string& app_info_storage)
    : app_storage_folder_(app_storage_folder)
    , app_info_storage_(app_info_storage) {
  LoadFromFileSystem();
  SDL_LOG_AUTO_TRACE();
}

LastStateImpl::~LastStateImpl() {
  SDL_LOG_AUTO_TRACE();
  SaveToFileSystem();
}

void LastStateImpl::SaveStateToFileSystem() {
  SDL_LOG_AUTO_TRACE();

  std::string styled_string;
  {
    sync_primitives::AutoLock lock(dictionary_lock_);
    styled_string = dictionary_.toStyledString();
  }

  const std::string full_path =
      !app_storage_folder_.empty()
          ? app_storage_folder_ + "/" + app_info_storage_
          : app_info_storage_;

  const std::vector<uint8_t> char_vector_pdata(styled_string.begin(),
                                               styled_string.end());
  DCHECK(file_system::CreateDirectoryRecursively(app_storage_folder_));
  SDL_LOG_INFO("LastState::SaveStateToFileSystem[DEPRECATED] "
               << full_path << styled_string);
  DCHECK(file_system::Write(full_path, char_vector_pdata));
}

void LastStateImpl::SaveToFileSystem() {
  SDL_LOG_AUTO_TRACE();

  std::string styled_string;
  {
    sync_primitives::AutoLock lock(dictionary_lock_);
    styled_string = dictionary_.toStyledString();
  }

  const std::string full_path =
      !app_storage_folder_.empty()
          ? app_storage_folder_ + "/" + app_info_storage_
          : app_info_storage_;

  const std::vector<uint8_t> char_vector_pdata(styled_string.begin(),
                                               styled_string.end());
  DCHECK(file_system::CreateDirectoryRecursively(app_storage_folder_));
  SDL_LOG_INFO("LastState::SaveToFileSystem " << app_info_storage_
                                              << full_path);
  DCHECK(file_system::Write(full_path, char_vector_pdata));
}

void LastStateImpl::LoadFromFileSystem() {
  const std::string full_path =
      !app_storage_folder_.empty()
          ? app_storage_folder_ + "/" + app_info_storage_
          : app_info_storage_;
  std::string buffer;
  const bool result = file_system::ReadFile(full_path, buffer);
  utils::JsonReader reader;

  if (result && reader.parse(buffer, &dictionary_)) {
    SDL_LOG_INFO("Valid last state was found." << dictionary_.toStyledString());
    return;
  }
  SDL_LOG_WARN("No valid last state was found.");
}

void LastStateImpl::RemoveFromFileSystem() {
  SDL_LOG_AUTO_TRACE();
  if (!file_system::DeleteFile(app_info_storage_)) {
    SDL_LOG_WARN("Failed attempt to delete " << app_info_storage_);
  }
}

Json::Value LastStateImpl::dictionary() const {
  sync_primitives::AutoLock lock(dictionary_lock_);
  return dictionary_;
}

Json::Value& LastStateImpl::get_dictionary() {
  sync_primitives::AutoLock lock(dictionary_lock_);
  return dictionary_;
}

void LastStateImpl::set_dictionary(const Json::Value& dictionary) {
  DCHECK(dictionary.type() == Json::objectValue ||
         dictionary.type() == Json::nullValue);
  sync_primitives::AutoLock lock(dictionary_lock_);
  dictionary_ = dictionary;
}

}  // namespace resumption
