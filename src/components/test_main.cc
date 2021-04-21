#include <iostream>
#include "gmock/gmock.h"
#include "utils/custom_string.h"

#ifdef ENABLE_LOG
#include "utils/logger/log4cxxlogger.h"
#include "utils/logger/logger_impl.h"
#endif  // ENABLE_LOG

#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("SDLMain")
int main(int argc, char** argv) {
#ifdef ENABLE_LOG
  // --------------------------------------------------------------------------
  // Logger initialization
  // Redefine for each paticular logger implementation
  auto logger = std::unique_ptr<logger::Log4CXXLogger>(
      new logger::Log4CXXLogger("log4cxx.properties"));
  auto logger_impl =
      std::unique_ptr<logger::LoggerImpl>(new logger::LoggerImpl(false));
  logger::Logger::instance(logger_impl.get());
#endif  // ENABLE_LOG

  namespace custom_str = utils::custom_string;
  testing::InitGoogleMock(&argc, argv);
  ::testing::DefaultValue<custom_str::CustomString>::Set(
      custom_str::CustomString(""));
  const int result = RUN_ALL_TESTS();

  SDL_DEINIT_LOGGER();
  return result;
}
