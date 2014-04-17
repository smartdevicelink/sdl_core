#include "transport_manager_observer.h"
#include "transport_manager_metric.h"
#include "time_manager.h"

#include <time.h>

namespace time_tester {

void TransportManagerObserver::StartRawMsg(const protocol_handler::RawMessage* ptr) {
  time_t time_start = time(NULL);
  //time_starts[ptr] = time_start;
}

void TransportManagerObserver::StopRawMsg(const protocol_handler::RawMessage* ptr) {
  TransportManagerMectic* m = new TransportManagerMectic();
  m->message_metric = new transport_manager::TMMetricObserver::MessageMetric();
  TimeManager::instance()->SendMetric(m);
//  std::map<protocol_handler::RawMessage*, time_t>::iterator it;
//  it = time_starts.find(ptr);
//  if (it != time_starts.end()) {
//    time_t time_end = time(NULL);
//    MessageMetric metric;
//    metric.begin = it->second();
//    metric.end = time_end;
//    metric.data_size = ptr->data_size();
//    //messages_.push(metric);
//  }
}

} //namespace time_tester
