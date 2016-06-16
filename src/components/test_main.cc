#include "gmock/gmock.h"
#include "utils/logger.h"
#include "utils/custom_string.h"
namespace custom_str = utils::custom_string;
int main(int argc, char** argv) {
  testing::InitGoogleMock(&argc, argv);
  ::testing::DefaultValue<custom_str::CustomString>::Set(
      custom_str::CustomString(""));
  const int result = RUN_ALL_TESTS();
  DEINIT_LOGGER();
  return result;
}
