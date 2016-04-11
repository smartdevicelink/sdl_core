#ifndef REQUEST_CONTROLLER_SETTINGS_H
#define REQUEST_CONTROLLER_SETTINGS_H
namespace application_manager {
class RequestControlerSettings {
public:
  virtual uint32_t thread_pool_size() const = 0;
  virtual const uint32_t& app_hmi_level_none_time_scale() const = 0;
  virtual const uint32_t& app_hmi_level_none_time_scale_max_requests()
      const = 0;
  virtual const uint32_t& app_time_scale() const = 0;
  virtual const uint32_t& app_time_scale_max_requests() const = 0;
  virtual const uint32_t& pending_requests_amount() const = 0;
};
}
#endif  // REQUEST_CONTROLLER_SETTINGS_H
