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

#ifndef TEST_COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_MOCK_H_
#define TEST_COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_MOCK_H_

#include <gmock/gmock.h>
#include "time_manager.h"
#include "metric_wrapper.h"
namespace test {
namespace components {
namespace time_tester_test {

using namespace time_tester;
/*
 * MOCK implementation of ::security_manager::SecurityManager
 */
class TimeManagerMock : public time_tester::TimeManager {
 public:
  MOCK_METHOD1(Init, void(protocol_handler::ProtocolHandlerImpl* ph));
  MOCK_METHOD0(Stop, void());
  MOCK_METHOD0(Start, void());
  MOCK_METHOD1(SendMetric, void(utils::SharedPtr<MetricWrapper> metric));
};
}  // time_tester_test
}  // components
}  // test
#endif  // TEST_COMPONENTS_TIME_MANAGER_INCLUDE_TIME_MANAGER_MOCK_H_
