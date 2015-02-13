#include "utils/macro.h"
#include "functional_module/module_timer.h"

namespace functional_modules {

class TestTrackable : public Trackable {
 public:
  explicit TestTrackable(TimeUnit interval = 0)
    : Trackable()
    , custorm_interval_(interval) {}

  virtual TimeUnit custorm_interval() const {
    return custorm_interval_;
  }

  bool operator==(const TestTrackable& other) const {
    return custorm_interval_ == other.custorm_interval_;
  }
 private:
  TimeUnit custorm_interval_;
};

class ModuleTimerTest {
 public:
  ModuleTimerTest(ModuleTimer<TestTrackable>& timer)
    : timer_(timer) {}

  TimeUnit period() const {
    return timer_.period_;
  }

  int observers_size() const {
    return timer_.observers_.size();
  }

  int trackables_size() const {
    return timer_.trackables_.size();
  }

  TestTrackable trackable(const TestTrackable& track) const {
    return *std::find(timer_.trackables_.begin(),
                      timer_.trackables_.end(), track);
  }

  TimeUnit current_time() const {
    return timer_.CurrentTime();
  }

 private:
  ModuleTimer<TestTrackable>& timer_;
};

class MockTimerObserver : public TimerObserver<TestTrackable> {
 public:
  MOCK_METHOD1(OnTimeoutTriggered, void(const TestTrackable& expired));
};
}  //  namespace functional_modules
