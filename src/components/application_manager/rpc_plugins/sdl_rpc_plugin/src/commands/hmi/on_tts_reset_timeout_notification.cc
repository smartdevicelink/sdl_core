#include "sdl_rpc_plugin/commands/hmi/on_tts_reset_timeout_notification.h"
#include "application_manager/event_engine/event.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

namespace hmi {

OnTTSResetTimeoutNotification::OnTTSResetTimeoutNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : NotificationFromHMI(message, application_manager) {}

OnTTSResetTimeoutNotification::~OnTTSResetTimeoutNotification() {}

void OnTTSResetTimeoutNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  event_engine::Event event(hmi_apis::FunctionID::TTS_OnResetTimeout);
  event.set_smart_object(*message_);
  event.raise(application_manager_.event_dispatcher());
}

}  // namespace hmi

}  // namespace commands

}  // namespace application_manager
