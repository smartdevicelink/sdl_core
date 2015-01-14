/*
 * Copyright (c) 2013, Ford Motor Company
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

#include "resumption/last_state.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace resumption {

CREATE_LOGGERPTR_GLOBAL(logger_, "LastState");

void LastState::SaveToFileSystem() {
  LOG4CXX_INFO(logger_, "LastState::SaveToFileSystem");
  const std::string file =
      profile::Profile::instance()->app_info_storage();
  const std::string& str = dictionary.toStyledString();
  const std::vector<uint8_t> char_vector_pdata(
    str.begin(), str.end());

  DCHECK(file_system::CreateDirectoryRecursively(
        profile::Profile::instance()->app_storage_folder()));

  LOG4CXX_INFO(logger_, "LastState::SaveToFileSystem " << file);
  DCHECK(file_system::Write(file, char_vector_pdata));
}

void LastState::LoadFromFileSystem() {
  const std::string file =
      profile::Profile::instance()->app_info_storage();
  std::string buffer;
  bool result = file_system::ReadFile(file, buffer);
  Json::Reader m_reader;
  if (result && m_reader.parse(buffer, dictionary)) {
    LOG4CXX_INFO(logger_, "Valid last state was found.");
    return;
  }
  LOG4CXX_WARN(logger_, "No valid last state was found.");
}

LastState::LastState() {
  LoadFromFileSystem();
}

LastState::~LastState() {
  SaveToFileSystem();
}

}
