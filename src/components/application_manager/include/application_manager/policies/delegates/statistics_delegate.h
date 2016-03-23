/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_STATISTICS_DELEGATE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_STATISTICS_DELEGATE_H_

#include <string>

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"
#include "application_manager/usage_statistics.h"

namespace policy {

class PolicyHandler;

  class StatisticsDelegate: public threads::ThreadDelegate {
      enum StatisticType{
        INCREMENT_GLOBAL,
        INCREMENT_APP,
        SET,
        ADD
      };
    public:
      StatisticsDelegate(PolicyHandler& policy_handler,
                                  usage_statistics::GlobalCounterId type);

      StatisticsDelegate(PolicyHandler& policy_handler,
                         const std::string& app_id,
                        usage_statistics::AppCounterId type);

      StatisticsDelegate(PolicyHandler &policy_handler, const std::string& app_id,
                        usage_statistics::AppInfoId type,
                        const std::string& value);

      StatisticsDelegate(PolicyHandler& policy_handler,
                         const std::string& app_id,
                        usage_statistics::AppStopwatchId type,
                        int32_t timespan_seconds);

      virtual void threadMain();

      virtual void exitThreadMain();
    private:
      StatisticType type_;
      usage_statistics::GlobalCounterId global_counter_;
      usage_statistics::AppCounterId app_counter_;
      usage_statistics::AppInfoId app_info_;
      usage_statistics::AppStopwatchId stop_watch_;

      std::string app_id_;
      std::string value_;
      int32_t timespan_seconds_;
      PolicyHandler& policy_handler_;
  };
} // namespace policy

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DELEGATES_STATISTICS_DELEGATE_H_
