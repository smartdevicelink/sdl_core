/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_TEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_TEST_H_

#include <stdint.h>
#include "gtest/gtest.h"

#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/command.h"
#include "utils/make_shared.h"
#include "application_manager/mock_application_manager.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {

namespace am = ::application_manager;

using ::testing::ReturnRef;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;

using ::utils::SharedPtr;
using ::smart_objects::SmartObject;
using am::commands::MessageSharedPtr;
using ::test::components::application_manager_test::MockApplicationManager;
using ::test::components::application_manager_test::
    MockApplicationManagerSettings;
using am::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

// Depending on the value type will be selected
template <const bool kIf, class ThenT, class ElseT>
struct TypeIf {
  typedef ThenT Result;
};
template <class ThenT, class ElseT>
struct TypeIf<false, ThenT, ElseT> {
  typedef ElseT Result;
};

template <class MockT, bool kIsNice>
struct IsNiceMock : public TypeIf<kIsNice, ::testing::NiceMock<MockT>, MockT> {
};

#define MOCK(Type) typename IsNiceMock<Type, kMocksAreNice>::Result

// If `kIsNice` is `true` then all used mock types
// will be wrapped by a `NiceMock`

enum CommandsTestMocks { kNotNice = 0, kIsNice };

template <const CommandsTestMocks kIsNice = CommandsTestMocks::kNotNice>
class CommandsTest : public ::testing::Test {
 public:
  enum { kMocksAreNice = kIsNice };

  typedef NiceMock<MockApplicationManagerSettings> MockAppManagerSettings;
  typedef typename TypeIf<kIsNice,
                          NiceMock<MockApplicationManager>,
                          MockApplicationManager>::Result MockAppManager;
  typedef typename TypeIf<kIsNice,
                          NiceMock<MockApplication>,
                          MockApplication>::Result MockApp;
  typedef SharedPtr<MockApp> MockAppPtr;

  virtual ~CommandsTest() {}

  static MessageSharedPtr CreateMessage(
      const smart_objects::SmartType type = smart_objects::SmartType_Null) {
    return ::utils::MakeShared<SmartObject>(type);
  }

  static MockAppPtr CreateMockApp() {
    return ::utils::MakeShared<MockApp>();
  }

  template <class Command>
  SharedPtr<Command> CreateCommand(const uint32_t timeout,
                                   MessageSharedPtr& msg) {
    InitCommand(timeout);
    return ::utils::MakeShared<Command>((msg ? msg : msg = CreateMessage()),
                                        app_mngr_);
  }

  template <class Command>
  SharedPtr<Command> CreateCommand(MessageSharedPtr& msg) {
    return CreateCommand<Command>(kDefaultTimeout_, msg);
  }

  template <class Command>
  SharedPtr<Command> CreateCommand(const uint32_t timeout = kDefaultTimeout_) {
    InitCommand(timeout);
    MessageSharedPtr msg = CreateMessage();
    return ::utils::MakeShared<Command>(msg, app_mngr_);
  }

  enum { kDefaultTimeout_ = 100 };

  MockAppManager app_mngr_;
  MockAppManagerSettings app_mngr_settings_;
  MOCK(am::MockHmiInterfaces) mock_hmi_interfaces_;

 protected:
  virtual void InitCommand(const uint32_t& timeout) {
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, default_timeout())
        .WillByDefault(ReturnRef(timeout));
  }

  CommandsTest() {
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_SDL));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }
};

}  // namespace commands_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMANDS_TEST_H_
