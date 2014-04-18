#include "transport_manager_observer.h"

#include <time.h>

#include "transport_manager_metric.h"
#include "time_manager.h"


namespace time_tester {

TransportManagerObserver::TransportManagerObserver(TimeManager* time_manager):
  time_manager_ (time_manager) {
}

void TransportManagerObserver::StartRawMsg(const protocol_handler::RawMessage* ptr) {
  time_starts[ptr] = time(NULL);
}

void TransportManagerObserver::StopRawMsg(const protocol_handler::RawMessage* ptr) {
    std::map<const protocol_handler::RawMessage*, time_t>::const_iterator it;
    it = time_starts.find(ptr);
    if (it != time_starts.end()) {
      TransportManagerMectic* m = new TransportManagerMectic();
      m->message_metric = new transport_manager::TMMetricObserver::MessageMetric();
      m->message_metric->begin = it->second;
      m->message_metric->end = time(NULL);
      m->message_metric->data_size = ptr->data_size();
      time_manager_->SendMetric(m);
    }
}

} //namespace time_tester
