#include "application_manager_observer.h"
#include "time_manager.h"
#include "application_manager_metric.h"
#include "utils/shared_ptr.h"

namespace time_tester {

ApplicationManagerObserver::ApplicationManagerObserver(TimeManager* time_manager):
  time_manager_(time_manager) {
}

void ApplicationManagerObserver::OnMessage(utils::SharedPtr<MessageMetric> metric) {
  ApplicationManagerMetric* m = new ApplicationManagerMetric();
  m->message_metric = metric;
  time_manager_->SendMetric(m);
}

}
