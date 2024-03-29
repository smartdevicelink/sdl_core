/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_OBSERVER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_OBSERVER_H_

#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_observer.h"
#include "gmock/gmock.h"

namespace test {
namespace components {
namespace event_engine_test {

class MockEventObserver
    : public application_manager::event_engine::EventObserver {
 public:
  MockEventObserver(
      ::application_manager::event_engine::EventDispatcher& event_dispatcher)
      : application_manager::event_engine::EventObserver(event_dispatcher) {}
  MOCK_METHOD1(on_event,
               void(const application_manager::event_engine::Event& event));
  MOCK_METHOD1(
      HandleOnEvent,
      void(const application_manager::event_engine::MobileEvent& event));
  MOCK_METHOD1(HandleOnEvent,
               void(const application_manager::event_engine::Event& event));
};

}  // namespace event_engine_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_OBSERVER_H_
