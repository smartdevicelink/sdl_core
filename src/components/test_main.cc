#include "gmock/gmock.h"
#include "utils/logger.h"

int main(int argc, char** argv) {
   testing::InitGoogleMock(&argc, argv);
   const int result = RUN_ALL_TESTS();
   DEINIT_LOGGER();
   return result;
}

