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

#ifndef SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_IMPL_H_
#define SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_IMPL_H_

#include "resumption/last_state.h"
#include "utils/macro.h"

namespace resumption {

/**
 * @brief The LastStateImpl class handles interface of last state
 */

class LastStateImpl : public LastState {
 public:
  /**
   * @brief Constructor
   */
  LastStateImpl(const std::string& app_storage_folder,
                const std::string& app_info_storage);

  /**
   * @brief Destructor
   */
  ~LastStateImpl();

  /**
   * @brief Saving dictionary to filesystem
   */
  void SaveStateToFileSystem() OVERRIDE;

  /**
   * @brief Get reference to dictionary
   */
  Json::Value& get_dictionary() OVERRIDE;

 private:
  const std::string app_storage_folder_;
  const std::string app_info_storage_;
  Json::Value dictionary_;

  /**
   * @brief Load dictionary from filesystem
   */
  void LoadStateFromFileSystem();

  DISALLOW_COPY_AND_ASSIGN(LastStateImpl);
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_IMPL_H_
