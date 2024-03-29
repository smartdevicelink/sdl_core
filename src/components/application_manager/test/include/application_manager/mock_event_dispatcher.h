/*
 Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_DISPATCHER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_DISPATCHER_H_

#include <list>
#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/event_engine/event_observer.h"
#include "gmock/gmock.h"

namespace test {
namespace components {
namespace event_engine_test {

class MockEventDispatcher
    : public ::application_manager::event_engine::EventDispatcher {
 public:
  MOCK_METHOD1(raise_event,
               void(const ::application_manager::event_engine::Event& event));
  MOCK_METHOD3(
      add_observer,
      void(const ::application_manager::event_engine::Event::EventID& event_id,
           int32_t hmi_correlation_id,
           ::application_manager::event_engine::EventObserver& observer));
  MOCK_METHOD2(
      remove_observer,
      void(const ::application_manager::event_engine::Event::EventID& event_id,
           ::application_manager::event_engine::EventObserver& observer));
  MOCK_METHOD2(
      remove_observer,
      void(const ::application_manager::event_engine::Event::EventID& event_id,
           int32_t hmi_correlation_id));
  MOCK_METHOD1(
      remove_observer,
      void(::application_manager::event_engine::EventObserver& observer));

  MOCK_METHOD1(
      raise_mobile_event,
      void(const ::application_manager::event_engine::MobileEvent& event));
  MOCK_METHOD3(
      add_mobile_observer,
      void(
          const ::application_manager::event_engine::MobileEvent::MobileEventID&
              event_id,
          int32_t mobile_correlation_id,
          ::application_manager::event_engine::EventObserver& observer));
  MOCK_METHOD2(
      remove_mobile_observer,
      void(
          const ::application_manager::event_engine::MobileEvent::MobileEventID&
              event_id,
          ::application_manager::event_engine::EventObserver& observer));
  MOCK_METHOD1(
      remove_mobile_observer,
      void(::application_manager::event_engine::EventObserver& observer));
};

}  // namespace event_engine_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_EVENT_DISPATCHER_H_
