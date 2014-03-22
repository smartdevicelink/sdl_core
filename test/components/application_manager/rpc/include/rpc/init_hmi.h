/**
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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_INIT_HMI_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_INIT_HMI_H_

#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/signals.h"
#include "config_profile/profile.h"
#include "networking.h"
#include "application_manager/message_helper.h"
#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread_options.h"
#include "utils/threads/thread.h"

namespace InitializeHMI {
  /**
   * @brief Patch to browser
   */
  const char kBrowser[] = "/usr/bin/chromium-browser";
  /**
   * @brief Browser name
   */
  const char kBrowserName[] = "chromium-browser";
  /**
   * @brief Browser params
   */
  const char kBrowserParams[] = "--auth-schemes=basic,digest,ntlm";
  /**
   * @brief Local host address
   */
  const char kLocalHostAddress[] = "127.0.0.1";
  /**
   * @brief Initialize HTML based HMI.
   *
   * @return true if success otherwise false.
   */
  // bool InitHmi();

  class InitHMI : public threads::ThreadDelegate {
   public:
    InitHMI();

    ~InitHMI();

    virtual void threadMain();

   private:
    DISALLOW_COPY_AND_ASSIGN(InitHMI);
  };

  bool InitFuncHmi();

  bool InitHmi();

}  // namespace InitializeHMI

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_INIT_HMI_H_
