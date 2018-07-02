#include "gmock/gmock.h"
#include "utils/logger.h"
#include "utils/custom_string.h"
#include <iostream>

CREATE_LOGGERPTR_GLOBAL(logger_, "SDLMain")
int main(int argc, char** argv) {
  namespace custom_str = utils::custom_string;
  testing::InitGoogleMock(&argc, argv);
  ::testing::DefaultValue<custom_str::CustomString>::Set(
      custom_str::CustomString(""));
  const int result = RUN_ALL_TESTS();

  DEINIT_LOGGER();
  return result;
}
