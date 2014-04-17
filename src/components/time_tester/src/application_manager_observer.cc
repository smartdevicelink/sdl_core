#include "application_manager_observer.h"
#include "time_manager.h"
#include "application_manager_metric.h"
#include "utils/shared_ptr.h"

namespace time_tester {

void ApplicationManagerObserver::OnMessage(utils::SharedPtr<MessageMetric> metric) {
  ApplicationManagerMectic* m = new ApplicationManagerMectic();
  m->message_metric = metric;
  TimeManager::instance()->SendMetric(m);
}

}
