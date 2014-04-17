#include "transport_manager_observer.h"
#include "transport_manager_metric.h"
#include "time_manager.h"

#include <time.h>

namespace time_tester {

void TransportManagerObserver::StartRawMsg(const protocol_handler::RawMessage* ptr) {
  time_t time_start = time(NULL);
  time_starts[ptr] = time_start;
  LOG4CXX_WARN(TimeManager::logger_, "StartRawMsg " << ptr );
}

void TransportManagerObserver::StopRawMsg(const protocol_handler::RawMessage* ptr) {
    std::map<const protocol_handler::RawMessage*, time_t>::iterator it;
    it = time_starts.find(ptr);
    LOG4CXX_WARN(TimeManager::logger_, "StopRawMsg " << ptr );
    if (it != time_starts.end()) {
      TransportManagerMectic* m = new TransportManagerMectic();
      m->message_metric = new transport_manager::TMMetricObserver::MessageMetric();
      time_t time_end = time(NULL);
      m->message_metric->begin = it->second;
      m->message_metric->end = time_end;
      m->message_metric->data_size = ptr->data_size();
      TimeManager::instance()->SendMetric(m);
    }
}

} //namespace time_tester
