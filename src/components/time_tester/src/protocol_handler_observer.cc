#include "protocol_handler_observer.h"
#include "utils/date_time.h"
#include "protocol_handler_metric.h"
#include "time_manager.h"


namespace time_tester {

ProtocolHandlerObserver::ProtocolHandlerObserver(TimeManager *time_manager):
  time_manager_(time_manager) {
}

void ProtocolHandlerObserver::StartMessageProcess(uint32_t message_id) {
  if (message_id == 0) {
    return;
  }
  if (time_starts.find(message_id) != time_starts.end()) {
    LOG4CXX_INFO(logger_, "Message ID already wait for stop processing" << message_id);
    return;
  }

  time_starts[message_id] = time(NULL);
}

void ProtocolHandlerObserver::EndMessageProcess(utils::SharedPtr<MessageMetric> m) {
  uint32_t message_id = m->message_id;
  std::map<uint32_t, time_t>::const_iterator it = time_starts.find(message_id);
  if (it == time_starts.end()) {
    LOG4CXX_ERROR(logger_, "Cant find start time for message" << message_id);
    return;
  }
  m->begin= time_starts[message_id];
  m->end = time(NULL);
  ProtocolHandlerMectic* metric = new ProtocolHandlerMectic();
  metric->message_metric = m;
  time_manager_->SendMetric(metric);
}

} //namespace time_tester
