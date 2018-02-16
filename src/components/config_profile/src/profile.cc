/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "config_profile/profile.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>

#include <string>

#include "config_profile/ini_file.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/file_system.h"
#include "config_profile/profile_default_values.h"

#ifdef ENABLE_SECURITY
#include <openssl/ssl.h>
#endif  // ENABLE_SECURITY

using namespace profile::default_values;


namespace {
#define LOG_UPDATED_VALUE(value, key, section)                              \
  {                                                                         \
    LOG4CXX_INFO(logger_,                                                   \
                 "Setting value '" << value << "' for key '" << key         \
                                   << "' in section '" << section << "'."); \
  }

#define LOG_UPDATED_BOOL_VALUE(value, key, section)                            \
  {                                                                            \
    LOG4CXX_INFO(logger_,                                                      \
                 "Setting value '" << std::boolalpha << value << "' for key '" \
                                   << key << "' in section '" << section       \
                                   << "'.");                                   \
  }

}

namespace profile {

CREATE_LOGGERPTR_GLOBAL(logger_, "Profile")

Profile::Profile()
    : sdl_version_(default_values::kDefaultSDLVersion)
    , launch_hmi_(true)
    ,
#ifdef WEB_HMI
    link_to_web_hmi_(default_values::kDefaultLinkToWebHMI)
    ,
#endif  // WEB_HMI
    app_config_folder_()
    , app_storage_folder_()
    , app_resource_folder_()
    , app_icons_folder_()
    , app_icons_folder_max_size_(default_values::kDefaultAppIconsFolderMaxSize)
    , app_icons_amount_to_remove_(default_values::kDefaultAppIconsAmountToRemove)
    , maximum_control_payload_size_(default_values::kDefaultMaximumControlPayloadSize)
    , maximum_rpc_payload_size_(default_values::kDefaultMaximumRpcPayloadSize)
    , maximum_audio_payload_size_(default_values::kDefaultMaximumAudioPayloadSize)
    , maximum_video_payload_size_(default_values::kDefaultMaximumVideoPayloadSize)
    , config_file_name_(default_values::kDefaultConfigFileName)
    , server_address_(default_values::kDefaultServerAddress)
    , server_port_(default_values::kDefaultServerPort)
    , video_streaming_port_(default_values::kDefaultVideoStreamingPort)
    , audio_streaming_port_(default_values::kDefaultAudioStreamingPort)
    , stop_streaming_timeout_(default_values::kDefaultStopStreamingTimeout)
    , time_testing_port_(default_values::kDefaultTimeTestingPort)
    , hmi_capabilities_file_name_(default_values::kDefaultHmiCapabilitiesFileName)
    , help_prompt_()
    , time_out_promt_()
    , min_tread_stack_size_(threads::Thread::kMinStackSize)
    , is_mixing_audio_supported_(false)
    , is_redecoding_enabled_(false)
    , max_cmd_id_(default_values::kDefaultMaxCmdId)
    , default_timeout_(default_values::kDefaultTimeout)
    , app_resuming_timeout_(default_values::kDefaultAppResumingTimeout)
    , app_resumption_save_persistent_data_timeout_(
          default_values::kDefaultAppSavePersistentDataTimeout)
    , app_dir_quota_(default_values::kDefaultDirQuota)
    , app_hmi_level_none_time_scale_max_requests_(
          default_values::kDefaultAppHmiLevelNoneTimeScaleMaxRequests)
    , app_hmi_level_none_requests_time_scale_(
          default_values::kDefaultAppHmiLevelNoneRequestsTimeScale)
    , app_time_scale_max_requests_(default_values::kDefaultAppTimeScaleMaxRequests)
    , app_requests_time_scale_(default_values::kDefaultAppRequestsTimeScale)
    , pending_requests_amount_(default_values::kDefaultPendingRequestsAmount)
    , put_file_in_none_(default_values::kDefaultPutFileRequestInNone)
    , delete_file_in_none_(default_values::kDefaultDeleteFileRequestInNone)
    , list_files_in_none_(default_values::kDefaultListFilesRequestInNone)
    , list_files_response_size_(default_values::kDefaultListFilesResponseSize)
    , app_info_storage_(default_values::kDefaultAppInfoFileName)
    , heart_beat_timeout_(default_values::kDefaultHeartBeatTimeout)
    , max_supported_protocol_version_(default_values::kDefaultMaxSupportedProtocolVersion)
    , policy_snapshot_file_name_(default_values::kDefaultPoliciesSnapshotFileName)
    , enable_policy_(false)
    , transport_manager_disconnect_timeout_(
          default_values::kDefaultTransportManagerDisconnectTimeout)
    , use_last_state_(false)
    , supported_diag_modes_()
    , system_files_path_(default_values::kDefaultSystemFilesPath)
    , transport_manager_tcp_adapter_port_(kDefautTransportManagerTCPPort)
    , tts_delimiter_(default_values::kDefaultTtsDelimiter)
    , audio_data_stopped_timeout_(default_values::kDefaultAudioDataStoppedTimeout)
    , video_data_stopped_timeout_(default_values::kDefaultVideoDataStoppedTimeout)
    , event_mq_name_(default_values::kDefaultEventMQ)
    , ack_mq_name_(default_values::kDefaultAckMQ)
    , recording_file_source_(default_values::kDefaultRecordingFileSourceName)
    , recording_file_name_(default_values::kDefaultRecordingFileName)
    , application_list_update_timeout_(default_values::kDefaultApplicationListUpdateTimeout)
    , max_thread_pool_size_(default_values::kDefaultMaxThreadPoolSize)
    , default_hub_protocol_index_(default_values::kDefaultHubProtocolIndex)
    , iap_legacy_protocol_mask_(default_values::kDefaultLegacyProtocolMask)
    , iap_hub_protocol_mask_(default_values::kDefaultHubProtocolMask)
    , iap_pool_protocol_mask_(default_values::kDefaultPoolProtocolMask)
    , iap_system_config_(default_values::kDefaultIAPSystemConfig)
    , iap2_system_config_(default_values::kDefaultIAP2SystemConfig)
    , iap2_hub_connect_attempts_(default_values::kDefaultIAP2HubConnectAttempts)
    , iap_hub_connection_wait_timeout_(default_values::kDefaultIAPHubConnectionWaitTimeout)
    , tts_global_properties_timeout_(default_values::kDefaultTTSGlobalPropertiesTimeout)
    , attempts_to_open_policy_db_(default_values::kDefaultAttemptsToOpenPolicyDB)
    , open_attempt_timeout_ms_(default_values::kDefaultAttemptsToOpenPolicyDB)
    , resumption_delay_before_ign_(default_values::kDefaultResumptionDelayBeforeIgn)
    , resumption_delay_after_ign_(default_values::kDefaultResumptionDelayAfterIgn)
    , hash_string_size_(default_values::kDefaultHashStringSize)
    , use_db_for_resumption_(false)
    , attempts_to_open_resumption_db_(default_values::kDefaultAttemptsToOpenResumptionDB)
    , open_attempt_timeout_ms_resumption_db_(
          default_values::kDefaultOpenAttemptTimeoutMsResumptionDB)
    , app_launch_wait_time_(default_values::kDefaultAppLaunchWaitTime)
    , app_launch_max_retry_attempt_(default_values::kDefaultAppLaunchMaxRetryAttempt)
    , app_launch_retry_wait_time_(default_values::kDefaultAppLaunchRetryWaitTime)
    , remove_bundle_id_attempts_(default_values::kDefaultRemoveBundleIDattempts)
    , max_number_of_ios_device_(default_values::kDefaultMaxNumberOfiOSDevice)
    , wait_time_between_apps_(default_values::kDefaultWaitTimeBetweenApps)
    , enable_app_launch_ios_(default_values::kDefaultEnableAppLaunchIOS)
    , app_tranport_change_timer_(default_values::kDefaultAppTransportChangeTimer)
    , app_tranport_change_timer_addition_(
          default_values::kDefaultAppTransportChangeTimerAddition)
    , error_occured_(false)
    , error_description_() {
  // SDL version
  ReadStringValue(
      &sdl_version_, default_values::kDefaultSDLVersion, kMainSection, kSDLVersionKey);
}

Profile::~Profile() {}

void Profile::set_config_file_name(const std::string& file_name) {
  if (false == file_name.empty()) {
    config_file_name_ = file_name;
    UpdateValues();
  }
}

const std::string& Profile::config_file_name() const {
  return config_file_name_;
}

const std::string& Profile::sdl_version() const {
  return sdl_version_;
}

bool Profile::launch_hmi() const {
  return launch_hmi_;
}

#ifdef WEB_HMI
std::string Profile::link_to_web_hmi() const {
  return link_to_web_hmi_;
}
#endif  // WEB_HMI

const std::string& Profile::app_config_folder() const {
  return app_config_folder_;
}

const std::string& Profile::app_storage_folder() const {
  return app_storage_folder_;
}

const std::string& Profile::app_resource_folder() const {
  return app_resource_folder_;
}

bool Profile::enable_protocol_4() const {
  return max_supported_protocol_version_ >= 4;
}

const std::string& Profile::app_icons_folder() const {
  return app_icons_folder_;
}

const uint32_t& Profile::app_icons_folder_max_size() const {
  return app_icons_folder_max_size_;
}

const uint32_t& Profile::app_icons_amount_to_remove() const {
  return app_icons_amount_to_remove_;
}

size_t Profile::maximum_control_payload_size() const {
  return maximum_control_payload_size_;
}

size_t Profile::maximum_rpc_payload_size() const {
  return maximum_rpc_payload_size_;
}

size_t Profile::maximum_audio_payload_size() const {
  return maximum_audio_payload_size_;
}

size_t Profile::maximum_video_payload_size() const {
  return maximum_video_payload_size_;
}

const std::string& Profile::hmi_capabilities_file_name() const {
  return hmi_capabilities_file_name_;
}

const std::string& Profile::server_address() const {
  return server_address_;
}

const std::vector<std::string>& Profile::help_prompt() const {
  return help_prompt_;
}

const std::vector<std::string>& Profile::time_out_promt() const {
  return time_out_promt_;
}

const std::vector<std::string>& Profile::vr_commands() const {
  return vr_commands_;
}

const uint32_t& Profile::max_cmd_id() const {
  return max_cmd_id_;
}

const uint32_t& Profile::default_timeout() const {
  return default_timeout_;
}

const uint32_t& Profile::app_resuming_timeout() const {
  return app_resuming_timeout_;
}

const uint32_t& Profile::app_resumption_save_persistent_data_timeout() const {
  return app_resumption_save_persistent_data_timeout_;
}

const std::string& Profile::vr_help_title() const {
  return vr_help_title_;
}

const uint16_t& Profile::server_port() const {
  return server_port_;
}

const uint16_t Profile::video_streaming_port() const {
  return video_streaming_port_;
}

const uint16_t Profile::audio_streaming_port() const {
  return audio_streaming_port_;
}

uint32_t Profile::stop_streaming_timeout() const {
  return stop_streaming_timeout_;
}

const uint16_t& Profile::time_testing_port() const {
  return time_testing_port_;
}

const uint64_t Profile::thread_min_stack_size() const {
  return min_tread_stack_size_;
}

bool Profile::is_mixing_audio_supported() const {
  return is_mixing_audio_supported_;
}

const uint32_t& Profile::app_dir_quota() const {
  return app_dir_quota_;
}

bool Profile::is_redecoding_enabled() const {
  return is_redecoding_enabled_;
}

const std::string& Profile::video_server_type() const {
  return video_consumer_type_;
}

const std::string& Profile::audio_server_type() const {
  return audio_consumer_type_;
}

const std::string& Profile::named_video_pipe_path() const {
  return named_video_pipe_path_;
}

const std::string& Profile::named_audio_pipe_path() const {
  return named_audio_pipe_path_;
}

const uint32_t& Profile::app_hmi_level_none_time_scale() const {
  return app_hmi_level_none_requests_time_scale_;
}

const uint32_t& Profile::app_hmi_level_none_time_scale_max_requests() const {
  return app_hmi_level_none_time_scale_max_requests_;
}

const std::string& Profile::video_stream_file() const {
  return video_stream_file_;
}

const std::string& Profile::audio_stream_file() const {
  return audio_stream_file_;
}

const uint32_t Profile::audio_data_stopped_timeout() const {
  return audio_data_stopped_timeout_;
}

const uint32_t Profile::video_data_stopped_timeout() const {
  return video_data_stopped_timeout_;
}

const uint32_t& Profile::app_time_scale() const {
  return app_requests_time_scale_;
}

const uint32_t& Profile::app_time_scale_max_requests() const {
  return app_time_scale_max_requests_;
}

const uint32_t& Profile::pending_requests_amount() const {
  return pending_requests_amount_;
}

const uint32_t& Profile::put_file_in_none() const {
  return put_file_in_none_;
}

const uint32_t& Profile::delete_file_in_none() const {
  return delete_file_in_none_;
}

const uint32_t& Profile::list_files_in_none() const {
  return list_files_in_none_;
}

const uint32_t& Profile::list_files_response_size() const {
  return list_files_response_size_;
}

const std::string& Profile::app_info_storage() const {
  return app_info_storage_;
}

uint32_t Profile::heart_beat_timeout() const {
  return heart_beat_timeout_;
}

uint16_t Profile::max_supported_protocol_version() const {
  return max_supported_protocol_version_;
}

const std::string& Profile::preloaded_pt_file() const {
  return preloaded_pt_file_;
}

const std::string& Profile::policies_snapshot_file_name() const {
  return policy_snapshot_file_name_;
}

bool Profile::enable_policy() const {
  return enable_policy_;
}

uint32_t Profile::transport_manager_disconnect_timeout() const {
  return transport_manager_disconnect_timeout_;
}

bool Profile::use_last_state() const {
  return use_last_state_;
}

const std::string& Profile::system_files_path() const {
  return system_files_path_;
}

const std::string& Profile::plugins_folder() const {
  return plugins_folder_;
}
const std::vector<uint32_t>& Profile::supported_diag_modes() const {
  return supported_diag_modes_;
}

uint16_t Profile::transport_manager_tcp_adapter_port() const {
  return transport_manager_tcp_adapter_port_;
}

const std::string& Profile::tts_delimiter() const {
  return tts_delimiter_;
}

const std::string& Profile::recording_file_source() const {
  return recording_file_source_;
}
const std::string& Profile::recording_file_name() const {
  return recording_file_name_;
}

const std::string& Profile::event_mq_name() const {
  return event_mq_name_;
}

const std::string& Profile::ack_mq_name() const {
  return ack_mq_name_;
}

uint32_t Profile::application_list_update_timeout() const {
  return application_list_update_timeout_;
}

const std::pair<uint32_t, int32_t>& Profile::read_did_frequency() const {
  return read_did_frequency_;
}

const std::pair<uint32_t, int32_t>& Profile::get_vehicle_data_frequency()
    const {
  return get_vehicle_data_frequency_;
}

const std::pair<uint32_t, int32_t>& Profile::start_stream_retry_amount() const {
  return start_stream_retry_amount_;
}

uint32_t Profile::thread_pool_size() const {
  return max_thread_pool_size_;
}

uint32_t Profile::default_hub_protocol_index() const {
  return default_hub_protocol_index_;
}

const std::string& Profile::iap_legacy_protocol_mask() const {
  return iap_legacy_protocol_mask_;
}

const std::string& Profile::iap_hub_protocol_mask() const {
  return iap_hub_protocol_mask_;
}

const std::string& Profile::iap_pool_protocol_mask() const {
  return iap_pool_protocol_mask_;
}

const std::string& Profile::iap_system_config() const {
  return iap_system_config_;
}

const std::string& Profile::iap2_system_config() const {
  return iap2_system_config_;
}

uint32_t Profile::iap2_hub_connect_attempts() const {
  return iap2_hub_connect_attempts_;
}

uint32_t Profile::iap_hub_connection_wait_timeout() const {
  return iap_hub_connection_wait_timeout_;
}

size_t Profile::maximum_payload_size() const {
  size_t maximum_payload_size = 0;
  ReadUIntValue(&maximum_payload_size,
                default_values::kDefaultMaximumPayloadSize,
                kProtocolHandlerSection,
                kMaximumPayloadSizeKey);
  return maximum_payload_size;
}

size_t Profile::message_frequency_count() const {
  size_t message_frequency_count = 0;
  ReadUIntValue(&message_frequency_count,
                default_values::kDefaultFrequencyCount,
                kProtocolHandlerSection,
                kFrequencyCount);
  return message_frequency_count;
}

size_t Profile::message_frequency_time() const {
  size_t message_frequency_time = 0;
  ReadUIntValue(&message_frequency_time,
                default_values::kDefaultFrequencyTime,
                kProtocolHandlerSection,
                kFrequencyTime);
  return message_frequency_time;
}

bool Profile::malformed_message_filtering() const {
  bool malformed_message_filtering = 0;
  ReadBoolValue(&malformed_message_filtering,
                kDefaulMalformedMessageFiltering,
                kProtocolHandlerSection,
                kMalformedMessageFiltering);
  return malformed_message_filtering;
}

size_t Profile::malformed_frequency_count() const {
  size_t malformed_frequency_count = 0;
  ReadUIntValue(&malformed_frequency_count,
                default_values::kDefaultMalformedFrequencyCount,
                kProtocolHandlerSection,
                kMalformedFrequencyCount);
  return malformed_frequency_count;
}

size_t Profile::malformed_frequency_time() const {
  size_t malformed_frequency_time = 0;
  ReadUIntValue(&malformed_frequency_time,
                default_values::kDefaultMalformedFrequencyTime,
                kProtocolHandlerSection,
                kMalformedFrequencyTime);
  return malformed_frequency_time;
}
uint32_t Profile::multiframe_waiting_timeout() const {
  uint32_t multiframe_waiting_timeout = 0;
  ReadUIntValue(&multiframe_waiting_timeout,
                default_values::kDefaultExpectedConsecutiveFramesTimeout,
                kProtocolHandlerSection,
                kExpectedConsecutiveFramesTimeout);
  return multiframe_waiting_timeout;
}

uint16_t Profile::attempts_to_open_policy_db() const {
  return attempts_to_open_policy_db_;
}

uint16_t Profile::open_attempt_timeout_ms() const {
  return open_attempt_timeout_ms_;
}

uint32_t Profile::resumption_delay_before_ign() const {
  return resumption_delay_before_ign_;
}

const uint32_t Profile::resumption_delay_after_ign() const {
  return resumption_delay_after_ign_;
}

uint32_t Profile::hash_string_size() const {
  return hash_string_size_;
}

uint16_t Profile::tts_global_properties_timeout() const {
  return tts_global_properties_timeout_;
}

#ifdef ENABLE_SECURITY

const std::string& Profile::cert_path() const {
  return cert_path_;
}

const std::string& Profile::ca_cert_path() const {
  return ca_cert_path_;
}

const std::string& Profile::ssl_mode() const {
  return ssl_mode_;
}

const std::string& Profile::key_path() const {
  return key_path_;
}

const std::string& Profile::ciphers_list() const {
  return ciphers_list_;
}

bool Profile::verify_peer() const {
  return verify_peer_;
}

size_t Profile::update_before_hours() const {
  return update_before_hours_;
}

const std::string& Profile::security_manager_protocol_name() const {
  return security_manager_protocol_name_;
}

const std::vector<int>& Profile::force_protected_service() const {
  return force_protected_service_;
}

const std::vector<int>& Profile::force_unprotected_service() const {
  return force_unprotected_service_;
}
#endif  // ENABLE_SECURITY

bool Profile::logs_enabled() const {
  return logs_enabled_;
}

bool Profile::use_db_for_resumption() const {
  return use_db_for_resumption_;
}

uint16_t Profile::attempts_to_open_resumption_db() const {
  return attempts_to_open_resumption_db_;
}

uint16_t Profile::open_attempt_timeout_ms_resumption_db() const {
  return open_attempt_timeout_ms_resumption_db_;
}

const uint16_t Profile::app_launch_max_retry_attempt() const {
  return app_launch_max_retry_attempt_;
}

const uint16_t Profile::app_launch_retry_wait_time() const {
  return app_launch_retry_wait_time_;
}

const uint16_t Profile::app_launch_wait_time() const {
  return app_launch_wait_time_;
}

const bool Profile::enable_app_launch_ios() const {
  return enable_app_launch_ios_;
}

uint32_t Profile::app_transport_change_timer() const {
  return app_tranport_change_timer_;
}

uint32_t Profile::app_transport_change_timer_addition() const {
  return app_tranport_change_timer_addition_;
}

const uint16_t Profile::max_number_of_ios_device() const {
  return max_number_of_ios_device_;
}

const uint16_t Profile::remove_bundle_id_attempts() const {
  return remove_bundle_id_attempts_;
}

const uint16_t Profile::wait_time_between_apps() const {
  return wait_time_between_apps_;
}

const bool Profile::ErrorOccured() const {
  return error_occured_;
}

const std::string Profile::ErrorDescription() const {
  return error_description_;
}

bool Profile::IsFileNamePortable(const std::string& file_name) const {
  if (file_name.find_first_not_of(kAllowedSymbols) != std::string::npos) {
    return false;
  }
  return true;
}

void Profile::UpdateValues() {
  LOG4CXX_AUTO_TRACE(logger_);

  // SDL version
  ReadStringValue(
      &sdl_version_, default_values::kDefaultSDLVersion, kMainSection, kSDLVersionKey);

  LOG_UPDATED_VALUE(sdl_version_, kSDLVersionKey, kMainSection);

  // Launch HMI parameter
  std::string launch_value;
  if (ReadValue(&launch_value, kHmiSection, kLaunchHMIKey) &&
      0 == strcmp("true", launch_value.c_str())) {
    launch_hmi_ = true;
  } else {
    launch_hmi_ = false;
  }

  LOG_UPDATED_BOOL_VALUE(launch_hmi_, kLaunchHMIKey, kHmiSection);

#ifdef WEB_HMI
  // Link to web HMI parameter
  ReadStringValue(
      &link_to_web_hmi_, default_values::kDefaultLinkToWebHMI, kHmiSection, kLinkToWebHMI);
  LOG_UPDATED_BOOL_VALUE(link_to_web_hmi_, kLinkToWebHMI, kHmiSection);
#endif  // WEB_HMI

#ifdef ENABLE_SECURITY

  force_protected_service_ =
      ReadIntContainer(kSecuritySection, kForceProtectedService, NULL);

  force_unprotected_service_ =
      ReadIntContainer(kSecuritySection, kForceUnprotectedService, NULL);

  ReadStringValue(&security_manager_protocol_name_,
                  default_values::kDefaultSecurityProtocol,
                  kSecuritySection,
                  kSecurityProtocolKey);

  ReadStringValue(
      &cert_path_, "", kSecuritySection, kSecurityCertificatePathKey);
  ReadStringValue(
      &ca_cert_path_, "", kSecuritySection, kSecurityCACertificatePathKey);

  ReadStringValue(
      &ssl_mode_, default_values::kDefaultSSLMode, kSecuritySection, kSecuritySSLModeKey);

  ReadStringValue(&key_path_, "", kSecuritySection, kSecurityKeyPathKey);

  ReadStringValue(
      &ciphers_list_, SSL_TXT_ALL, kSecuritySection, kSecurityCipherListKey);

  ReadBoolValue(&verify_peer_,
                default_values::kDefaultVerifyPeer,
                kSecuritySection,
                kSecurityVerifyPeerKey);

  ReadUIntValue(&update_before_hours_,
                default_values::kDefaultBeforeUpdateHours,
                kSecuritySection,
                kBeforeUpdateHours);

#endif  // ENABLE_SECURITY

  // Logs enabled
  ReadBoolValue(&logs_enabled_, false, kMainSection, kLogsEnabledKey);

  LOG_UPDATED_BOOL_VALUE(logs_enabled_, kLogsEnabledKey, kMainSection);

  // Application config folder
  ReadStringValue(&app_config_folder_,
                  file_system::CurrentWorkingDirectory().c_str(),
                  kMainSection,
                  kAppConfigFolderKey);

  if (IsRelativePath(app_config_folder_)) {
    MakeAbsolutePath(app_config_folder_);
  }

  LOG_UPDATED_VALUE(app_config_folder_, kAppConfigFolderKey, kMainSection);

  // Application storage folder
  ReadStringValue(&app_storage_folder_,
                  file_system::CurrentWorkingDirectory().c_str(),
                  kMainSection,
                  kAppStorageFolderKey);

  if (IsRelativePath(app_storage_folder_)) {
    MakeAbsolutePath(app_storage_folder_);
  }

  LOG_UPDATED_VALUE(app_storage_folder_, kAppStorageFolderKey, kMainSection);

  // Application resourse folder
  ReadStringValue(&app_resource_folder_,
                  file_system::CurrentWorkingDirectory().c_str(),
                  kMainSection,
                  kAppResourseFolderKey);

  if (IsRelativePath(app_resource_folder_)) {
    MakeAbsolutePath(app_resource_folder_);
  }

  LOG_UPDATED_VALUE(app_resource_folder_, kAppResourseFolderKey, kMainSection);

  // Application icon folder
  ReadStringValue(&app_icons_folder_,
                  file_system::CurrentWorkingDirectory().c_str(),
                  kSDL4Section,
                  kAppIconsFolderKey);

  if (IsRelativePath(app_icons_folder_)) {
    MakeAbsolutePath(app_icons_folder_);
  }

  LOG_UPDATED_VALUE(app_icons_folder_, kAppIconsFolderKey, kSDL4Section);

  // Application icon folder maximum size
  ReadUIntValue(&app_icons_folder_max_size_,
                default_values::kDefaultAppIconsFolderMaxSize,
                kSDL4Section,
                kAppIconsFolderMaxSizeKey);

  if (app_icons_folder_max_size_ < default_values::kDefaultAppIconsFolderMaxSize) {
    app_icons_folder_max_size_ = default_values::kDefaultAppIconsFolderMaxSize;
  }

  LOG_UPDATED_VALUE(
      app_icons_folder_max_size_, kAppIconsFolderMaxSizeKey, kSDL4Section);

  // Application icon folder maximum size
  ReadUIntValue(&app_icons_amount_to_remove_,
                default_values::kDefaultAppIconsAmountToRemove,
                kSDL4Section,
                kAppIconsAmountToRemoveKey);

  LOG_UPDATED_VALUE(
      app_icons_amount_to_remove_, kAppIconsAmountToRemoveKey, kSDL4Section);

  ReadUIntValue(&maximum_control_payload_size_,
                default_values::kDefaultMaximumControlPayloadSize,
                kSDL5Section,
                kMaximumControlPayloadSizeKey);

  LOG_UPDATED_VALUE(maximum_control_payload_size_,
                    kMaximumControlPayloadSizeKey,
                    kSDL5Section);

  ReadUIntValue(&maximum_rpc_payload_size_,
                default_values::kDefaultMaximumRpcPayloadSize,
                kSDL5Section,
                kMaximumRpcPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_rpc_payload_size_, kMaximumRpcPayloadSizeKey, kSDL5Section);

  ReadUIntValue(&maximum_audio_payload_size_,
                default_values::kDefaultMaximumAudioPayloadSize,
                kSDL5Section,
                kMaximumAudioPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_audio_payload_size_, kMaximumAudioPayloadSizeKey, kSDL5Section);

  ReadUIntValue(&maximum_video_payload_size_,
                default_values::kDefaultMaximumVideoPayloadSize,
                kSDL5Section,
                kMaximumVideoPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_video_payload_size_, kMaximumVideoPayloadSizeKey, kSDL5Section);

  // Application info file name
  ReadStringValue(&app_info_storage_,
                  default_values::kDefaultAppInfoFileName,
                  kAppInfoSection,
                  kAppInfoStorageKey);

  LOG_UPDATED_VALUE(app_info_storage_, kAppInfoStorageKey, kAppInfoSection);

  // Server address
  ReadStringValue(
      &server_address_, default_values::kDefaultServerAddress, kHmiSection, kServerAddressKey);

  LOG_UPDATED_VALUE(server_address_, kServerAddressKey, kHmiSection);

  // HMI capabilities
  ReadStringValue(&hmi_capabilities_file_name_,
                  default_values::kDefaultHmiCapabilitiesFileName,
                  kMainSection,
                  kHmiCapabilitiesKey);

  hmi_capabilities_file_name_ =
      app_config_folder_ + "/" + hmi_capabilities_file_name_;

  LOG_UPDATED_VALUE(
      hmi_capabilities_file_name_, kHmiCapabilitiesKey, kMainSection);

  // Server port
  ReadUIntValue(&server_port_, default_values::kDefaultServerPort, kHmiSection, kServerPortKey);

  LOG_UPDATED_VALUE(server_port_, kServerPortKey, kHmiSection);

  // Video streaming port
  ReadUIntValue(&video_streaming_port_,
                default_values::kDefaultVideoStreamingPort,
                kHmiSection,
                kVideoStreamingPortKey);

  LOG_UPDATED_VALUE(video_streaming_port_, kVideoStreamingPortKey, kHmiSection);
  // Audio streaming port
  ReadUIntValue(&audio_streaming_port_,
                default_values::kDefaultAudioStreamingPort,
                kHmiSection,
                kAudioStreamingPortKey);

  LOG_UPDATED_VALUE(audio_streaming_port_, kAudioStreamingPortKey, kHmiSection);

  // Streaming timeout
  ReadUIntValue(&stop_streaming_timeout_,
                default_values::kDefaultStopStreamingTimeout,
                kMediaManagerSection,
                kStopStreamingTimeout);

  stop_streaming_timeout_ =
      std::max(default_values::kDefaultStopStreamingTimeout, stop_streaming_timeout_);

  LOG_UPDATED_VALUE(
      stop_streaming_timeout_, kStopStreamingTimeout, kHmiSection);

  // Time testing port
  ReadUIntValue(&time_testing_port_,
                default_values::kDefaultTimeTestingPort,
                kMainSection,
                kTimeTestingPortKey);

  LOG_UPDATED_VALUE(time_testing_port_, kTimeTestingPortKey, kMainSection);

  // Minimum thread stack size
  ReadUIntValue(&min_tread_stack_size_,
                threads::Thread::kMinStackSize,
                kMainSection,
                kThreadStackSizeKey);

  if (min_tread_stack_size_ < threads::Thread::kMinStackSize) {
    min_tread_stack_size_ = threads::Thread::kMinStackSize;
  }

  LOG_UPDATED_VALUE(min_tread_stack_size_, kThreadStackSizeKey, kMainSection);

  // Start stream retry frequency
  ReadUintIntPairValue(&start_stream_retry_amount_,
                       kStartStreamRetryAmount,
                       kMediaManagerSection,
                       kStartStreamRetry);

  // Redecoding parameter
  std::string redecoding_value;
  if (ReadValue(
          &redecoding_value, kMediaManagerSection, kEnableRedecodingKey) &&
      0 == strcmp("true", redecoding_value.c_str())) {
    is_redecoding_enabled_ = true;
  } else {
    is_redecoding_enabled_ = false;
  }

  LOG_UPDATED_BOOL_VALUE(
      is_redecoding_enabled_, kEnableRedecodingKey, kMediaManagerSection);

  // Video consumer type
  ReadStringValue(
      &video_consumer_type_, "", kMediaManagerSection, kVideoStreamConsumerKey);

  LOG_UPDATED_VALUE(
      video_consumer_type_, kVideoStreamConsumerKey, kMediaManagerSection);

  // Audio stream consumer
  ReadStringValue(
      &audio_consumer_type_, "", kMediaManagerSection, kAudioStreamConsumerKey);

  LOG_UPDATED_VALUE(
      audio_consumer_type_, kAudioStreamConsumerKey, kMediaManagerSection);

  // Named video pipe path
  ReadStringValue(&named_video_pipe_path_,
                  "",
                  kMediaManagerSection,
                  kNamedVideoPipePathKey);

  named_video_pipe_path_ = app_storage_folder_ + "/" + named_video_pipe_path_;

  LOG_UPDATED_VALUE(
      named_video_pipe_path_, kNamedVideoPipePathKey, kMediaManagerSection);

  // Named audio pipe path
  ReadStringValue(&named_audio_pipe_path_,
                  "",
                  kMediaManagerSection,
                  kNamedAudioPipePathKey);

  named_audio_pipe_path_ = app_storage_folder_ + "/" + named_audio_pipe_path_;

  LOG_UPDATED_VALUE(
      named_audio_pipe_path_, kNamedAudioPipePathKey, kMediaManagerSection);

  // Video stream file
  ReadStringValue(
      &video_stream_file_, "", kMediaManagerSection, kVideoStreamFileKey);

  video_stream_file_ = app_storage_folder_ + "/" + video_stream_file_;

  LOG_UPDATED_VALUE(
      video_stream_file_, kVideoStreamFileKey, kMediaManagerSection);

  // Audio stream file
  ReadStringValue(
      &audio_stream_file_, "", kMediaManagerSection, kAudioStreamFileKey);

  audio_stream_file_ = app_storage_folder_ + "/" + audio_stream_file_;

  LOG_UPDATED_VALUE(
      audio_stream_file_, kAudioStreamFileKey, kMediaManagerSection);

  ReadUIntValue(&audio_data_stopped_timeout_,
                default_values::kDefaultAudioDataStoppedTimeout,
                kMediaManagerSection,
                kAudioDataStoppedTimeoutKey);

  LOG_UPDATED_VALUE(audio_data_stopped_timeout_,
                    kAudioDataStoppedTimeoutKey,
                    kMediaManagerSection);

  ReadUIntValue(&video_data_stopped_timeout_,
                default_values::kDefaultVideoDataStoppedTimeout,
                kMediaManagerSection,
                kVideoDataStoppedTimeoutKey);

  LOG_UPDATED_VALUE(video_data_stopped_timeout_,
                    kVideoDataStoppedTimeoutKey,
                    kMediaManagerSection);

  // Mixing audio parameter
  std::string mixing_audio_value;
  if (ReadValue(&mixing_audio_value, kMainSection, kMixingAudioSupportedKey) &&
      0 == strcmp("true", mixing_audio_value.c_str())) {
    is_mixing_audio_supported_ = true;
  } else {
    is_mixing_audio_supported_ = false;
  }

  LOG_UPDATED_BOOL_VALUE(
      is_mixing_audio_supported_, kMixingAudioSupportedKey, kMainSection);

  // Maximum command id value
  ReadUIntValue(&max_cmd_id_, default_values::kDefaultMaxCmdId, kMainSection, kMaxCmdIdKey);

  LOG_UPDATED_VALUE(max_cmd_id_, kMaxCmdIdKey, kMainSection);

  // PutFile restrictions
  ReadUIntValue(&put_file_in_none_,
                default_values::kDefaultPutFileRequestInNone,
                kFilesystemRestrictionsSection,
                kPutFileRequestKey);

  LOG_UPDATED_VALUE(
      put_file_in_none_, kPutFileRequestKey, kFilesystemRestrictionsSection);

  // DeleteFileRestrictions
  ReadUIntValue(&delete_file_in_none_,
                default_values::kDefaultDeleteFileRequestInNone,
                kFilesystemRestrictionsSection,
                kDeleteFileRequestKey);

  LOG_UPDATED_VALUE(delete_file_in_none_,
                    kDeleteFileRequestKey,
                    kFilesystemRestrictionsSection);

  // ListFiles restrictions
  ReadUIntValue(&list_files_in_none_,
                default_values::kDefaultListFilesRequestInNone,
                kFilesystemRestrictionsSection,
                kListFilesRequestKey);

  LOG_UPDATED_VALUE(list_files_in_none_,
                    kListFilesRequestKey,
                    kFilesystemRestrictionsSection);

  // ListFiles request size
  ReadUIntValue(&list_files_response_size_,
                default_values::kDefaultListFilesResponseSize,
                kFilesystemRestrictionsSection,
                kListFilesResponseSizeKey);

  LOG_UPDATED_VALUE(list_files_response_size_,
                    kListFilesResponseSizeKey,
                    kFilesystemRestrictionsSection);

  // Default timeout
  ReadUIntValue(
      &default_timeout_, default_values::kDefaultTimeout, kMainSection, default_values::kDefaultTimeoutKey);

  if (default_timeout_ == 0) {
    default_timeout_ = default_values::kDefaultTimeout;
  }

  LOG_UPDATED_VALUE(default_timeout_, default_values::kDefaultTimeoutKey, kMainSection);

  // Application resuming timeout
  ReadUIntValue(&app_resuming_timeout_,
                default_values::kDefaultAppResumingTimeout,
                kResumptionSection,
                kAppResumingTimeoutKey);

  if (app_resuming_timeout_ == 0) {
    app_resuming_timeout_ = default_values::kDefaultAppResumingTimeout;
  }
  // Save resumption info to File System
  LOG_UPDATED_VALUE(app_resuming_timeout_,
                    kAppSavePersistentDataTimeoutKey,
                    kResumptionSection);

  ReadUIntValue(&app_resumption_save_persistent_data_timeout_,
                default_values::kDefaultAppSavePersistentDataTimeout,
                kResumptionSection,
                kAppSavePersistentDataTimeoutKey);
  if (app_resuming_timeout_ == 0) {
    app_resuming_timeout_ = default_values::kDefaultAppSavePersistentDataTimeout;
  }

  LOG_UPDATED_VALUE(
      app_resuming_timeout_, kAppResumingTimeoutKey, kResumptionSection);
  // Open attempt timeout in ms
  ReadUIntValue(&resumption_delay_before_ign_,
                default_values::kDefaultResumptionDelayBeforeIgn,
                kResumptionSection,
                kResumptionDelayBeforeIgnKey);

  LOG_UPDATED_VALUE(resumption_delay_before_ign_,
                    kResumptionDelayBeforeIgnKey,
                    kResumptionSection);

  // Open attempt timeout in ms
  ReadUIntValue(&resumption_delay_after_ign_,
                default_values::kDefaultResumptionDelayAfterIgn,
                kResumptionSection,
                kResumptionDelayAfterIgnKey);

  LOG_UPDATED_VALUE(resumption_delay_after_ign_,
                    kResumptionDelayAfterIgnKey,
                    kResumptionSection);

  // Application directory quota
  ReadUIntValue(
      &app_dir_quota_, default_values::kDefaultDirQuota, kMainSection, kAppDirectoryQuotaKey);

  if (app_dir_quota_ == 0) {
    app_dir_quota_ = default_values::kDefaultDirQuota;
  }

  LOG_UPDATED_VALUE(app_dir_quota_, kAppDirectoryQuotaKey, kMainSection);

  // TTS delimiter
  // Should be gotten before any TTS prompts, since it should be appended back
  ReadStringValue(&tts_delimiter_,
                  default_values::kDefaultTtsDelimiter,
                  kGlobalPropertiesSection,
                  kTTSDelimiterKey);

  LOG_UPDATED_VALUE(tts_delimiter_, kTTSDelimiterKey, kGlobalPropertiesSection);

  // Help prompt
  help_prompt_.clear();
  std::string help_prompt_value;
  if (ReadValue(&help_prompt_value, kGlobalPropertiesSection, kHelpPromptKey)) {
    char* str = NULL;
    str = strtok(const_cast<char*>(help_prompt_value.c_str()), ",");
    while (str != NULL) {
      // Default prompt should have delimiter included for each item
      const std::string prompt_item = std::string(str) + tts_delimiter_;
      help_prompt_.push_back(prompt_item);
      LOG_UPDATED_VALUE(prompt_item, kHelpPromptKey, kGlobalPropertiesSection);
      str = strtok(NULL, ",");
    }
  } else {
    help_prompt_value.clear();
    LOG_UPDATED_VALUE(
        help_prompt_value, kHelpPromptKey, kGlobalPropertiesSection);
  }

  // Timeout prompt
  time_out_promt_.clear();
  std::string timeout_prompt_value;
  if (ReadValue(
          &timeout_prompt_value, kGlobalPropertiesSection, kTimeoutPromptKey)) {
    char* str = NULL;
    str = strtok(const_cast<char*>(timeout_prompt_value.c_str()), ",");
    while (str != NULL) {
      // Default prompt should have delimiter included for each item
      const std::string prompt_item = std::string(str) + tts_delimiter_;
      time_out_promt_.push_back(prompt_item);
      LOG_UPDATED_VALUE(
          prompt_item, kTimeoutPromptKey, kGlobalPropertiesSection);
      str = strtok(NULL, ",");
    }
  } else {
    timeout_prompt_value.clear();
    LOG_UPDATED_VALUE(
        timeout_prompt_value, kTimeoutPromptKey, kGlobalPropertiesSection);
  }

  // Voice recognition help title
  ReadStringValue(&vr_help_title_, "", kGlobalPropertiesSection, kHelpTitleKey);

  LOG_UPDATED_VALUE(vr_help_title_, kHelpTitleKey, kGlobalPropertiesSection);

  // Voice recognition help command
  vr_commands_.clear();
  std::string vr_help_command_value;
  if (ReadValue(&vr_help_command_value, kVrCommandsSection, kHelpCommandKey)) {
    char* str = NULL;
    str = strtok(const_cast<char*>(vr_help_command_value.c_str()), ",");
    while (str != NULL) {
      const std::string vr_item = str;
      vr_commands_.push_back(vr_item);
      LOG_UPDATED_VALUE(vr_item, kHelpCommandKey, kVrCommandsSection);
      str = strtok(NULL, ",");
    }
  } else {
    vr_help_command_value.clear();
    LOG_UPDATED_VALUE(
        vr_help_command_value, kHelpCommandKey, kVrCommandsSection);
  }

  // TTS GlobalProperties timeout
  ReadUIntValue(&tts_global_properties_timeout_,
                default_values::kDefaultTTSGlobalPropertiesTimeout,
                kGlobalPropertiesSection,
                kTTSGlobalPropertiesTimeoutKey);

  LOG_UPDATED_VALUE(tts_global_properties_timeout_,
                    kTTSGlobalPropertiesTimeoutKey,
                    kGlobalPropertiesSection);

  // Application time scale maximum requests
  ReadUIntValue(&app_time_scale_max_requests_,
                default_values::kDefaultAppTimeScaleMaxRequests,
                kMainSection,
                kAppTimeScaleMaxRequestsKey);

  LOG_UPDATED_VALUE(
      app_time_scale_max_requests_, kAppTimeScaleMaxRequestsKey, kMainSection);

  // Application time scale
  ReadUIntValue(&app_requests_time_scale_,
                default_values::kDefaultAppRequestsTimeScale,
                kMainSection,
                kAppRequestsTimeScaleKey);

  LOG_UPDATED_VALUE(
      app_requests_time_scale_, kAppRequestsTimeScaleKey, kMainSection);

  // Application HMI level NONE time scale maximum requests
  ReadUIntValue(&app_hmi_level_none_time_scale_max_requests_,
                default_values::kDefaultAppHmiLevelNoneTimeScaleMaxRequests,
                kMainSection,
                kAppHmiLevelNoneTimeScaleMaxRequestsKey);

  LOG_UPDATED_VALUE(app_hmi_level_none_time_scale_max_requests_,
                    kAppHmiLevelNoneTimeScaleMaxRequestsKey,
                    kMainSection);

  // Application HMI level NONE requests time scale
  ReadUIntValue(&app_hmi_level_none_requests_time_scale_,
                default_values::kDefaultAppHmiLevelNoneRequestsTimeScale,
                kMainSection,
                kAppHmiLevelNoneRequestsTimeScaleKey);

  LOG_UPDATED_VALUE(app_hmi_level_none_requests_time_scale_,
                    kAppHmiLevelNoneRequestsTimeScaleKey,
                    kMainSection);

  // Amount of pending requests
  ReadUIntValue(&pending_requests_amount_,
                default_values::kDefaultPendingRequestsAmount,
                kMainSection,
                kPendingRequestsAmoundKey);

  if (pending_requests_amount_ == 0) {
    pending_requests_amount_ = default_values::kDefaultPendingRequestsAmount;
  }

  LOG_UPDATED_VALUE(
      pending_requests_amount_, kPendingRequestsAmoundKey, kMainSection);

  // Supported diagnostic modes
  supported_diag_modes_.clear();
  std::string supported_diag_modes_value;
  std::string correct_diag_modes;
  if (ReadStringValue(&supported_diag_modes_value,
                      "",
                      kMainSection,
                      kSupportedDiagModesKey)) {
    char* str = NULL;
    str = strtok(const_cast<char*>(supported_diag_modes_value.c_str()), ",");
    while (str != NULL) {
      errno = 0;
      uint32_t user_value = strtol(str, NULL, 16);
      if (user_value && errno != ERANGE) {
        correct_diag_modes += str;
        correct_diag_modes += ",";
        supported_diag_modes_.push_back(user_value);
      }
      str = strtok(NULL, ",");
    }
  }

  LOG_UPDATED_VALUE(correct_diag_modes, kSupportedDiagModesKey, kMainSection);

  // System files path
  ReadStringValue(&system_files_path_,
                  default_values::kDefaultSystemFilesPath,
                  kMainSection,
                  kSystemFilesPathKey);

  LOG_UPDATED_VALUE(system_files_path_, kSystemFilesPathKey, kMainSection);

  // Plugins folder
  ReadStringValue(
      &plugins_folder_, default_values::kDefaultPluginsPath, kMainSection, kPluginsFolderKey);
  LOG_UPDATED_VALUE(plugins_folder_, kPluginsFolderKey, kMainSection);
  // Heartbeat timeout
  ReadUIntValue(&heart_beat_timeout_,
                default_values::kDefaultHeartBeatTimeout,
                kMainSection,
                kHeartBeatTimeoutKey);

  LOG_UPDATED_VALUE(heart_beat_timeout_, kHeartBeatTimeoutKey, kMainSection);

  // Use last state value
  std::string last_state_value;
  if (ReadValue(&last_state_value, kMainSection, kUseLastStateKey) &&
      0 == strcmp("true", last_state_value.c_str())) {
    use_last_state_ = true;
  } else {
    use_last_state_ = false;
  }

  LOG_UPDATED_BOOL_VALUE(use_last_state_, kUseLastStateKey, kMainSection);

  // Transport manager TCP port
  ReadUIntValue(&transport_manager_tcp_adapter_port_,
                kDefautTransportManagerTCPPort,
                kTransportManagerSection,
                kTCPAdapterPortKey);

  LOG_UPDATED_VALUE(transport_manager_tcp_adapter_port_,
                    kTCPAdapterPortKey,
                    kTransportManagerSection);

  // Event MQ
  ReadStringValue(
      &event_mq_name_, default_values::kDefaultEventMQ, kTransportManagerSection, kEventMQKey);

  LOG_UPDATED_VALUE(event_mq_name_, kEventMQKey, kTransportManagerSection);

  // Ack MQ
  ReadStringValue(
      &ack_mq_name_, default_values::kDefaultAckMQ, kTransportManagerSection, kAckMQKey);

  LOG_UPDATED_VALUE(ack_mq_name_, kAckMQKey, kTransportManagerSection);

  // Transport manager disconnect timeout
  ReadUIntValue(&transport_manager_disconnect_timeout_,
                default_values::kDefaultTransportManagerDisconnectTimeout,
                kTransportManagerSection,
                kTransportManagerDisconnectTimeoutKey);

  LOG_UPDATED_VALUE(transport_manager_disconnect_timeout_,
                    kTransportManagerDisconnectTimeoutKey,
                    kTransportManagerSection);

  // Recording file
  ReadStringValue(&recording_file_name_,
                  default_values::kDefaultRecordingFileName,
                  kMediaManagerSection,
                  kRecordingFileNameKey);

  LOG_UPDATED_VALUE(
      recording_file_name_, kRecordingFileNameKey, kMediaManagerSection);

  // Recording file source
  ReadStringValue(&recording_file_source_,
                  default_values::kDefaultRecordingFileSourceName,
                  kMediaManagerSection,
                  kRecordingFileSourceKey);

  LOG_UPDATED_VALUE(
      recording_file_source_, kRecordingFileSourceKey, kMediaManagerSection);

  // Policy preloaded file
  ReadStringValue(&preloaded_pt_file_,
                  default_values::kDefaultPreloadedPTFileName,
                  kPolicySection,
                  kPreloadedPTKey);

  preloaded_pt_file_ = app_config_folder_ + '/' + preloaded_pt_file_;

  LOG_UPDATED_VALUE(preloaded_pt_file_, kPreloadedPTKey, kPolicySection);

  // Policy snapshot file
  ReadStringValue(&policy_snapshot_file_name_,
                  default_values::kDefaultPoliciesSnapshotFileName,
                  kPolicySection,
                  kPathToSnapshotKey);

  LOG_UPDATED_VALUE(
      policy_snapshot_file_name_, kPathToSnapshotKey, kPolicySection);

  if (!IsFileNamePortable(policy_snapshot_file_name_)) {
    error_occured_ = true;
    error_description_ = "PathToSnapshot has forbidden(non-portable) symbols";
  }

  // Attempts number for opening policy DB
  ReadUIntValue(&attempts_to_open_policy_db_,
                default_values::kDefaultAttemptsToOpenPolicyDB,
                kPolicySection,
                kAttemptsToOpenPolicyDBKey);

  LOG_UPDATED_VALUE(
      attempts_to_open_policy_db_, kAttemptsToOpenPolicyDBKey, kPolicySection);

  // Open attempt timeout in ms
  ReadUIntValue(&open_attempt_timeout_ms_,
                default_values::kDefaultOpenAttemptTimeoutMs,
                kPolicySection,
                kOpenAttemptTimeoutMsKey);

  LOG_UPDATED_VALUE(
      open_attempt_timeout_ms_, kOpenAttemptTimeoutMsKey, kPolicySection);

  // Turn Policy Off?
  std::string enable_policy_string;
  if (ReadValue(&enable_policy_string, kPolicySection, kEnablePolicy) &&
      0 == strcmp("true", enable_policy_string.c_str())) {
    enable_policy_ = true;
  } else {
    enable_policy_ = false;
  }

  // Max protocol version
  ReadUIntValue(&max_supported_protocol_version_,
                default_values::kDefaultMaxSupportedProtocolVersion,
                kProtocolHandlerSection,
                kMaxSupportedProtocolVersionKey);

  if (max_supported_protocol_version_ < 1) {
    max_supported_protocol_version_ = 1;
  } else if (max_supported_protocol_version_ >
             default_values::kDefaultMaxSupportedProtocolVersion) {
    max_supported_protocol_version_ = default_values::kDefaultMaxSupportedProtocolVersion;
  }

  LOG_UPDATED_BOOL_VALUE(enable_policy_, kEnablePolicy, kPolicySection);

  ReadUIntValue(&application_list_update_timeout_,
                default_values::kDefaultApplicationListUpdateTimeout,
                kApplicationManagerSection,
                kApplicationListUpdateTimeoutKey);

  LOG_UPDATED_VALUE(application_list_update_timeout_,
                    kApplicationListUpdateTimeoutKey,
                    kMainSection);

  ReadUintIntPairValue(&read_did_frequency_,
                       kReadDIDFrequency,
                       kMainSection,
                       kReadDIDFrequencykey);

  ReadUintIntPairValue(&get_vehicle_data_frequency_,
                       kGetVehicleDataFrequency,
                       kMainSection,
                       kGetVehicleDataFrequencyKey);

  ReadUIntValue(&max_thread_pool_size_,
                default_values::kDefaultMaxThreadPoolSize,
                kApplicationManagerSection,
                default_values::kDefaultThreadPoolSize);
  if (max_thread_pool_size_ > default_values::kDefaultMaxThreadPoolSize) {
    max_thread_pool_size_ = default_values::kDefaultMaxThreadPoolSize;
  }
  LOG_UPDATED_VALUE(max_thread_pool_size_,
                    default_values::kDefaultMaxThreadPoolSize,
                    kApplicationManagerSection);

  ReadStringValue(&iap_legacy_protocol_mask_,
                  default_values::kDefaultLegacyProtocolMask,
                  kIAPSection,
                  kLegacyProtocolMaskKey);

  LOG_UPDATED_VALUE(
      iap_legacy_protocol_mask_, kLegacyProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_hub_protocol_mask_,
                  default_values::kDefaultHubProtocolMask,
                  kIAPSection,
                  kHubProtocolMaskKey);

  LOG_UPDATED_VALUE(iap_hub_protocol_mask_, kHubProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_pool_protocol_mask_,
                  default_values::kDefaultPoolProtocolMask,
                  kIAPSection,
                  kPoolProtocolMaskKey);

  LOG_UPDATED_VALUE(iap_pool_protocol_mask_, kPoolProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_system_config_,
                  default_values::kDefaultIAPSystemConfig,
                  kIAPSection,
                  kIAPSystemConfigKey);

  LOG_UPDATED_VALUE(iap_system_config_, kIAPSystemConfigKey, kIAPSection);

  ReadStringValue(&iap2_system_config_,
                  default_values::kDefaultIAP2SystemConfig,
                  kIAPSection,
                  kIAP2SystemConfigKey);

  LOG_UPDATED_VALUE(iap2_system_config_, kIAP2SystemConfigKey, kIAPSection);

  ReadIntValue(&iap2_hub_connect_attempts_,
               default_values::kDefaultIAP2HubConnectAttempts,
               kIAPSection,
               kIAP2HubConnectAttemptskey);

  LOG_UPDATED_VALUE(
      iap2_hub_connect_attempts_, kIAP2HubConnectAttemptskey, kIAPSection);

  ReadIntValue(&iap_hub_connection_wait_timeout_,
               default_values::kDefaultIAPHubConnectionWaitTimeout,
               kIAPSection,
               kIAPHubConnectionWaitTimeoutKey);

  LOG_UPDATED_VALUE(iap_hub_connection_wait_timeout_,
                    kIAPHubConnectionWaitTimeoutKey,
                    kIAPSection);

  ReadUIntValue(&default_hub_protocol_index_,
                default_values::kDefaultHubProtocolIndex,
                kIAPSection,
                default_values::kDefaultHubProtocolIndexKey);

  LOG_UPDATED_VALUE(
      default_hub_protocol_index_, default_values::kDefaultHubProtocolIndexKey, kIAPSection);

  ReadUIntValue(&hash_string_size_,
                default_values::kDefaultHashStringSize,
                kApplicationManagerSection,
                kHashStringSizeKey);

  LOG_UPDATED_VALUE(
      hash_string_size_, kHashStringSizeKey, kApplicationManagerSection);

  ReadBoolValue(&use_db_for_resumption_,
                false,
                kResumptionSection,
                kUseDBForResumptionKey);

  LOG_UPDATED_BOOL_VALUE(
      use_db_for_resumption_, kUseDBForResumptionKey, kResumptionSection);

  ReadUIntValue(&attempts_to_open_resumption_db_,
                default_values::kDefaultAttemptsToOpenResumptionDB,
                kResumptionSection,
                kAttemptsToOpenResumptionDBKey);

  LOG_UPDATED_VALUE(attempts_to_open_resumption_db_,
                    kAttemptsToOpenResumptionDBKey,
                    kResumptionSection);

  ReadUIntValue(&open_attempt_timeout_ms_resumption_db_,
                default_values::kDefaultOpenAttemptTimeoutMsResumptionDB,
                kResumptionSection,
                kOpenAttemptTimeoutMsResumptionDBKey);

  LOG_UPDATED_VALUE(open_attempt_timeout_ms_resumption_db_,
                    kOpenAttemptTimeoutMsResumptionDBKey,
                    kResumptionSection);

  // Read parameters from App Launch section
  ReadUIntValue(&app_launch_wait_time_,
                default_values::kDefaultAppLaunchWaitTime,
                kAppLaunchSection,
                kAppLaunchWaitTimeKey);

  LOG_UPDATED_VALUE(
      app_launch_wait_time_, kAppLaunchWaitTimeKey, kAppLaunchSection);

  ReadUIntValue(&app_launch_max_retry_attempt_,
                default_values::kDefaultAppLaunchMaxRetryAttempt,
                kAppLaunchSection,
                kAppLaunchMaxRetryAttemptKey);

  LOG_UPDATED_VALUE(app_launch_max_retry_attempt_,
                    kAppLaunchMaxRetryAttemptKey,
                    kAppLaunchSection);

  ReadUIntValue(&app_launch_retry_wait_time_,
                default_values::kDefaultAppLaunchRetryWaitTime,
                kAppLaunchSection,
                kAppLaunchRetryWaitTimeKey);

  LOG_UPDATED_VALUE(app_launch_retry_wait_time_,
                    kAppLaunchRetryWaitTimeKey,
                    kAppLaunchSection);

  ReadUIntValue(&remove_bundle_id_attempts_,
                default_values::kDefaultRemoveBundleIDattempts,
                kAppLaunchSection,
                kRemoveBundleIDattemptsKey);

  LOG_UPDATED_VALUE(remove_bundle_id_attempts_,
                    kRemoveBundleIDattemptsKey,
                    kAppLaunchSection);

  ReadUIntValue(&max_number_of_ios_device_,
                default_values::kDefaultMaxNumberOfiOSDevice,
                kAppLaunchSection,
                kMaxNumberOfiOSDeviceKey);

  LOG_UPDATED_VALUE(
      max_number_of_ios_device_, kMaxNumberOfiOSDeviceKey, kAppLaunchSection);

  ReadUIntValue(&wait_time_between_apps_,
                default_values::kDefaultWaitTimeBetweenApps,
                kAppLaunchSection,
                kWaitTimeBetweenAppsKey);

  LOG_UPDATED_VALUE(
      wait_time_between_apps_, kWaitTimeBetweenAppsKey, kAppLaunchSection);

  ReadBoolValue(&enable_app_launch_ios_,
                default_values::kDefaultEnableAppLaunchIOS,
                kAppLaunchSection,
                kEnableAppLaunchIOSKey);

  LOG_UPDATED_BOOL_VALUE(
      enable_app_launch_ios_, kEnableAppLaunchIOSKey, kAppLaunchSection);

  ReadUIntValue(&app_tranport_change_timer_,
                default_values::kDefaultAppTransportChangeTimer,
                kMainSection,
                kAppTransportChangeTimerKey);

  LOG_UPDATED_VALUE(
      app_tranport_change_timer_, kAppTransportChangeTimerKey, kMainSection);

  ReadUIntValue(&app_tranport_change_timer_addition_,
                default_values::kDefaultAppTransportChangeTimerAddition,
                kMainSection,
                kAppTransportChangeTimerAdditionKey);

  LOG_UPDATED_VALUE(app_tranport_change_timer_addition_,
                    kAppTransportChangeTimerAdditionKey,
                    kMainSection);
}

bool Profile::ReadValue(bool* value,
                        const char* const pSection,
                        const char* const pKey) const {
  DCHECK(value);
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf)) &&
      ('\0' != *buf)) {
    const int32_t tmpVal = atoi(buf);
    if ((0 == strcmp("true", buf)) || (0 != tmpVal)) {
      *value = true;
    } else {
      *value = false;
    }
    ret = true;
  }
  return ret;
}

bool Profile::ReadValue(std::string* value,
                        const char* const pSection,
                        const char* const pKey) const {
  DCHECK(value);
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if ((0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf)) &&
      ('\0' != *buf)) {
    *value = buf;
    ret = true;
  }
  return ret;
}

bool Profile::ReadStringValue(std::string* value,
                              const char* default_value,
                              const char* const pSection,
                              const char* const pKey) const {
  DCHECK(value);
  if (!ReadValue(value, pSection, pKey)) {
    *value = default_value;
    return false;
  }
  return true;
}

bool Profile::ReadIntValue(int32_t* value,
                           const int32_t default_value,
                           const char* const pSection,
                           const char* const pKey) const {
  DCHECK(value);
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  }
  *value = atoi(string_value.c_str());
  return true;
}

bool Profile::ReadUintIntPairValue(
    std::pair<uint32_t, int32_t>* value,
    const std::pair<uint32_t, uint32_t>& default_value,
    const char* const pSection,
    const char* const pKey) const {
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  }
  std::string first_str = string_value.substr(0, string_value.find(","));
  std::string second_str = string_value.substr(
      string_value.find(",") + 1, string_value.size() - first_str.size());
  (*value).first = strtoul(first_str.c_str(), NULL, 10);
  (*value).second = strtoul(second_str.c_str(), NULL, 10);
  return true;
}

bool Profile::ReadBoolValue(bool* value,
                            const bool default_value,
                            const char* const pSection,
                            const char* const pKey) const {
  DCHECK(value);
  bool read_value;
  const bool result = ReadValue(&read_value, pSection, pKey);
  *value = result ? read_value : default_value;
  return result;
}
namespace {
int32_t hex_to_int(const std::string& value) {
  return static_cast<int32_t>(strtol(value.c_str(), NULL, 16));
}
}

std::vector<int> Profile::ReadIntContainer(const char* const pSection,
                                           const char* const pKey,
                                           bool* out_result) const {
  const std::vector<std::string> string_list =
      ReadStringContainer(pSection, pKey, out_result);
  std::vector<int> value_list;
  value_list.resize(string_list.size());
  std::transform(
      string_list.begin(), string_list.end(), value_list.begin(), hex_to_int);
  return value_list;
}

std::vector<std::string> Profile::ReadStringContainer(
    const char* const pSection,
    const char* const pKey,
    bool* out_result) const {
  std::string string;
  const bool result = ReadValue(&string, pSection, pKey);
  if (out_result)
    *out_result = result;
  std::vector<std::string> value_container;
  if (result) {
    std::istringstream iss(string);
    std::string temp_str;
    while (iss) {
      if (!getline(iss, temp_str, ','))
        break;
      value_container.push_back(temp_str);
    }
  }
  return value_container;
}

bool Profile::ReadUIntValue(uint16_t* value,
                            uint16_t default_value,
                            const char* const pSection,
                            const char* const pKey) const {
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  } else {
    uint64_t user_value;
    if (!StringToNumber(string_value, user_value)) {
      *value = default_value;
      return false;
    }
    if (user_value > (std::numeric_limits<uint16_t>::max)()) {
      *value = default_value;
      return false;
    }
    *value = static_cast<uint16_t>(user_value);
    return true;
  }
}

bool Profile::ReadUIntValue(uint32_t* value,
                            uint32_t default_value,
                            const char* const pSection,
                            const char* const pKey) const {
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  } else {
    uint64_t user_value;
    if (!StringToNumber(string_value, user_value)) {
      *value = default_value;
      return false;
    }
    if (user_value > (std::numeric_limits<uint32_t>::max)()) {
      *value = default_value;
      return false;
    }

    *value = static_cast<uint32_t>(user_value);
    return true;
  }
}

bool Profile::ReadUIntValue(uint64_t* value,
                            uint64_t default_value,
                            const char* const pSection,
                            const char* const pKey) const {
  std::string string_value;
  if (!ReadValue(&string_value, pSection, pKey)) {
    *value = default_value;
    return false;
  } else {
    uint64_t user_value;
    if (!StringToNumber(string_value, user_value)) {
      *value = default_value;
      return false;
    }

    *value = user_value;
    return true;
  }
}

bool Profile::StringToNumber(const std::string& input, uint64_t& output) const {
  const char* input_value = input.c_str();
  char* endptr;
  const int8_t base = 10;
  errno = 0;
  uint64_t user_value = strtoull(input_value, &endptr, base);
  bool is_real_zero_value =
      (!user_value && endptr != input_value && *endptr == '\0');
  if (!is_real_zero_value && (!user_value || errno == ERANGE)) {
    return false;
  }
  output = user_value;
  return true;
}

bool Profile::IsRelativePath(const std::string& path) {
  if (path.empty()) {
    LOG4CXX_ERROR(logger_, "Empty path passed.");
    return false;
  }
  return '/' != path[0];
}

void Profile::MakeAbsolutePath(std::string& path) {
  path = file_system::CurrentWorkingDirectory() + "/" + path;
}

}  //  namespace profile
