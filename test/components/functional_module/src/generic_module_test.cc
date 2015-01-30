#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "driver_generic_module.h"

#include "mock_module_observer.h"
#include "mock_service.h"

using application_manager::ServicePtr;
using functional_modules::ModuleObserver;
using functional_modules::DriverGenericModule;

using application_manager::MockService;
using functional_modules::MockModuleObserver;

using ::testing::_;

namespace test {
namespace components {
namespace functional_module {

TEST(GenericModuleTest, SetService) {
  DriverGenericModule module(18);
  MockService* mock_service = new MockService();
  ServicePtr exp_service(mock_service);

  EXPECT_CALL(*mock_service, SubscribeToHMINotification(_)).Times(1);

  module.SetServiceHandler(exp_service);
  ServicePtr out_service = module.GetServiceHandler();

  EXPECT_EQ(exp_service.get(), out_service.get());
}

TEST(GenericModuleTest, AddObserver) {
  DriverGenericModule module(18);
  utils::SharedPtr<MockModuleObserver> observer(new MockModuleObserver());
  module.AddObserver(observer);
  const DriverGenericModule::Observers& full = module.observers();
  ASSERT_EQ(1, full.size());
  EXPECT_EQ(observer.get(), full[0].get());
}

TEST(GenericModuleTest, RemoveObserver) {
  DriverGenericModule module(18);
  utils::SharedPtr<MockModuleObserver> observer(new MockModuleObserver());
  module.AddObserver(observer);

  module.RemoveObserver(observer);
  const DriverGenericModule::Observers& empty = module.observers();
  ASSERT_TRUE(empty.empty());
}

TEST(GenericModuleTest, EmptyRemoveObserver) {
  DriverGenericModule module(18);
  utils::SharedPtr<MockModuleObserver> observer(new MockModuleObserver());
  module.RemoveObserver(observer);
  const DriverGenericModule::Observers& empty = module.observers();
  ASSERT_TRUE(empty.empty());
}

TEST(GenericModuleTest, WrongRemoveObserver) {
  DriverGenericModule module(18);
  utils::SharedPtr<MockModuleObserver> observer(new MockModuleObserver());
  module.AddObserver(observer);

  utils::SharedPtr<MockModuleObserver> wrong_observer(new MockModuleObserver());
  module.RemoveObserver(wrong_observer);
  const DriverGenericModule::Observers& empty = module.observers();
  ASSERT_EQ(1, empty.size());
  EXPECT_EQ(observer.get(), empty[0].get());
}

TEST(GenericModuleTest, NotifyObservers) {
  DriverGenericModule module(3);
  MockModuleObserver* observer = new MockModuleObserver();
  utils::SharedPtr<MockModuleObserver> mock_observer(observer);
  module.AddObserver(mock_observer);

  EXPECT_CALL(*observer, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  application_manager::MessagePtr message;
  module.ProcessMessage(message);
}

TEST(GenericModuleTest, NotifyObserversComplex) {
  DriverGenericModule module(3);
  MockModuleObserver* observer_1 = new MockModuleObserver();
  utils::SharedPtr<MockModuleObserver> mock_observer_1(observer_1);
  module.AddObserver(mock_observer_1);
  MockModuleObserver* observer_2 = new MockModuleObserver();
  utils::SharedPtr<MockModuleObserver> mock_observer_2(observer_2);
  module.AddObserver(mock_observer_2);

  EXPECT_CALL(*observer_1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);
  EXPECT_CALL(*observer_2, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  application_manager::MessagePtr message;
  module.ProcessMessage(message);

  module.RemoveObserver(mock_observer_1);
  EXPECT_CALL(*observer_1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(0);
  EXPECT_CALL(*observer_2, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  module.ProcessMessage(message);
}

}  // namespace functional_module
}  // namespace components
}  // namespace test
