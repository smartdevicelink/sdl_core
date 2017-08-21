#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "driver_generic_module_test.h"

#include "mock_module_observer.h"
#include "mock_service.h"

using application_manager::ServicePtr;
using application_manager::MockService;

using ::testing::_;

namespace functional_modules {

TEST(GenericModuleTest, SetService) {
  DriverGenericModuleTest module(18);
  MockService* mock_service = new MockService();
  ServicePtr exp_service(mock_service);

  module.set_service(exp_service);
  ServicePtr out_service = module.service();

  EXPECT_EQ(exp_service.get(), out_service.get());
}

TEST(GenericModuleTest, RemoveObserver) {
  DriverGenericModuleTest module(18);
  MockModuleObserver* observer = new MockModuleObserver();
  module.AddObserver(observer);

  module.RemoveObserver(observer);
  const DriverGenericModuleTest::Observers& empty = module.observers();
  ASSERT_TRUE(empty.empty());
  delete observer;
}

TEST(GenericModuleTest, EmptyRemoveObserver) {
  DriverGenericModuleTest module(18);
  MockModuleObserver* observer = new MockModuleObserver();
  module.RemoveObserver(observer);
  const DriverGenericModuleTest::Observers& empty = module.observers();
  ASSERT_TRUE(empty.empty());
  delete observer;
}

TEST(GenericModuleTest, WrongRemoveObserver) {
  DriverGenericModuleTest module(18);
  MockModuleObserver observer;
  module.AddObserver(&observer);

  MockModuleObserver* wrong_observer = new MockModuleObserver();
  module.RemoveObserver(wrong_observer);
  const DriverGenericModuleTest::Observers& empty = module.observers();
  ASSERT_EQ(1u, empty.size());
  EXPECT_EQ(&observer, empty[0]);
  delete wrong_observer;
}

TEST(GenericModuleTest, CrashRemovedObserver) {
  DriverGenericModuleTest module(18);
  MockModuleObserver* observer = new MockModuleObserver();
  module.AddObserver(observer);
  EXPECT_EQ(13, observer->ObserverMethod());
  module.RemoveObserver(observer);
  EXPECT_EQ(13, observer->ObserverMethod());
  delete observer;
  MockModuleObserver second_observer;
  module.AddObserver(&second_observer);
  EXPECT_EQ(13, second_observer.ObserverMethod());
  module.RemoveObserver(&second_observer);
  EXPECT_EQ(13, second_observer.ObserverMethod());
}

TEST(GenericModuleTest, NotifyObservers) {
  DriverGenericModuleTest module(3);
  MockModuleObserver observer;
  module.AddObserver(&observer);

  EXPECT_CALL(observer, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  application_manager::MessagePtr message;
  module.ProcessMessage(message);
}

TEST(GenericModuleTest, NotifyObserversComplex) {
  DriverGenericModuleTest module(3);
  MockModuleObserver observer_1;
  module.AddObserver(&observer_1);
  MockModuleObserver observer_2;
  module.AddObserver(&observer_2);

  EXPECT_CALL(observer_1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);
  EXPECT_CALL(observer_2, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  application_manager::MessagePtr message;
  module.ProcessMessage(message);

  module.RemoveObserver(&observer_1);
  EXPECT_CALL(observer_1, OnError(ModuleObserver::FS_FAILURE, 3)).Times(0);
  EXPECT_CALL(observer_2, OnError(ModuleObserver::FS_FAILURE, 3)).Times(1);

  module.ProcessMessage(message);
}

}  // namespace functional_modules
