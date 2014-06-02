#include <gmock/gmock.h>

#include "usage_statistics/statistics_manager.h"

namespace usage_statistics {
namespace test {

class MockStatisticsManager: public StatisticsManager {
 public:
  MOCK_METHOD1(Increment, void(GlobalCounterId type));
  MOCK_METHOD2(Increment, void(const std::string& app_id, AppCounterId type));
  MOCK_METHOD3(Set, void(const std::string& app_id,
                         AppInfoId type,
                         const std::string& value));
  MOCK_METHOD3(Add, void(const std::string& app_id,
                         AppStopwatchId type,
                         int32_t timespan_seconds));

};

}  // namespace test
}  // namespace usage_statistics
