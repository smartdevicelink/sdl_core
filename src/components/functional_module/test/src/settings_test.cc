#include "gtest/gtest.h"
#include "functional_module/settings.h"

namespace functional_modules {

TEST(Settings, ChangeConfigFile) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  int port = 0;
  bool result = set.ReadParameter("HMI", "ServerPort", &port);
  ASSERT_TRUE(result);
  ASSERT_EQ(8087, port);
  set.ChangeConfigFile("not_existing_file.ini");
  result = set.ReadParameter("HMI", "ServerPort", &port);
  ASSERT_FALSE(result);
}

TEST(Settings, ReadParamIntWithSectionSuccess) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  int port = 0;
  bool result = set.ReadParameter<int>("HMI", "ServerPort", &port);
  ASSERT_TRUE(result);
  ASSERT_EQ(8087, port);
}

TEST(Settings, ReadParamIntWithSectionFail) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  int port = 0;
  bool result = set.ReadParameter<int>("HMI", "ServerPo", &port);
  ASSERT_FALSE(result);
  ASSERT_EQ(0, port);
}

TEST(Settings, ReadParamBoolWithSectionSuccess) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  bool launch = true;
  bool result = set.ReadParameter<bool>("HMI", "LaunchHMI", &launch);
  ASSERT_TRUE(result);
  ASSERT_FALSE(launch);
}

TEST(Settings, ReadParamBoolWithSectionFail) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  bool launch = true;
  bool result = set.ReadParameter<bool>("HMI", "LaunchH", &launch);
  ASSERT_FALSE(result);
  ASSERT_TRUE(launch);
}

TEST(Settings, ReadParamStringNoSectionSuccess) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  std::string caps;
  bool result = set.ReadParameter<std::string>("HMICapabilities", &caps);
  ASSERT_TRUE(result);
  ASSERT_EQ("hmi_capabilities.json", caps);
}

TEST(Settings, ReadParamStringNoSectionFail) {
  Settings set;
  set.ChangeConfigFile("test.ini");
  std::string caps;
  bool result = set.ReadParameter("HMICaps", &caps);
  ASSERT_FALSE(result);
  ASSERT_EQ("", caps);
}

}  // namespace functional_modules
