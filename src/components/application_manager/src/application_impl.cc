/*
 * Copyright (c) 2016, Ford Motor Company
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
#include <string>
#include <strings.h>
#include "application_manager/message_helper.h"
#include "protocol_handler/protocol_handler.h"
#include "application_manager/application_manager.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/logger.h"
#include "utils/gen_hash.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/timer_task_impl.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/resumption/resume_ctrl.h"

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
}

CREATE_LOGGERPTR_GLOBAL(logger_, "ApplicationManager")

namespace application_manager {

void SwitchApplicationParameters(ApplicationSharedPtr app,
                                 const uint32_t app_id,
                                 const size_t device_id,
                                 const std::string& mac_address) {
  utils::SharedPtr<ApplicationImpl> application =
      ApplicationSharedPtr::dynamic_pointer_cast<ApplicationImpl>(app);
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
    utils::SharedPtr<usage_statistics::StatisticsManager> statistics_manager,
    ApplicationManager& application_manager)
    : grammar_id_(0)
    , hmi_app_id_(0)
    , app_id_(application_id)
    , active_message_(NULL)
    , is_media_(false)
    , is_navi_(false)
    , mobile_projection_enabled_(false)
    , video_streaming_approved_(false)
    , audio_streaming_approved_(false)
    , video_streaming_allowed_(false)
    , audio_streaming_allowed_(false)
    , video_streaming_suspended_(true)
    , audio_streaming_suspended_(true)
    , is_app_allowed_(true)
    , has_been_activated_(false)
    , tts_properties_in_none_(false)
    , tts_properties_in_full_(false)
    , is_foreground_(false)
    , is_application_data_changed_(false)
    , put_file_in_none_count_(0)
    , delete_file_in_none_count_(0)
    , list_files_in_none_count_(0)
    , mac_address_(mac_address)
    , device_id_(device_id)
    , usage_report_(mobile_app_id, statistics_manager)
    , protocol_version_(
          protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3)
    , is_voice_communication_application_(false)
    , is_resuming_(false)
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
    , application_manager_(application_manager) {
  cmd_number_to_time_limits_[mobile_apis::FunctionID::ReadDIDID] = {
      date_time::DateTime::getCurrentTime(), 0};
  cmd_number_to_time_limits_[mobile_apis::FunctionID::GetVehicleDataID] = {
      date_time::DateTime::getCurrentTime(), 0};

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
  subscribed_vehicle_info_.clear();
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
  return mobile_api::HMILevel::HMI_FULL == hmi_level();
}

bool ApplicationImpl::is_audio() const {
  return is_media_application() || is_voice_communication_supported() ||
         is_navi();
}

void ApplicationImpl::ChangeSupportingAppHMIType() {
  is_navi_ = false;
  is_voice_communication_application_ = false;
  mobile_projection_enabled_ = false;
  const smart_objects::SmartObject& array_app_types = *app_types_;
  uint32_t lenght_app_types = array_app_types.length();

  for (uint32_t i = 0; i < lenght_app_types; ++i) {
    if (mobile_apis::AppHMIType::NAVIGATION ==
        static_cast<mobile_apis::AppHMIType::eType>(
            array_app_types[i].asUInt())) {
      is_navi_ = true;
    }
    if (mobile_apis::AppHMIType::COMMUNICATION ==
        static_cast<mobile_apis::AppHMIType::eType>(
            array_app_types[i].asUInt())) {
      is_voice_communication_application_ = true;
    }
    if (mobile_apis::AppHMIType::PROJECTION ==
        static_cast<mobile_apis::AppHMIType::eType>(
            array_app_types[i].asUInt())) {
      mobile_projection_enabled_ = true;
    }
  }
}

void ApplicationImpl::set_is_navi(bool allow) {
  is_navi_ = allow;
}

bool ApplicationImpl::is_voice_communication_supported() const {
  return is_voice_communication_application_;
}

void ApplicationImpl::set_voice_communication_supported(
    bool is_voice_communication_supported) {
  is_voice_communication_application_ = is_voice_communication_supported;
}

bool ApplicationImpl::IsAudioApplication() const {
  return is_media_ || is_voice_communication_application_ || is_navi_ ||
         mobile_projection_enabled_;
}

bool ApplicationImpl::IsVideoApplication() const {
  return is_navi_ || mobile_projection_enabled_;
}

void ApplicationImpl::SetRegularState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  state_.AddState(state);
}

void ApplicationImpl::RemovePostponedState() {
  LOG4CXX_AUTO_TRACE(logger_);
  state_.RemoveState(HmiState::STATE_ID_POSTPONED);
}

void ApplicationImpl::SetPostponedState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  state_.AddState(state);
}

void ApplicationImpl::set_mobile_projection_enabled(bool option) {
  LOG4CXX_AUTO_TRACE(logger_);
  mobile_projection_enabled_ = option;
}

bool ApplicationImpl::mobile_projection_enabled() const {
  return mobile_projection_enabled_;
}

struct StateIDComparator {
  HmiState::StateID state_id_;
  StateIDComparator(HmiState::StateID state_id) : state_id_(state_id) {}
  bool operator()(const HmiStatePtr cur) {
    return cur->state_id() == state_id_;
  }
};

void ApplicationImpl::AddHMIState(HmiStatePtr state) {
  LOG4CXX_AUTO_TRACE(logger_);
  state_.AddState(state);
}

void ApplicationImpl::RemoveHMIState(HmiState::StateID state_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  state_.RemoveState(state_id);
}

const HmiStatePtr ApplicationImpl::CurrentHmiState() const {
  return state_.GetState(HmiState::STATE_ID_CURRENT);
}

const HmiStatePtr ApplicationImpl::RegularHmiState() const {
  return state_.GetState(HmiState::STATE_ID_REGULAR);
}

const HmiStatePtr ApplicationImpl::PostponedHmiState() const {
  return state_.GetState(HmiState::STATE_ID_POSTPONED);
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

const mobile_api::HMILevel::eType ApplicationImpl::hmi_level() const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state = CurrentHmiState();
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

const mobile_api::SystemContext::eType ApplicationImpl::system_context() const {
  using namespace mobile_apis;
  const HmiStatePtr hmi_state = CurrentHmiState();
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

const std::string& ApplicationImpl::mac_address() const {
  return mac_address_;
}

void ApplicationImpl::set_version(const Version& ver) {
  version_ = ver;
}

void ApplicationImpl::set_name(const custom_str::CustomString& name) {
  app_name_ = name;
}

void ApplicationImpl::set_is_media_application(bool is_media) {
  is_media_ = is_media;
}

bool IsTTSState(const HmiStatePtr state) {
  return state->state_id() == HmiState::STATE_ID_TTS_SESSION;
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
  LOG4CXX_AUTO_TRACE(logger_);

  if (ServiceType::kMobileNav == service_type) {
    // See StartStreaming(). We issue SetVideoConfig and StartStream
    // only when streaming is not approved yet
    if (!video_streaming_approved()) {
      LOG4CXX_TRACE(logger_, "Video streaming not approved");
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
  LOG4CXX_AUTO_TRACE(logger_);

  if (ServiceType::kMobileNav == service_type) {
    LOG4CXX_TRACE(logger_, "ServiceType = Video");
    if (!video_streaming_approved()) {
      LOG4CXX_TRACE(logger_, "Video streaming not approved");
      MessageHelper::SendNaviStartStream(app_id(), application_manager_);
      set_video_stream_retry_number(0);
    }
  } else if (ServiceType::kAudio == service_type) {
    LOG4CXX_TRACE(logger_, "ServiceType = Audio");
    if (!audio_streaming_approved()) {
      LOG4CXX_TRACE(logger_, "Audio streaming not approved");
      MessageHelper::SendAudioStartStream(app_id(), application_manager_);
      set_audio_stream_retry_number(0);
    }
  }
}

void ApplicationImpl::StopStreamingForce(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

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
  LOG4CXX_AUTO_TRACE(logger_);

  SuspendStreaming(service_type);

  if (service_type == ServiceType::kMobileNav && video_streaming_approved()) {
    StopNaviStreaming();
  } else if (service_type == ServiceType::kAudio &&
             audio_streaming_approved()) {
    StopAudioStreaming();
  }
}

void ApplicationImpl::StopNaviStreaming() {
  LOG4CXX_AUTO_TRACE(logger_);
  video_stream_suspend_timer_.Stop();
  MessageHelper::SendNaviStopStream(app_id(), application_manager_);
  set_video_streaming_approved(false);
  set_video_stream_retry_number(0);
}

void ApplicationImpl::StopAudioStreaming() {
  LOG4CXX_AUTO_TRACE(logger_);
  audio_stream_suspend_timer_.Stop();
  MessageHelper::SendAudioStopStream(app_id(), application_manager_);
  set_audio_streaming_approved(false);
  set_audio_stream_retry_number(0);
}

void ApplicationImpl::SuspendStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  if (ServiceType::kMobileNav == service_type) {
    video_stream_suspend_timer_.Stop();
    application_manager_.OnAppStreaming(app_id(), service_type, false);
    sync_primitives::AutoLock lock(video_streaming_suspended_lock_);
    video_streaming_suspended_ = true;
  } else if (ServiceType::kAudio == service_type) {
    audio_stream_suspend_timer_.Stop();
    application_manager_.OnAppStreaming(app_id(), service_type, false);
    sync_primitives::AutoLock lock(audio_streaming_suspended_lock_);
    audio_streaming_suspended_ = true;
  }
  MessageHelper::SendOnDataStreaming(service_type, false, application_manager_);
}

void ApplicationImpl::WakeUpStreaming(
    protocol_handler::ServiceType service_type) {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);

  if (ServiceType::kMobileNav == service_type) {
    sync_primitives::AutoLock lock(video_streaming_suspended_lock_);
    if (video_streaming_suspended_) {
      application_manager_.OnAppStreaming(app_id(), service_type, true);
      MessageHelper::SendOnDataStreaming(
          ServiceType::kMobileNav, true, application_manager_);
      video_streaming_suspended_ = false;
    }
    video_stream_suspend_timer_.Start(video_stream_suspend_timeout_,
                                      timer::kPeriodic);
  } else if (ServiceType::kAudio == service_type) {
    sync_primitives::AutoLock lock(audio_streaming_suspended_lock_);
    if (audio_streaming_suspended_) {
      application_manager_.OnAppStreaming(app_id(), service_type, true);
      MessageHelper::SendOnDataStreaming(
          ServiceType::kAudio, true, application_manager_);
      audio_streaming_suspended_ = false;
    }
    audio_stream_suspend_timer_.Start(audio_stream_suspend_timeout_,
                                      timer::kPeriodic);
  }
}

void ApplicationImpl::OnVideoStreamSuspend() {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "Suspend video streaming by timer");
  SuspendStreaming(ServiceType::kMobileNav);
}

void ApplicationImpl::OnAudioStreamSuspend() {
  using namespace protocol_handler;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "Suspend audio streaming by timer");
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

void ApplicationImpl::set_device(connection_handler::DeviceHandle device) {
  device_id_ = device;
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

bool ApplicationImpl::AddFile(const AppFile& file) {
  if (app_files_.count(file.file_name) == 0) {
    LOG4CXX_INFO(logger_,
                 "AddFile file " << file.file_name << " File type is "
                                 << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::UpdateFile(const AppFile& file) {
  if (app_files_.count(file.file_name) != 0) {
    LOG4CXX_INFO(logger_,
                 "UpdateFile file " << file.file_name << " File type is "
                                    << file.file_type);
    app_files_[file.file_name] = file;
    return true;
  }
  return false;
}

bool ApplicationImpl::DeleteFile(const std::string& file_name) {
  AppFilesMap::iterator it = app_files_.find(file_name);
  if (it != app_files_.end()) {
    LOG4CXX_INFO(logger_,
                 "DeleteFile file " << it->second.file_name << " File type is "
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
  sync_primitives::AutoLock lock(button_lock_);
  return subscribed_buttons_.insert(btn_name).second;
}

bool ApplicationImpl::IsSubscribedToButton(
    mobile_apis::ButtonName::eType btn_name) {
  sync_primitives::AutoLock lock(button_lock_);
  std::set<mobile_apis::ButtonName::eType>::iterator it =
      subscribed_buttons_.find(btn_name);
  return (subscribed_buttons_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromButton(
    mobile_apis::ButtonName::eType btn_name) {
  sync_primitives::AutoLock lock(button_lock_);
  return subscribed_buttons_.erase(btn_name);
}

bool ApplicationImpl::SubscribeToIVI(uint32_t vehicle_info_type) {
  sync_primitives::AutoLock lock(vi_lock_);
  return subscribed_vehicle_info_
      .insert(
           static_cast<mobile_apis::VehicleDataType::eType>(vehicle_info_type))
      .second;
}

bool ApplicationImpl::IsSubscribedToIVI(uint32_t vehicle_info_type) const {
  sync_primitives::AutoLock lock(vi_lock_);
  VehicleInfoSubscriptions::const_iterator it = subscribed_vehicle_info_.find(
      static_cast<mobile_apis::VehicleDataType::eType>(vehicle_info_type));
  return (subscribed_vehicle_info_.end() != it);
}

bool ApplicationImpl::UnsubscribeFromIVI(uint32_t vehicle_info_type) {
  sync_primitives::AutoLock lock(vi_lock_);
  return subscribed_vehicle_info_.erase(
      static_cast<mobile_apis::VehicleDataType::eType>(vehicle_info_type));
}

UsageStatistics& ApplicationImpl::usage_report() {
  return usage_report_;
}

bool ApplicationImpl::AreCommandLimitsExceeded(
    mobile_apis::FunctionID::eType cmd_id, TLimitSource source) {
  TimevalStruct current = date_time::DateTime::getCurrentTime();
  switch (source) {
    // In case of config file values there is COMMON limitations for number of
    // commands per certain time in seconds, i.e. 5 requests per 10 seconds with
    // any interval between them
    case CONFIG_FILE: {
      CommandNumberTimeLimit::iterator it =
          cmd_number_to_time_limits_.find(cmd_id);
      if (cmd_number_to_time_limits_.end() == it) {
        LOG4CXX_WARN(logger_,
                     "Limits for command id " << cmd_id << "had not been set.");
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
        LOG4CXX_INFO(logger_, "No restrictions for request");
        return false;
      }

      LOG4CXX_INFO(logger_,
                   "Time Info: "
                       << "\n Current: " << current.tv_sec << "\n Limit: ("
                       << limit.first.tv_sec << "," << limit.second
                       << ")"
                          "\n frequency_restrictions: ("
                       << frequency_restrictions.first << ","
                       << frequency_restrictions.second << ")");
      if (current.tv_sec < limit.first.tv_sec + frequency_restrictions.second) {
        if (limit.second < frequency_restrictions.first) {
          ++limit.second;
          return false;
        }
        return true;
      }

      limit.first = current;
      limit.second = 1;

      return false;

      break;
    }
    // In case of policy table values, there is EVEN limitation for number of
    // commands per minute, e.g. 10 command per minute i.e. 1 command per 6 sec
    case POLICY_TABLE: {
      const policy::PolicyHandlerInterface& policy_handler =
          application_manager_.GetPolicyHandler();
      std::string priority;
      policy_handler.GetPriority(policy_app_id(), &priority);
      uint32_t cmd_limit = policy_handler.GetNotificationsNumber(priority);

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
      if (static_cast<uint32_t>(current.tv_sec - limit.first.tv_sec) <
          minute / cmd_limit) {
        return true;
      }

      cmd_number_to_time_limits_[cmd_id] = {current, dummy_limit};

      return false;
      break;
    }
    default: {
      LOG4CXX_WARN(logger_, "Limit source is not implemented.");
      break;
    }
  }

  return true;
}

DataAccessor<ButtonSubscriptions> ApplicationImpl::SubscribedButtons() const {
  return DataAccessor<ButtonSubscriptions>(subscribed_buttons_, button_lock_);
}

DataAccessor<VehicleInfoSubscriptions> ApplicationImpl::SubscribedIVI() const {
  return DataAccessor<VehicleInfoSubscriptions>(subscribed_vehicle_info_,
                                                vi_lock_);
}

const std::string& ApplicationImpl::curHash() const {
  return hash_val_;
}

bool ApplicationImpl::is_application_data_changed() const {
  return is_application_data_changed_;
}

void ApplicationImpl::SetInitialState(HmiStatePtr state) {
  state_.InitState(state);
}

void ApplicationImpl::set_is_application_data_changed(
    bool state_application_data) {
  is_application_data_changed_ = state_application_data;
}

void ApplicationImpl::UpdateHash() {
  LOG4CXX_AUTO_TRACE(logger_);
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

  if (file_system::DirectoryExists(directory_name)) {
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
        LOG4CXX_INFO(logger_, "DeleteFile file " << file_name);
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

      LOG4CXX_INFO(logger_,
                   "Loaded persistent file "
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
    int32_t cmd_id, const SoftButtonID& softbuttons_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  if (static_cast<int32_t>(mobile_apis::FunctionID::ScrollableMessageID) ==
      cmd_id) {
    CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);
    if (cmd_softbuttonid_.end() == it) {
      cmd_softbuttonid_[cmd_id] = softbuttons_id;
    }
  } else {
    cmd_softbuttonid_[cmd_id] = softbuttons_id;
  }
}

bool ApplicationImpl::IsSubscribedToSoftButton(const uint32_t softbutton_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.begin();
  for (; it != cmd_softbuttonid_.end(); ++it) {
    if ((it->second).find(softbutton_id) != (it->second).end()) {
      return true;
    }
  }
  return false;
}

void ApplicationImpl::UnsubscribeFromSoftButtons(int32_t cmd_id) {
  sync_primitives::AutoLock lock(cmd_softbuttonid_lock_);
  CommandSoftButtonID::iterator it = cmd_softbuttonid_.find(cmd_id);
  if (it != cmd_softbuttonid_.end()) {
    cmd_softbuttonid_.erase(it);
  }
}

#ifdef SDL_REMOTE_CONTROL

void ApplicationImpl::set_system_context(
    const mobile_api::SystemContext::eType& system_context) {
  const HmiStatePtr hmi_state = CurrentHmiState();
  hmi_state->set_system_context(system_context);
}

void ApplicationImpl::set_audio_streaming_state(
    const mobile_api::AudioStreamingState::eType& state) {
  if (!(is_media_application() || is_navi()) &&
      state != mobile_api::AudioStreamingState::NOT_AUDIBLE) {
    LOG4CXX_WARN(logger_,
                 "Trying to set audio streaming state"
                 " for non-media application to different from NOT_AUDIBLE");
    return;
  }
  CurrentHmiState()->set_audio_streaming_state(state);
}

void ApplicationImpl::set_hmi_level(
    const mobile_api::HMILevel::eType& new_hmi_level) {
  using namespace mobile_apis;
  const HMILevel::eType current_hmi_level = hmi_level();
  if (HMILevel::HMI_NONE != current_hmi_level &&
      HMILevel::HMI_NONE == new_hmi_level) {
    put_file_in_none_count_ = 0;
    delete_file_in_none_count_ = 0;
    list_files_in_none_count_ = 0;
  }
  ApplicationSharedPtr app = application_manager_.application(app_id());
  DCHECK_OR_RETURN_VOID(app)
  application_manager_.state_controller().SetRegularState(app, new_hmi_level);
  LOG4CXX_INFO(logger_, "hmi_level = " << new_hmi_level);
  usage_report_.RecordHmiStateChanged(new_hmi_level);
}

const VehicleInfoSubscriptions& ApplicationImpl::SubscribesIVI() const {
  return subscribed_vehicle_info_;
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
    extensions_.push_back(extension);
    return true;
  }
  return false;
}

bool ApplicationImpl::RemoveExtension(AppExtensionUID uid) {
  for (std::list<AppExtensionPtr>::iterator it = extensions_.begin();
       extensions_.end() != it;
       ++it) {
    if ((*it)->uid() == uid) {
      extensions_.erase(it);
      return true;
    }
  }
  return false;
}

void ApplicationImpl::RemoveExtensions() {
  application_manager_.GetPluginManager().RemoveAppExtension(app_id_);
}
#endif  // SDL_REMOTE_CONTROL

}  // namespace application_manager
