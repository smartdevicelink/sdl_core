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

#include "application_manager/policies/delegates/statistics_delegate.h"
#include "application_manager/policies/policy_handler.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")
namespace policy {
StatisticsDelegate::StatisticsDelegate(PolicyHandler& policy_handler,
                                       usage_statistics::GlobalCounterId type)
    : type_(INCREMENT_GLOBAL)
    , global_counter_(type)
    , app_id_("")
    , value_("")
    , timespan_seconds_(0)
    , policy_handler_(policy_handler) {}

StatisticsDelegate::StatisticsDelegate(PolicyHandler& policy_handler,
                                       const std::string& app_id,
                                       usage_statistics::AppCounterId type)
    : type_(INCREMENT_APP)
    , app_counter_(type)
    , app_id_(app_id)
    , value_("")
    , timespan_seconds_(0)
    , policy_handler_(policy_handler) {}

StatisticsDelegate::StatisticsDelegate(PolicyHandler& policy_handler,
                                       const std::string& app_id,
                                       usage_statistics::AppInfoId type,
                                       const std::string& value)
    : type_(SET)
    , app_info_(type)
    , app_id_(app_id)
    , value_(value)
    , timespan_seconds_(0)
    , policy_handler_(policy_handler) {}

StatisticsDelegate::StatisticsDelegate(PolicyHandler& policy_handler,
                                       const std::string& app_id,
                                       usage_statistics::AppStopwatchId type,
                                       int32_t timespan_seconds)
    : type_(ADD)
    , stop_watch_(type)
    , app_id_(app_id)
    , value_("")
    , timespan_seconds_(timespan_seconds)
    , policy_handler_(policy_handler) {}

  void StatisticsDelegate::threadMain() {
    LOG4CXX_AUTO_TRACE(logger_);
    switch (type_) {
      case INCREMENT_GLOBAL:
        policy_handler_.Increment(global_counter_);
        break;
      case INCREMENT_APP:
        policy_handler_.Increment(app_id_, app_counter_);
        break;
      case SET:
        policy_handler_.Set(app_id_, app_info_, value_);
        break;
      case ADD:
        policy_handler_.Add(app_id_, stop_watch_, timespan_seconds_);
        break;
      default:
        LOG4CXX_ERROR(logger_,"Unknown statistics operator");
        break;
    }
  }

  void StatisticsDelegate::exitThreadMain() {
    // Do nothing
  }
} // namespace policy
