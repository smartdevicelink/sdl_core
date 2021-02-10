/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/application_impl.h"
#include <strings.h>
#include <string>
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "protocol_handler/protocol_handler.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/logger.h"

#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "transport_manager/common.h"
#include "utils/timer_task_impl.h"

namespace {

mobile_apis::FileType::eType StringToFileType(const char* str) {
  if (0 == strcasecmp(str, "json")) {
    return mobile_apis::FileType::JSON;
  } else if (0 == strcasecmp(str, "bmp")) {
    return mobile_apis::FileType::GRAPHIC_BMP;
  } else if (0 == strcasecmp(str, "jpeg")) {
    return mobile_apis::FileType::GRAPHIC_JPEG;
  } else if (0 == strcasecmp(str, "png")) {
    return mobile_apis::FileType::GRAPHIC_PNG;
  } else if (0 == strcasecmp(str, "wave")) {
    return mobile_apis::FileType::AUDIO_WAVE;
  } else if ((0 == strcasecmp(str, "m4a")) || (0 == strcasecmp(str, "m4b")) ||
             (0 == strcasecmp(str, "m4p")) || (0 == strcasecmp(str, "m4v")) ||
             (0 == strcasecmp(str, "m4r")) || (0 == strcasecmp(str, "3gp")) ||
             (0 == strcasecmp(str, "mp4")) || (0 == strcasecmp(str, "aac"))) {
    return mobile_apis::FileType::AUDIO_AAC;
  } else if (0 == strcasecmp(str, "mp3")) {
    return mobile_apis::FileType::AUDIO_MP3;
  } else {
    return mobile_apis::FileType::BINARY;
  }
}
}  // namespace

SDL_CREATE_LOG_VARIABLE("ApplicationManager")

namespace application_manager {

void SwitchApplicationParameters(ApplicationSharedPtr app,
                                 const uint32_t app_id,
                                 const size_t device_id,
                                 const std::string& mac_address) {
  SDL_LOG_AUTO_TRACE();
  std::shared_ptr<ApplicationImpl> application =
      std::dynamic_pointer_cast<ApplicationImpl>(app);
  DCHECK_OR_RETURN_VOID(application);
  application->app_id_ = app_id;
  application->device_id_ = device_id;
  application->mac_address_ = mac_address;
}

ApplicationImpl::ApplicationImpl(
    uint32_t application_id,
    const std::string& mobile_app_id,
    const std::string& mac_address,
    const connection_handler::DeviceHandle device_id,
    const custom_str::CustomString& app_name,
    std::shared_ptr<usage_statistics::StatisticsManager> statistics_manager,
    ApplicationManager& application_manager)
    : grammar_id_(0)
    , hmi_app_id_(0)
    , app_id_(application_id)
    , active_message_(NULL)
    , is_media_(false)
    , is_navi_(false)
    , is_remote_control_supported_(false)
    , mobile_projection_enabled_(false)
    , webengine_projection_enabled_(false)
    , video_streaming_approved_(false)
    , audio_streaming_approved_(false)
    , video_streaming_allowed_(false)
    , audio_streaming_allowed_(false)
    , video_streaming_suspended_(true)
    , audio_streaming_suspended_(true)
    , is_app_allowed_(true)
    , is_app_data_resumption_allowed_(false)
    , has_been_activated_(false)
    , is_ready_(false)
    , tts_properties_in_none_(false)
    , tts_properties_in_full_(false)
    , keep_context_(false)
    , is_foreground_(false)
    , is_application_data_changed_(false)
    , put_file_in_none_count_(0)
    , delete_file_in_none_count_(0)
    , list_files_in_none_count_(0)
    , mac_address_(mac_address)
    , device_id_(device_id)
    , secondary_device_id_(0)
    , usage_report_(mobile_app_id, statistics_manager)
    , help_prompt_manager_impl_(*this, application_manager)
    , protocol_version_(
          protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3)
    , is_voice_communication_application_(false)
    , is_resuming_(false)
    , deferred_resumption_hmi_level_(mobile_api::HMILevel::INVALID_ENUM)
    , is_hash_changed_during_suspend_(false)
    , video_stream_retry_number_(0)
    , audio_stream_retry_number_(0)
    , video_stream_suspend_timer_(
          "VideoStreamSuspend",
          new ::timer::TimerTaskImpl<ApplicationImpl>(
              this, &ApplicationImpl::OnVideoStreamSuspend))
    , audio_stream_suspend_timer_(
          "AudioStreamSuspend",
          new ::timer::TimerTaskImpl<ApplicationImpl>(
              this, &ApplicationImpl::OnAudioStreamSuspend))
    , hybrid_app_preference_(mobile_api::HybridAppPreference::INVALID_ENUM)
    , vi_lock_ptr_(std::make_shared<sync_primitives::Lock>())
    , button_lock_ptr_(std::make_shared<sync_primitives::Lock>())
    , application_manager_(application_manager) {
  cmd_number_to_time_limits_[mobile_apis::FunctionID::ReadDIDID] = {
      date_time::getCurrentTime(), 0};
  cmd_number_to_time_limits_[mobile_apis::FunctionID::GetVehicleDataID] = {
      date_time::getCurrentTime(), 0};

  set_mobile_app_id(mobile_app_id);
  set_name(app_name);

  MarkUnregistered();
  // subscribe application to custom button by default
  SubscribeToButton(mobile_apis::ButtonName::CUSTOM_BUTTON);
  // load persistent files
  LoadPersistentFiles();

  video_stream_suspend_timeout_ =
      application_manager_.get_settings().video_data_stopped_timeout();
  audio_stream_suspend_timeout_ =
      application_manager_.get_settings().audio_data_stopped_timeout();
}

ApplicationImpl::~ApplicationImpl() {
  // TODO(AK): check if this is correct assimption
  if (active_message_) {
    delete active_message_;
    active_message_ = NULL;
  }

  subscribed_buttons_.clear();
  if (is_perform_interaction_active()) {
    set_perform_interaction_active(0);
    set_perform_interaction_mode(-1);
  }
  CleanupFiles();
}

void ApplicationImpl::CloseActiveMessage() {
  delete active_message_;
  active_message_ = NULL;
}

bool ApplicationImpl::IsFullscreen() const {
  return mobile_api::HMILevel::HMI_FULL ==
         hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
}

bool ApplicationImpl::is_audio() const {
  return is_media_application() || is_voice_communication_supported() ||
         is_navi();
}

void ApplicationImpl::ChangeSupportingAppHMIType() {
  set_is_navi(false);
  set_voice_communication_supported(false);
  set_mobile_projection_enabled(false);
  set_webengine_projection_enabled(false);
  const smart_objects::SmartObject& array_app_types = *app_types_;
  uint32_t lenght_app_types = array_app_types.length();

  for (uint32_t i = 0; i < lenght_app_types; ++i) {
    const auto app_hmi_type = static_cast<mobile_apis::AppHMIType::eType>(
        array_app_types[i].asUInt());

    switch (app_hmi_type) {
      case mobile_apis::AppHMIType::NAVIGATION:
        set_is_navi(true);
        break;
      case mobile_apis::AppHMIType::COMMUNICATION:
        set_voice_communication_supported(true);
        break;
      case mobile_apis::AppHMIType::PROJECTION:
        set_mobile_projection_enabled(true);
        break;
      case mobile_apis::AppHMIType::WEB_VIEW:
        set_webengine_projection_enabled(true);
        break;
      default:
        break;
    }
  }
}

void ApplicationImpl::set_is_navi(bool option) {
  SDL_LOG_TRACE("option " << std::boolalpha << option);
  is_navi_ = option;
}

bool ApplicationImpl::is_remote_control_supported() const {
  return is_remote_control_supported_;
}

void ApplicationImpl::set_remote_control_supported(const bool allow) {
  is_remote_control_supported_ = allow;
}

bool ApplicationImpl::is_voice_communication_supported() const {
  return is_voice_communication_application_;
}

void ApplicationImpl::set_voice_communication_supported(bool option) {
  SDL_LOG_TRACE("option " << std::boolalpha << option);
  is_voice_communication_application_ = option;
}

bool ApplicationImpl::IsAudioApplication() const {
  const bool is_audio_app =
      is_media_application() || is_voice_communication_supported() || is_navi();
  SDL_LOG_DEBUG(std::boolalpha
                << "is audio app --> ((is_media_app: " << is_media_application()
                << ")"
                << " || (is_voice_communication_app: "
                << is_voice_communication_supported() << ")"
                << " || (is_navi: " << is_navi() << ")) --> " << is_audio_app);
  return is_audio_app;
}

bool ApplicationImpl::IsVideoApplication() const {
  const bool is_video_app = is_navi() || mobile_projection_enabled();
  SDL_LOG_DEBUG(std::boolalpha
                << "is video app --> ((is_navi: " << is_navi() << ")"
                << " || (mobile_projection: " << mobile_projection_enabled()
                << ")) --> " << is_video_app);
  return is_video_app;
}

void ApplicationImpl::SetRegularState(const WindowID window_id,
                                      HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  state_.AddState(window_id, state);
}

void ApplicationImpl::RemovePostponedState(const WindowID window_id) {
  SDL_LOG_AUTO_TRACE();
  state_.RemoveState(window_id, HmiState::STATE_ID_POSTPONED);
}

void ApplicationImpl::SetPostponedState(const WindowID window_id,
                                        HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  state_.AddState(window_id, state);
}

void ApplicationImpl::set_mobile_projection_enabled(bool option) {
  SDL_LOG_TRACE("option " << std::boolalpha << option);
  mobile_projection_enabled_ = option;
}

bool ApplicationImpl::mobile_projection_enabled() const {
  return mobile_projection_enabled_;
}

void ApplicationImpl::set_webengine_projection_enabled(const bool option) {
  SDL_LOG_TRACE("option " << std::boolalpha << option);
  webengine_projection_enabled_ = option;
}

bool ApplicationImpl::webengine_projection_enabled() const {
  return webengine_projection_enabled_;
}

struct StateIDComparator {
  HmiState::StateID state_id_;
  explicit StateIDComparator(HmiState::StateID state_id)
      : state_id_(state_id) {}
  bool operator()(const HmiStatePtr cur) const {
    return cur->state_id() == state_id_;
  }
};

void ApplicationImpl::AddHMIState(const WindowID window_id, HmiStatePtr state) {
  SDL_LOG_AUTO_TRACE();
  state_.AddState(window_id, state);
}

void ApplicationImpl::RemoveHMIState(const WindowID window_id,
                                     HmiState::StateID state_id) {
  SDL_LOG_AUTO_TRACE();
  state_.RemoveState(window_id, state_id);
}

const HmiStatePtr ApplicationImpl::CurrentHmiState(
    const WindowID window_id) const {
  return state_.GetState(window_id, HmiState::STATE_ID_CURRENT);
}

const HmiStatePtr ApplicationImpl::RegularHmiState(
    const WindowID window_id) const {
  return state_.GetState(window_id, HmiState::STATE_ID_REGULAR);
}

WindowNames ApplicationImpl::GetWindowNames() const {
  SDL_LOG_DEBUG("Collecting window names for application " << app_id());

  WindowNames window_names;
  std::string stringified_window_names;

  sync_primitives::AutoLock auto_lock(window_params_map_lock_ptr_);
  for (const auto& window_info_item : window_params_map_) {
    const auto window_name =
        (*window_info_item.second)[strings::window_name].asString();
    window_names.push_back(window_name);
    stringified_window_names +=
        (stringified_window_names.empty() ? "" : ",") + window_name;
  }

  SDL_LOG_DEBUG("Existing window names: [" + stringified_window_names + "]");
  return window_names;
}

WindowIds ApplicationImpl::GetWindowIds() const {
  SDL_LOG_DEBUG("Collecting window IDs for application " << app_id());
  return state_.GetWindowIds();
}

bool ApplicationImpl::WindowIdExists(const WindowID window_id) const {
  const WindowIds window_ids = GetWindowIds();
  return helpers::in_range(window_ids, window_id);
}

bool ApplicationImpl::IsAllowedToChangeAudioSource() const {
  if (is_remote_control_supported() && is_media_application()) {
    return true;
  }
  return false;
}

const HmiStatePtr ApplicationImpl::PostponedHmiState(
    const WindowID window_id) const {
  return state_.GetState(window_id, HmiState::STATE_ID_POSTPONED);
}

const smart_objects::SmartObject* ApplicationImpl::active_message() const {
  return active_message_;
}

const Version& ApplicationImpl::version() const {
  return version_;
}

void ApplicationImpl::set_hmi_application_id(uint32_t hmi_app_id) {
  hmi_app_id_ = hmi_app_id;
}

const custom_str::CustomString& ApplicationImpl::name() const {
  return app_name_;
}

void ApplicationImpl::set_folder_name(const std::string& folder_name) {
  folder_name_ = folder_name;
}

const std::string ApplicationImpl::folder_name() const {
  return folder_name_;
}

bool ApplicationImpl::is_media_application() const {
  return is_media_;
}

const mobile_api::HMILevel::eType ApplicationImpl::hmi_level(
    const WindowID window_id) const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state = CurrentHmiState(window_id);
  return hmi_state ? hmi_state->hmi_level() : HMILevel::INVALID_ENUM;
}

bool ApplicationImpl::is_foreground() const {
  return is_foreground_;
}

void ApplicationImpl::set_foreground(const bool is_foreground) {
  is_foreground_ = is_foreground;
}

const uint32_t ApplicationImpl::put_file_in_none_count() const {
  return put_file_in_none_count_;
}

const uint32_t ApplicationImpl::delete_file_in_none_count() const {
  return delete_file_in_none_count_;
}

const uint32_t ApplicationImpl::list_files_in_none_count() const {
  return list_files_in_none_count_;
}

const mobile_api::SystemContext::eType ApplicationImpl::system_context(
    const WindowID window_id) const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state = CurrentHmiState(window_id);
  return hmi_state ? hmi_state->system_context() : SystemContext::INVALID_ENUM;
  ;
}

const std::string& ApplicationImpl::app_icon_path() const {
  return app_icon_path_;
}

const std::string& ApplicationImpl::bundle_id() const {
  return bundle_id_;
}

connection_handler::DeviceHandle ApplicationImpl::device() const {
  return device_id_;
}

connection_handler::DeviceHandle ApplicationImpl::secondary_device() const {
  return secondary_device_id_;
}

const std::string& ApplicationImpl::mac_address() const {
  return mac_address_;
}

void ApplicationImpl::set_version(const Version& ver) {
  version_ = ver;
}

void ApplicationImpl::set_name(const custom_str::CustomString& name) {
  app_name_ = name;
}

void ApplicationImpl::set_is_media_application(bool option) {
  SDL_LOG_TRACE("option " << std::boolalpha << option);
  is_media_ = option;
}

void ApplicationImpl::set_tts_properties_in_none(bool active) {
  tts_properties_in_none_ = active;
}

bool ApplicationImpl::tts_properties_in_none() {
  return tts_properties_in_none_;
}

void ApplicationImpl::set_tts_properties_in_full(bool active) {
  tts_properties_in_full_ = active;
}

bool ApplicationImpl::tts_properties_in_full() {
  return tts_properties_in_full_;
}

void ApplicationImpl::set_keep_context(bool keep_context) {
  keep_context_ = keep_context;
}

bool ApplicationImpl::keep_context() {
  return keep_context_;
}

void ApplicationImpl::set_video_streaming_approved(bool state) {
  video_streaming_approved_ = state;
}

bool ApplicationImpl::video_streaming_approved() const {
  return video_streaming_approved_;
}

void ApplicationImpl::set_audio_streaming_approved(bool state) {
  audio_streaming_approved_ = state;
}

bool ApplicationImpl::audio_streaming_approved() const {
  return audio_streaming_approved_;
}

void ApplicationImpl::set_video_streaming_allowed(bool state) {
  video_streaming_allowed_ = state;
}

bool ApplicationImpl::video_streaming_allowed() const {
  return video_streaming_allowed_;
}

void ApplicationImpl::set_audio_streaming_allowed(bool state) {
  audio_streaming_allowed_ = state;
}

bool ApplicationImpl::audio_streaming_allowed() const {
  return audio_streaming_allowed_;
}

bool ApplicationImpl::SetVideoConfig(protocol_handler::ServiceType service_type,
                                     const smart_objects::SmartObject& params) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  if (ServiceType::kMobileNav == service_type) {
    // See StartStreaming(). We issue SetVideoConfig and StartStream
    // only when streaming is not approved yet
    if (!video_streaming_approved()) {
      SDL_LOG_TRACE("Video streaming not approved");
      MessageHelper::SendNaviSetVideoConfig(
          app_id(), application_manager_, params);
      return true;
    }
  }
  return false;
}

void ApplicationImpl::StartStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  if (ServiceType::kMobileNav == service_type) {
    SDL_LOG_TRACE("ServiceType = Video");
    if (!video_streaming_approved()) {
      SDL_LOG_TRACE("Video streaming not approved");
      MessageHelper::SendNaviStartStream(app_id(), application_manager_);
      set_video_stream_retry_number(0);
    }
  } else if (ServiceType::kAudio == service_type) {
    SDL_LOG_TRACE("ServiceType = Audio");
    if (!audio_streaming_approved()) {
      SDL_LOG_TRACE("Audio streaming not approved");
      MessageHelper::SendAudioStartStream(app_id(), application_manager_);
      set_audio_stream_retry_number(0);
    }
  }
}

void ApplicationImpl::StopStreamingForce(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  // see the comment in StopStreaming()
  sync_primitives::AutoLock lock(streaming_stop_lock_);

  SuspendStreaming(service_type);

  if (service_type == ServiceType::kMobileNav) {
    StopNaviStreaming();
  } else if (service_type == ServiceType::kAudio) {
    StopAudioStreaming();
  }
}

void ApplicationImpl::StopStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  // since WakeUpStreaming() is called from another thread, it is possible that
  // the stream will be restarted after we call SuspendStreaming() and before
  // we call StopXxxStreaming(). To avoid such timing issue, make sure that
  // we run SuspendStreaming() and StopXxxStreaming() atomically.
  sync_primitives::AutoLock lock(streaming_stop_lock_);

  SuspendStreaming(service_type);

  if (service_type == ServiceType::kMobileNav && video_streaming_approved()) {
    StopNaviStreaming();
  } else if (service_type == ServiceType::kAudio &&
             audio_streaming_approved()) {
    StopAudioStreaming();
  }
}

void ApplicationImpl::StopNaviStreaming() {
  SDL_LOG_AUTO_TRACE();
  video_stream_suspend_timer_.Stop();
  application_manager_.OnAppStreaming(app_id(),
                                      protocol_handler::ServiceType::kMobileNav,
                                      StreamingState::kStopped);
  MessageHelper::SendNaviStopStream(app_id(), application_manager_);
  set_video_streaming_approved(false);
  set_video_stream_retry_number(0);
}

void ApplicationImpl::StopAudioStreaming() {
  SDL_LOG_AUTO_TRACE();
  audio_stream_suspend_timer_.Stop();
  application_manager_.OnAppStreaming(app_id(),
                                      protocol_handler::ServiceType::kAudio,
                                      StreamingState::kStopped);
  MessageHelper::SendAudioStopStream(app_id(), application_manager_);
  set_audio_streaming_approved(false);
  set_audio_stream_retry_number(0);
}

void ApplicationImpl::SuspendStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  if (ServiceType::kMobileNav == service_type && !video_streaming_suspended_) {
    video_stream_suspend_timer_.Stop();
    application_manager_.OnAppStreaming(
        app_id(), service_type, StreamingState::kSuspended);
    sync_primitives::AutoLock lock(video_streaming_suspended_lock_);
    video_streaming_suspended_ = true;
  } else if (ServiceType::kAudio == service_type &&
             !audio_streaming_suspended_) {
    audio_stream_suspend_timer_.Stop();
    application_manager_.OnAppStreaming(
        app_id(), service_type, StreamingState::kSuspended);
    sync_primitives::AutoLock lock(audio_streaming_suspended_lock_);
    audio_streaming_suspended_ = true;
  }
  application_manager_.ProcessOnDataStreamingNotification(
      service_type, app_id(), false);
}

void ApplicationImpl::WakeUpStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();

  // See the comment in StopStreaming(). Also, please make sure that we acquire
  // streaming_stop_lock_ then xxx_streaming_suspended_lock_ in this order!
  sync_primitives::AutoLock lock(streaming_stop_lock_);

  if (ServiceType::kMobileNav == service_type) {
    {  // reduce the range of video_streaming_suspended_lock_
      sync_primitives::AutoLock auto_lock(video_streaming_suspended_lock_);
      if (video_streaming_suspended_) {
        application_manager_.OnAppStreaming(
            app_id(), service_type, StreamingState::kStarted);
        application_manager_.ProcessOnDataStreamingNotification(
            service_type, app_id(), true);
        video_streaming_suspended_ = false;
      }
    }

    video_stream_suspend_timer_.Start(video_stream_suspend_timeout_,
                                      timer::kPeriodic);
  } else if (ServiceType::kAudio == service_type) {
    {  // reduce the range of audio_streaming_suspended_lock_
      sync_primitives::AutoLock auto_lock(audio_streaming_suspended_lock_);
      if (audio_streaming_suspended_) {
        application_manager_.OnAppStreaming(
            app_id(), service_type, StreamingState::kStarted);
        application_manager_.ProcessOnDataStreamingNotification(
            service_type, app_id(), true);
        audio_streaming_suspended_ = false;
      }
    }
    audio_stream_suspend_timer_.Start(audio_stream_suspend_timeout_,
                                      timer::kPeriodic);
  }
}

void ApplicationImpl::OnVideoStreamSuspend() {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_INFO("Suspend video streaming by timer");
  SuspendStreaming(ServiceType::kMobileNav);
}

void ApplicationImpl::OnAudioStreamSuspend() {
  using namespace protocol_handler;
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_INFO("Suspend audio streaming by timer");
  SuspendStreaming(ServiceType::kAudio);
}

uint32_t ApplicationImpl::audio_stream_retry_number() const {
  return audio_stream_retry_number_;
}

uint32_t ApplicationImpl::video_stream_retry_number() const {
  return video_stream_retry_number_;
}

void ApplicationImpl::set_video_stream_retry_number(
    const uint32_t& video_stream_retry_number) {
  video_stream_retry_number_ = video_stream_retry_number;
}

void ApplicationImpl::set_audio_stream_retry_number(
    const uint32_t& audio_stream_retry_number) {
  audio_stream_retry_number_ = audio_stream_retry_number;
}

void ApplicationImpl::increment_put_file_in_none_count() {
  ++put_file_in_none_count_;
}

void ApplicationImpl::increment_delete_file_in_none_count() {
  ++delete_file_in_none_count_;
}

void ApplicationImpl::increment_list_files_in_none_count() {
  ++list_files_in_none_count_;
}

bool ApplicationImpl::set_app_icon_path(const std::string& path) {
  if (app_files_.find(path) != app_files_.end()) {
    app_icon_path_ = path;
    return true;
  }
  return false;
}

void ApplicationImpl::set_app_allowed(const bool allowed) {
  is_app_allowed_ = allowed;
}

bool ApplicationImpl::is_app_data_resumption_allowed() const {
  return is_app_data_resumption_allowed_;
}

void ApplicationImpl::set_app_data_resumption_allowance(const bool allowed) {
  is_app_data_resumption_allowed_ = allowed;
}

void ApplicationImpl::set_secondary_device(
    connection_handler::DeviceHandle secondary_device) {
  secondary_device_id_ = secondary_device;
}

uint32_t ApplicationImpl::get_grammar_id() const {
  return grammar_id_;
}

void ApplicationImpl::set_grammar_id(uint32_t value) {
  grammar_id_ = value;
}

void ApplicationImpl::set_bundle_id(const std::string& bundle_id) {
  bundle_id_ = bundle_id;
}

void ApplicationImpl::ResetDataInNone() {
  put_file_in_none_count_ = 0;
  delete_file_in_none_count_ = 0;
  list_files_in_none_count_ = 0;
}

bool ApplicationImpl::has_been_activated() const {
  return has_been_activated_;
}

bool ApplicationImpl::set_activated(bool is_active) {
  has_been_activated_ = is_active;
  return true;
}

bool ApplicationImpl::is_ready() const {
  return is_ready_;
}

bool ApplicationImpl::set_is_ready(bool is_ready) {
  is_ready_ = is_ready;
  return true;
}

void ApplicationImpl::set_protocol_version(
    const protocol_handler::MajorProtocolVersion& protocol_version) {
  protocol_version_ = protocol_version;
}

protocol_handler::MajorProtocolVersion ApplicationImpl::protocol_version()
    const {
  return protocol_version_;
}

void ApplicationImpl::set_is_resuming(bool is_resuming) {
  is_resuming_ = is_resuming;
}

bool ApplicationImpl::is_resuming() const {
  return is_resuming_;
}

void ApplicationImpl::set_deferred_resumption_hmi_level(
    mobile_api::HMILevel::eType level) {
  deferred_resumption_hmi_level_ = level;
}

mobile_api::HMILevel::eType ApplicationImpl::deferred_resumption_hmi_level()
    const {
  return deferred_resumption_hmi_level_;
}

bool ApplicationImpl::AddFile(const AppFile& file) {
  if (app_files_.count(file.file_name) == 0) {
    SDL_LOG_INFO("AddFile file " << file.file_name << " File type is "
                                 << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::UpdateFile(const AppFile& file) {
  if (app_files_.count(file.file_name) != 0) {
    SDL_LOG_INFO("UpdateFile file " << file.file_name << " File type is "
                                    << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::DeleteFile(const std::string& file_name) {
  AppFilesMap::iterator it = app_files_.find(file_name);
  if (it != app_files_.end()) {
    SDL_LOG_INFO("DeleteFile file " << it->second.file_name << " File type is "
                                    << it->second.file_type);
    app_files_.erase(it);
    return true;
  }
  return false;
}

const AppFilesMap& ApplicationImpl::getAppFiles() const {
  return this->app_files_;
}

const AppFile* ApplicationImpl::GetFile(const std::string& file_name) {
  if (app_files_.find(file_name) != app_files_.end()) {
    return &(app_files_[file_name]);
  }
  return NULL;
}

bool ApplicationImpl::SubscribeToButton(
    mobile_apis::ButtonName::eType btn_name) {
  sync_primitives::AutoLock lock(button_lock_ptr_);
  return subscribed_buttons_.insert(btn_name).second;
}

bool ApplicationImpl::IsSubscribedToButton(
    mobile_apis::ButtonName::eType btn_name) {
  sync_primitives::AutoLock lock(button_lock_ptr_);
  std::set<mobile_apis::ButtonName::eType>::iterator it =
      subscribed_buttons_.find(btn_name);
  return (subscribed_buttons_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromButton(
    mobile_apis::ButtonName::eType btn_name) {
  sync_primitives::AutoLock lock(button_lock_ptr_);
  return subscribed_buttons_.erase(btn_name);
}

UsageStatistics& ApplicationImpl::usage_report() {
  return usage_report_;
}

HelpPromptManager& ApplicationImpl::help_prompt_manager() {
  return help_prompt_manager_impl_;
}

bool ApplicationImpl::AreCommandLimitsExceeded(
    mobile_apis::FunctionID::eType cmd_id, TLimitSource source) {
  date_time::TimeDuration current = date_time::getCurrentTime();
  switch (source) {
    // In case of config file values there is COMMON limitations for number of
    // commands per certain time in seconds, i.e. 5 requests per 10 seconds with
    // any interval between them
    case CONFIG_FILE: {
      CommandNumberTimeLimit::iterator it =
          cmd_number_to_time_limits_.find(cmd_id);
      if (cmd_number_to_time_limits_.end() == it) {
        SDL_LOG_WARN("Limits for command id " << cmd_id << "had not been set.");
        return true;
      }

      TimeToNumberLimit& limit = it->second;

      std::pair<uint32_t, int32_t> frequency_restrictions;

      if (mobile_apis::FunctionID::ReadDIDID == cmd_id) {
        frequency_restrictions =
            application_manager_.get_settings().read_did_frequency();

      } else if (mobile_apis::FunctionID::GetVehicleDataID == cmd_id) {
        frequency_restrictions =
            application_manager_.get_settings().get_vehicle_data_frequency();
      } else {
        SDL_LOG_INFO("No restrictions for request");
        return false;
      }

      SDL_LOG_INFO("Time Info: "
                   << "\n Current: " << date_time::getSecs(current)
                   << "\n Limit: (" << date_time::getSecs(limit.first) << ","
                   << limit.second
                   << ")"
                      "\n frequency_restrictions: ("
                   << frequency_restrictions.first << ","
                   << frequency_restrictions.second << ")");
      if (date_time::getSecs(current) <
          date_time::getSecs(limit.first) + frequency_restrictions.second) {
        if (limit.second < frequency_restrictions.first) {
          ++limit.second;
          return false;
        }
        return true;
      }

      limit.first = current;
      limit.second = 1;
      return false;
    }
    // In case of policy table values, there is EVEN limitation for number of
    // commands per minute, e.g. 10 command per minute i.e. 1 command per 6 sec
    case POLICY_TABLE: {
      const policy::PolicyHandlerInterface& policy_handler =
          application_manager_.GetPolicyHandler();
      std::string priority;
      policy_handler.GetPriority(policy_app_id(), &priority);
      const bool is_subtle_alert =
          (cmd_id == mobile_apis::FunctionID::SubtleAlertID);
      uint32_t cmd_limit =
          policy_handler.GetNotificationsNumber(priority, is_subtle_alert);

      if (0 == cmd_limit) {
        return true;
      }

      const uint32_t dummy_limit = 1;
      CommandNumberTimeLimit::iterator it =
          cmd_number_to_time_limits_.find(cmd_id);
      // If no command with cmd_id had been executed yet, just add to limits
      if (cmd_number_to_time_limits_.end() == it) {
        cmd_number_to_time_limits_[cmd_id] = {current, dummy_limit};
        return false;
      }

      const uint32_t minute = 60;

      TimeToNumberLimit& limit = it->second;

      // Checking even limitation for command
      if (static_cast<uint32_t>(date_time::getSecs(current) -
                                date_time::getSecs(limit.first)) <
          minute / cmd_limit) {
        return true;
      }

      cmd_number_to_time_limits_[cmd_id] = {current, dummy_limit};

      return false;
    }
    default: {
      SDL_LOG_WARN("Limit source is not implemented.");
      break;
    }
  }

  return true;
}

DataAccessor<ButtonSubscriptions> ApplicationImpl::SubscribedButtons() const {
  return DataAccessor<ButtonSubscriptions>(subscribed_buttons_,
                                           button_lock_ptr_);
}

const std::string& ApplicationImpl::curHash() const {
  return hash_val_;
}

bool ApplicationImpl::is_application_data_changed() const {
  return is_application_data_changed_;
}

void ApplicationImpl::SetInitialState(const WindowID window_id,
                                      const std::string& window_name,
                                      HmiStatePtr state) {
  state_.InitState(window_id, window_name, state);
}

void ApplicationImpl::set_is_application_data_changed(
    bool state_application_data) {
  is_application_data_changed_ = state_application_data;
}

void ApplicationImpl::UpdateHash() {
  SDL_LOG_AUTO_TRACE();
  hash_val_ =
      utils::gen_hash(application_manager_.get_settings().hash_string_size());
  set_is_application_data_changed(true);

  if (!application_manager_.resume_controller().is_suspended()) {
    MessageHelper::SendHashUpdateNotification(app_id(), application_manager_);
  } else {
    is_hash_changed_during_suspend_ = true;
  }
}

bool ApplicationImpl::IsHashChangedDuringSuspend() const {
  return is_hash_changed_during_suspend_;
}

void ApplicationImpl::SetHashChangedDuringSuspend(const bool state) {
  is_hash_changed_during_suspend_ = state;
}

void ApplicationImpl::CleanupFiles() {
  std::string directory_name =
      application_manager_.get_settings().app_storage_folder();
  directory_name += "/" + folder_name();

  if (file_system::DirectoryExists(directory_name) && !folder_name().empty()) {
    std::vector<std::string> files = file_system::ListFiles(directory_name);
    AppFilesMap::const_iterator app_files_it;

    std::vector<std::string>::const_iterator it = files.begin();
    for (; it != files.end(); ++it) {
      std::string file_name = directory_name;
      file_name += "/";
      file_name += *it;
      app_files_it = app_files_.find(file_name);
      if ((app_files_it == app_files_.end()) ||
          (!app_files_it->second.is_persistent)) {
        SDL_LOG_INFO("DeleteFile file " << file_name);
        file_system::DeleteFile(file_name);
      }
    }

    file_system::RemoveDirectory(directory_name, false);
  }
  app_files_.clear();
}

void ApplicationImpl::LoadPersistentFiles() {
  using namespace profile;

  if (kWaitingForRegistration == app_state_) {
    const std::string app_icon_dir(
        application_manager_.get_settings().app_icons_folder());
    const std::string full_icon_path(app_icon_dir + "/" + mobile_app_id_);
    if (file_system::FileExists(full_icon_path)) {
      AppFile file;
      file.is_persistent = true;
      file.is_download_complete = true;
      file.file_name = full_icon_path;
      file.file_type = mobile_apis::FileType::GRAPHIC_PNG;
      AddFile(file);
    }
    return;
  }

  std::string directory_name =
      application_manager_.get_settings().app_storage_folder();
  directory_name += "/" + folder_name();

  if (file_system::DirectoryExists(directory_name)) {
    std::vector<std::string> persistent_files =
        file_system::ListFiles(directory_name);

    std::vector<std::string>::const_iterator it = persistent_files.begin();
    for (; it != persistent_files.end(); ++it) {
      AppFile file;
      file.is_persistent = true;
      file.is_download_complete = true;
      file.file_name = directory_name;
      file.file_name += "/";
      file.file_name += *it;
      file.file_type = mobile_apis::FileType::BINARY;
      // Search file extension and convert it to the type
      std::size_t index = it->find_last_of('.');
      if (index != std::string::npos) {
        std::string file_type = it->substr(++index);
        file.file_type = StringToFileType(file_type.c_str());
      }

      SDL_LOG_INFO("Loaded persistent file "
                   << file.file_name << " File type is " << file.file_type);
      AddFile(file);
    }
  }
}

uint32_t ApplicationImpl::GetAvailableDiskSpace() {
  const uint32_t app_quota =
      application_manager_.get_settings().app_dir_quota();
  std::string app_storage_path =
      application_manager_.get_settings().app_storage_folder();

  app_storage_path += "/";
  app_storage_path += folder_name();

  if (file_system::DirectoryExists(app_storage_path)) {
    size_t size_of_directory = file_system::DirectorySize(app_storage_path);
    if (app_quota < size_of_directory) {
      return 0;
    }

    uint32_t current_app_quota = app_quota - size_of_directory;
    uint32_t available_disk_space =
        file_system::GetAvailableDiskSpace(app_storage_path);

    if (current_app_quota > available_disk_space) {
      return available_disk_space;
    }
    return current_app_quota;
  }
  return app_quota;
}

void ApplicationImpl::SubscribeToSoftButtons(
    int32_t cmd_id, const WindowSoftButtons& window_softbuttons) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);

  if (cmd_softbuttonid_.end() == it) {
    SoftButtonIDs soft_buttons{window_softbuttons};
    cmd_softbuttonid_.insert({cmd_id, soft_buttons});
    return;
  }

  auto& command_soft_buttons = cmd_softbuttonid_[cmd_id];

  const auto window_id = window_softbuttons.first;
  auto find_window_id = [window_id](const WindowSoftButtons& window_buttons) {
    return window_id == window_buttons.first;
  };

  auto subscribed_window_buttons = std::find_if(
      command_soft_buttons.begin(), command_soft_buttons.end(), find_window_id);

  if (subscribed_window_buttons == command_soft_buttons.end()) {
    command_soft_buttons.insert(window_softbuttons);
    return;
  }

  WindowSoftButtons new_window_soft_buttons = *subscribed_window_buttons;
  new_window_soft_buttons.second.insert(window_softbuttons.second.begin(),
                                        window_softbuttons.second.end());
  command_soft_buttons.erase(subscribed_window_buttons);
  command_soft_buttons.insert(new_window_soft_buttons);
}

struct FindSoftButtonId {
  uint32_t soft_button_id_;

  explicit FindSoftButtonId(const uint32_t soft_button_id)
      : soft_button_id_(soft_button_id) {}

  bool operator()(const uint32_t softbutton_id) {
    return soft_button_id_ == softbutton_id;
  }
};

struct FindWindowSoftButtonId {
 public:
  FindWindowSoftButtonId(const uint32_t soft_button_id)
      : find_softbutton_id_(soft_button_id) {}

  bool operator()(const WindowSoftButtons& window_buttons) {
    const auto softbuttons = window_buttons.second;
    auto found_softbutton = std::find_if(
        softbuttons.begin(), softbuttons.end(), find_softbutton_id_);

    return found_softbutton != softbuttons.end();
  }

 private:
  FindSoftButtonId find_softbutton_id_;
};

bool ApplicationImpl::IsSubscribedToSoftButton(const uint32_t softbutton_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);

  for (const auto& command_soft_buttons : cmd_softbuttonid_) {
    FindWindowSoftButtonId find_window_softbutton_id(softbutton_id);
    const auto& window_softbuttons = command_soft_buttons.second;

    const auto found_window_softbutton_id =
        std::find_if(window_softbuttons.begin(),
                     window_softbuttons.end(),
                     find_window_softbutton_id);

    if (found_window_softbutton_id != window_softbuttons.end()) {
      return true;
    }
  }

  return false;
}

WindowID ApplicationImpl::GetSoftButtonWindowID(const uint32_t softbutton_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);

  for (const auto& command_soft_buttons : cmd_softbuttonid_) {
    FindWindowSoftButtonId find_window_softbutton_id(softbutton_id);
    const auto& window_softbuttons = command_soft_buttons.second;

    const auto found_window_softbutton_id =
        std::find_if(window_softbuttons.begin(),
                     window_softbuttons.end(),
                     find_window_softbutton_id);

    if (found_window_softbutton_id != window_softbuttons.end()) {
      return found_window_softbutton_id->first;
    }
  }

  return mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}

void ApplicationImpl::UnsubscribeFromSoftButtons(int32_t cmd_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);
  if (it != cmd_softbuttonid_.end()) {
    cmd_softbuttonid_.erase(it);
  }
}

void ApplicationImpl::set_system_context(
    const WindowID window_id,
    const mobile_api::SystemContext::eType& system_context) {
  HmiStatePtr hmi_state = CurrentHmiState(window_id);
  hmi_state->set_system_context(system_context);
}

void ApplicationImpl::set_audio_streaming_state(
    const mobile_api::AudioStreamingState::eType& state) {
  if (!(is_media_application() || is_navi()) &&
      state != mobile_api::AudioStreamingState::NOT_AUDIBLE) {
    SDL_LOG_WARN(
        "Trying to set audio streaming state"
        " for non-media application to different from NOT_AUDIBLE");
    return;
  }

  // According to proposal SDL-0216 audio streaming state should
  // be applied for all windows to keep consistency
  HmiStates hmi_states = state_.GetStates(HmiState::STATE_ID_CURRENT);
  for (const auto& hmi_state : hmi_states) {
    hmi_state->set_audio_streaming_state(state);
  }
}

void ApplicationImpl::set_hmi_level(
    const WindowID window_id,
    const mobile_api::HMILevel::eType& new_hmi_level) {
  using namespace mobile_apis;
  const HMILevel::eType current_hmi_level = hmi_level(window_id);
  if (HMILevel::HMI_NONE != current_hmi_level &&
      HMILevel::HMI_NONE == new_hmi_level) {
    put_file_in_none_count_ = 0;
    delete_file_in_none_count_ = 0;
    list_files_in_none_count_ = 0;
  }
  ApplicationSharedPtr app = application_manager_.application(app_id());
  DCHECK_OR_RETURN_VOID(app)
  application_manager_.state_controller().SetRegularState(
      app, window_id, new_hmi_level);
  SDL_LOG_INFO("hmi_level = " << new_hmi_level);
  usage_report_.RecordHmiStateChanged(new_hmi_level);
}

AppExtensionPtr ApplicationImpl::QueryInterface(AppExtensionUID uid) {
  std::list<AppExtensionPtr>::const_iterator it = extensions_.begin();
  for (; it != extensions_.end(); ++it) {
    if ((*it)->uid() == uid) {
      return (*it);
    }
  }
  return AppExtensionPtr();
}

bool ApplicationImpl::AddExtension(AppExtensionPtr extension) {
  if (!QueryInterface(extension->uid())) {
    SDL_LOG_TRACE("Add extenstion to add id" << app_id() << " with uid "
                                             << extension->uid());
    extensions_.push_back(extension);
    return true;
  }
  return false;
}

bool ApplicationImpl::RemoveExtension(AppExtensionUID uid) {
  auto it = std::find_if(
      extensions_.begin(), extensions_.end(), [uid](AppExtensionPtr extension) {
        return extension->uid() == uid;
      });

  return it != extensions_.end();
}

const std::list<AppExtensionPtr>& ApplicationImpl::Extensions() const {
  return extensions_;
}

const std::string& ApplicationImpl::cloud_app_endpoint() const {
  return endpoint_;
}

const std::string& ApplicationImpl::auth_token() const {
  return auth_token_;
}

const std::string& ApplicationImpl::cloud_app_transport_type() const {
  return cloud_transport_type_;
}

const mobile_apis::HybridAppPreference::eType&
ApplicationImpl::hybrid_app_preference() const {
  return hybrid_app_preference_;
}

const std::string& ApplicationImpl::cloud_app_certificate() const {
  return certificate_;
}

bool ApplicationImpl::is_cloud_app() const {
#if !defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  return false;
#else
  return !endpoint_.empty();
#endif  // CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
}

void ApplicationImpl::set_cloud_app_endpoint(const std::string& endpoint) {
  endpoint_ = endpoint;
}

void ApplicationImpl::set_auth_token(const std::string& auth_token) {
  auth_token_ = auth_token;
}

void ApplicationImpl::set_cloud_app_transport_type(
    const std::string& transport_type) {
  cloud_transport_type_ = transport_type;
}

void ApplicationImpl::set_hybrid_app_preference(
    const mobile_apis::HybridAppPreference::eType& hybrid_app_preference) {
  hybrid_app_preference_ = hybrid_app_preference;
}

void ApplicationImpl::set_cloud_app_certificate(
    const std::string& certificate) {
  certificate_ = certificate;
}

void ApplicationImpl::set_user_location(
    const smart_objects::SmartObject& user_location) {
  user_location_ = user_location;
}

const smart_objects::SmartObject& ApplicationImpl::get_user_location() const {
  return user_location_;
}

void ApplicationImpl::PushMobileMessage(
    smart_objects::SmartObjectSPtr mobile_message) {
  sync_primitives::AutoLock lock(mobile_message_lock_);
  mobile_message_queue_.push_back(mobile_message);
}

void ApplicationImpl::SwapMobileMessageQueue(
    MobileMessageQueue& mobile_messages) {
  sync_primitives::AutoLock lock(mobile_message_lock_);
  mobile_messages.swap(mobile_message_queue_);
}

}  // namespace application_manager
