#include "metric_wrapper.h"
#include "json_keys.h"

namespace time_tester {

  MetricWrapper::MetricWrapper():
    resources(NULL) {
  }

bool MetricWrapper::grabResources() {
  Clear();
  resources = utils::Resources::getCurrentResourseUsage();
  if (NULL != resources) {
    return true;
  } else {
    return false;
  }
  flush(std::cout);
}

std::string MetricWrapper::GetStyledString() {
  return GetJsonMetric().toStyledString();
}

Json::Value MetricWrapper::GetJsonMetric() {
  Json::Value result;
  if (resources) {
    result[strings::stime] = resources->stime;
    result[strings::utime] = resources->utime;
    result[strings::memory] = resources->memory;
  }
  return result;
}

void MetricWrapper::Clear() {
  if (NULL != resources) {
    delete resources;
    resources = NULL;
  }
}

MetricWrapper::~MetricWrapper() {
  Clear();
}

}
