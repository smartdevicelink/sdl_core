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

#ifndef SRC_COMPONENTS_TIME_TESTER_INCLUDE_TIME_TESTER_PROTOCOL_HANDLER_OBSERVER_H_
#define SRC_COMPONENTS_TIME_TESTER_INCLUDE_TIME_TESTER_PROTOCOL_HANDLER_OBSERVER_H_

#include "protocol_handler/time_metric_observer.h"
#include "utils/message_queue.h"

namespace time_tester {

class TimeManager;

class ProtocolHandlerObserver: public protocol_handler::PHMetricObserver {
 public:
  explicit ProtocolHandlerObserver(TimeManager* time_manager);

  virtual void StartMessageProcess(uint32_t message_id, const TimevalStruct& start_time);

  virtual void EndMessageProcess(utils::SharedPtr<MessageMetric> m);

 private:
  TimeManager* time_manager_;
  std::map<uint32_t, TimevalStruct> time_starts;
};
}  // namespace time_tester
#endif  // SRC_COMPONENTS_TIME_TESTER_INCLUDE_TIME_TESTER_PROTOCOL_HANDLER_OBSERVER_H_
