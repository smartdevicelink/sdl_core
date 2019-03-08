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
#include <numeric>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <algorithm>

#include <string>

#include "config_profile/ini_file.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/file_system.h"

#ifdef ENABLE_SECURITY
#include <openssl/ssl.h>
#endif  // ENABLE_SECURITY

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

const char* kDefaultConfigFileName = "smartDeviceLink.ini";

const char* kMainSection = "MAIN";
#ifdef ENABLE_SECURITY
const char* kSecuritySection = "Security Manager";
const char* kForceProtectedService = "ForceProtectedService";
const char* kForceUnprotectedService = "ForceUnprotectedService";
#endif
const char* kPolicySection = "Policy";
const char* kHmiSection = "HMI";
const char* kAppInfoSection = "AppInfo";
const char* kMediaManagerSection = "MEDIA MANAGER";
const char* kGlobalPropertiesSection = "GLOBAL PROPERTIES";
const char* kVrCommandsSection = "VR COMMANDS";
const char* kTransportManagerSection = "TransportManager";
const char* kCloudAppTransportSection = "CloudAppConnections";
const char* kApplicationManagerSection = "ApplicationManager";
const char* kFilesystemRestrictionsSection = "FILESYSTEM RESTRICTIONS";
const char* kIAPSection = "IAP";
const char* kProtocolHandlerSection = "ProtocolHandler";
const char* kSDL4Section = "SDL4";
const char* kSDL5Section = "SDL5";
const char* kResumptionSection = "Resumption";
const char* kAppLaunchSection = "AppLaunch";
const char* kMultipleTransportsSection = "MultipleTransports";
const char* kServicesMapSection = "ServicesMap";
const char* kTransportRequiredForResumptionSection =
    "TransportRequiredForResumption";
const char* kLowBandwidthTransportResumptionLevelSection =
    "LowBandwidthTransportResumptionLevel";

const char* kSDLVersionKey = "SDLVersion";
const char* kHmiCapabilitiesKey = "HMICapabilities";
const char* kPathToSnapshotKey = "PathToSnapshot";
const char* kPreloadedPTKey = "PreloadedPT";
const char* kAttemptsToOpenPolicyDBKey = "AttemptsToOpenPolicyDB";
const char* kOpenAttemptTimeoutMsKey = "OpenAttemptTimeoutMs";
const char* kServerAddressKey = "ServerAddress";
const char* kAppInfoStorageKey = "AppInfoStorage";
const char* kAppStorageFolderKey = "AppStorageFolder";
const char* kAppResourseFolderKey = "AppResourceFolder";
const char* kLogsEnabledKey = "LogsEnabled";
const char* kAppConfigFolderKey = "AppConfigFolder";
const char* kAppIconsFolderKey = "AppIconsFolder";
const char* kAppIconsFolderMaxSizeKey = "AppIconsFolderMaxSize";
const char* kAppIconsAmountToRemoveKey = "AppIconsAmountToRemove";
const char* kMaximumControlPayloadSizeKey = "MaximumControlPayloadSize";
const char* kMaximumRpcPayloadSizeKey = "MaximumRpcPayloadSize";
const char* kMaximumAudioPayloadSizeKey = "MaximumAudioPayloadSize";
const char* kMaximumVideoPayloadSizeKey = "MaximumVideoPayloadSize";
const char* kLaunchHMIKey = "LaunchHMI";
const char* kDefaultSDLVersion = "";
#ifdef WEB_HMI
const char* kLinkToWebHMI = "LinkToWebHMI";
#endif  // WEB_HMI
const char* kStartStreamRetry = "StartStreamRetry";
const char* kEnableRedecodingKey = "EnableRedecoding";
const char* kVideoStreamConsumerKey = "VideoStreamConsumer";
const char* kAudioStreamConsumerKey = "AudioStreamConsumer";
const char* kNamedVideoPipePathKey = "NamedVideoPipePath";
const char* kNamedAudioPipePathKey = "NamedAudioPipePath";
const char* kVideoStreamFileKey = "VideoStreamFile";
const char* kAudioStreamFileKey = "AudioStreamFile";

#ifdef ENABLE_SECURITY
const char* kSecurityProtocolKey = "Protocol";
const char* kSecurityCertificatePathKey = "CertificatePath";
const char* kSecurityCACertificatePathKey = "CACertificatePath";
const char* kSecuritySSLModeKey = "SSLMode";
const char* kSecurityKeyPathKey = "KeyPath";
const char* kSecurityCipherListKey = "CipherList";
const char* kSecurityVerifyPeerKey = "VerifyPeer";
const char* kBeforeUpdateHours = "UpdateBeforeHours";
#endif

const char* kAudioDataStoppedTimeoutKey = "AudioDataStoppedTimeout";
const char* kVideoDataStoppedTimeoutKey = "VideoDataStoppedTimeout";
const char* kMixingAudioSupportedKey = "MixingAudioSupported";
const char* kHelpPromptKey = "HelpPromt";
const char* kTimeoutPromptKey = "TimeOutPromt";
const char* kHelpTitleKey = "HelpTitle";
const char* kHelpCommandKey = "HelpCommand";
const char* kSystemFilesPathKey = "SystemFilesPath";
const char* kPluginsFolderKey = "PluginFolder";
const char* kHeartBeatTimeoutKey = "HeartBeatTimeout";
const char* kMaxSupportedProtocolVersionKey = "MaxSupportedProtocolVersion";
const char* kUseLastStateKey = "UseLastState";
const char* kTCPAdapterPortKey = "TCPAdapterPort";
const char* kTCPAdapterNetworkInterfaceKey = "TCPAdapterNetworkInterface";
const char* kCloudAppRetryTimeoutKey = "CloudAppRetryTimeout";
const char* kCloudAppMaxRetryAttemptsKey = "CloudAppMaxRetryAttempts";
const char* kServerPortKey = "ServerPort";
const char* kVideoStreamingPortKey = "VideoStreamingPort";
const char* kAudioStreamingPortKey = "AudioStreamingPort";
const char* kStopStreamingTimeout = "StopStreamingTimeout";
const char* kTimeTestingPortKey = "TimeTestingPort";
const char* kThreadStackSizeKey = "ThreadStackSize";
const char* kMaxCmdIdKey = "MaxCmdID";
const char* kPutFileRequestKey = "PutFileRequest";
const char* kDeleteFileRequestKey = "DeleteFileRequest";
const char* kListFilesRequestKey = "ListFilesRequest";
const char* kListFilesResponseSizeKey = "ListFilesResponseSize";
const char* kDefaultTimeoutKey = "DefaultTimeout";
const char* kAppResumingTimeoutKey = "ApplicationResumingTimeout";
const char* kAppSavePersistentDataTimeoutKey = "AppSavePersistentDataTimeout";
const char* kResumptionDelayBeforeIgnKey = "ResumptionDelayBeforeIgn";
const char* kResumptionDelayAfterIgnKey = "ResumptionDelayAfterIgn";
const char* kAppDirectoryQuotaKey = "AppDirectoryQuota";
const char* kAppTimeScaleMaxRequestsKey = "AppTimeScaleMaxRequests";
const char* kAppRequestsTimeScaleKey = "AppRequestsTimeScale";
const char* kAppHmiLevelNoneTimeScaleMaxRequestsKey =
    "AppHMILevelNoneTimeScaleMaxRequests";
const char* kAppHmiLevelNoneRequestsTimeScaleKey =
    "AppHMILevelNoneRequestsTimeScale";
const char* kPendingRequestsAmoundKey = "PendingRequestsAmount";
const char* kSupportedDiagModesKey = "SupportedDiagModes";
const char* kTransportManagerDisconnectTimeoutKey = "DisconnectTimeout";
const char* kTTSDelimiterKey = "TTSDelimiter";
const char* kRecordingFileNameKey = "RecordingFileName";
const char* kRecordingFileSourceKey = "RecordingFileSource";
const char* kEnablePolicy = "EnablePolicy";
const char* kUseFullAppID = "UseFullAppID";
const char* kEventMQKey = "EventMQ";
const char* kAckMQKey = "AckMQ";
const char* kApplicationListUpdateTimeoutKey = "ApplicationListUpdateTimeout";
const char* kReadDIDFrequencykey = "ReadDIDRequest";
const char* kGetVehicleDataFrequencyKey = "GetVehicleDataRequest";
const char* kGetInteriorVehicleDataFrequencyKey =
    "GetInteriorVehicleDataRequest";
const char* kLegacyProtocolMaskKey = "LegacyProtocol";
const char* kHubProtocolMaskKey = "HubProtocol";
const char* kPoolProtocolMaskKey = "PoolProtocol";
const char* kIAPSystemConfigKey = "IAPSystemConfig";
const char* kIAP2SystemConfigKey = "IAP2SystemConfig";
const char* kIAP2HubConnectAttemptskey = "IAP2HubConnectAttempts";
const char* kIAPHubConnectionWaitTimeoutKey = "ConnectionWaitTimeout";
const char* kDefaultHubProtocolIndexKey = "DefaultHubProtocolIndex";
const char* kTTSGlobalPropertiesTimeoutKey = "TTSGlobalPropertiesTimeout";
const char* kMaximumPayloadSizeKey = "MaximumPayloadSize";
const char* kFrequencyCount = "FrequencyCount";
const char* kFrequencyTime = "FrequencyTime";
const char* kMalformedMessageFiltering = "MalformedMessageFiltering";
const char* kMalformedFrequencyCount = "MalformedFrequencyCount";
const char* kMalformedFrequencyTime = "MalformedFrequencyTime";
const char* kExpectedConsecutiveFramesTimeout =
    "ExpectedConsecutiveFramesTimeout";
const char* kHashStringSizeKey = "HashStringSize";
const char* kUseDBForResumptionKey = "UseDBForResumption";
const char* kAttemptsToOpenResumptionDBKey = "AttemptsToOpenResumptionDB";
const char* kOpenAttemptTimeoutMsResumptionDBKey =
    "OpenAttemptTimeoutMsResumptionDB";

const char* kAppLaunchWaitTimeKey = "AppLaunchWaitTime";
const char* kAppLaunchMaxRetryAttemptKey = "AppLaunchMaxRetryAttempt";
const char* kAppLaunchRetryWaitTimeKey = "AppLaunchRetryWaitTime";
const char* kRemoveBundleIDattemptsKey = "RemoveBundleIDattempts";
const char* kMaxNumberOfiOSDeviceKey = "MaxNumberOfiOSDevice";
const char* kWaitTimeBetweenAppsKey = "WaitTimeBetweenApps";
const char* kEnableAppLaunchIOSKey = "EnableAppLaunchIOS";
const char* kAppTransportChangeTimerKey = "AppTransportChangeTimer";
const char* kAppTransportChangeTimerAdditionKey =
    "AppTransportChangeTimerAddition";
const char* kLowVoltageSignalOffsetKey = "LowVoltageSignal";
const char* kWakeUpSignalOffsetKey = "WakeUpSignal";
const char* kIgnitionOffSignalOffsetKey = "IgnitionOffSignal";
const char* kMultipleTransportsEnabledKey = "MultipleTransportsEnabled";
const char* kSecondaryTransportForBluetoothKey =
    "SecondaryTransportForBluetooth";
const char* kSecondaryTransportForUSBKey = "SecondaryTransportForUSB";
const char* kSecondaryTransportForWiFiKey = "SecondaryTransportForWiFi";
const char* kAudioServiceTransportsKey = "AudioServiceTransports";
const char* kVideoServiceTransportsKey = "VideoServiceTransports";

const char* kDefaultTransportRequiredForResumptionKey =
    "DefaultTransportRequiredForResumption";
const char* kAppHMITypeDefault = "DEFAULT";
const char* kCommunicationTransportRequiredForResumptionKey =
    "CommunicationTransportRequiredForResumption";
const char* kAppHMITypeCommunication = "COMMUNICATION";
const char* kMediaTransportRequiredForResumptionKey =
    "MediaTransportRequiredForResumption";
const char* kAppHMITypeMedia = "MEDIA";
const char* kMessagingTransportRequiredForResumptionKey =
    "MessagingTransportRequiredForResumption";
const char* kAppHMITypeMessaging = "MESSAGING";
const char* kNavigationTransportRequiredForResumptionKey =
    "NavigationTransportRequiredForResumption";
const char* kAppHMITypeNavigation = "NAVIGATION";
const char* kInformationTransportRequiredForResumptionKey =
    "InformationTransportRequiredForResumption";
const char* kAppHMITypeInformation = "INFORMATION";
const char* kSocialTransportRequiredForResumptionKey =
    "SocialTransportRequiredForResumption";
const char* kAppHMITypeSocial = "SOCIAL";
const char* kBackgroundProcessTransportRequiredForResumptionKey =
    "BackgroundProcessTransportRequiredForResumption";
const char* kAppHMITypeBackgroundProcess = "BACKGROUND_PROCESS";
const char* kTestingTransportRequiredForResumptionKey =
    "TestingTransportRequiredForResumption";
const char* kAppHMITypeTesting = "TESTING";
const char* kSystemTransportRequiredForResumptionKey =
    "SystemTransportRequiredForResumption";
const char* kAppHMITypeSystem = "SYSTEM";
const char* kProjectionTransportRequiredForResumptionKey =
    "ProjectionTransportRequiredForResumption";
const char* kAppHMITypeProjection = "PROJECTION";
const char* kRemoteControlTransportRequiredForResumptionKey =
    "RemoteControlTransportRequiredForResumption";
const char* kAppHMITypeRemoteControl = "REMOTE_CONTROL";
const char* kEmptyAppTransportRequiredForResumptionKey =
    "EmptyAppTransportRequiredForResumption";
const char* kAppHMITypeEmptyApp = "EMPTY_APP";
const char* kNavigationLowBandwidthResumptionLevelKey =
    "NavigationLowBandwidthResumptionLevel";
const char* kProjectionLowBandwidthResumptionLevelKey =
    "ProjectionLowBandwidthResumptionLevel";
const char* kMediaLowBandwidthResumptionLevelKey =
    "MediaLowBandwidthResumptionLevel";

#ifdef WEB_HMI
const char* kDefaultLinkToWebHMI = "HMI/index.html";
#endif  // WEB_HMI
const char* kDefaultPoliciesSnapshotFileName = "sdl_snapshot.json";
const char* kDefaultHmiCapabilitiesFileName = "hmi_capabilities.json";
const char* kDefaultPreloadedPTFileName = "sdl_preloaded_pt.json";
const char* kDefaultServerAddress = "127.0.0.1";
const char* kDefaultAppInfoFileName = "app_info.dat";
const char* kDefaultSystemFilesPath = "/tmp/fs/mp/images/ivsu_cache";
const char* kDefaultPluginsPath = "plugins";
const char* kDefaultTtsDelimiter = ",";
const uint32_t kDefaultAudioDataStoppedTimeout = 1000;
const uint32_t kDefaultVideoDataStoppedTimeout = 1000;
const char* kDefaultEventMQ = "/dev/mqueue/ToSDLCoreUSBAdapter";
const char* kDefaultAckMQ = "/dev/mqueue/FromSDLCoreUSBAdapter";
const char* kDefaultRecordingFileSourceName = "audio.8bit.wav";
const char* kDefaultRecordingFileName = "record.wav";
const char* kDefaultThreadPoolSize = "ThreadPoolSize";
const char* kDefaultLegacyProtocolMask = "com.ford.sync.prot";
const char* kDefaultHubProtocolMask = "com.smartdevicelink.prot";
const char* kDefaultPoolProtocolMask = "com.smartdevicelink.prot";
const char* kDefaultIAPSystemConfig = "/fs/mp/etc/mm/ipod.cfg";
const char* kDefaultIAP2SystemConfig = "/fs/mp/etc/mm/iap2.cfg";
const char* kDefaultTransportManagerTCPAdapterNetworkInterface = "";

#ifdef ENABLE_SECURITY
const char* kDefaultSecurityProtocol = "TLSv1.2";
const char* kDefaultSSLMode = "CLIENT";
const bool kDefaultVerifyPeer = false;
const uint32_t kDefaultBeforeUpdateHours = 24;
#endif  // ENABLE_SECURITY

const uint32_t kDefaultHubProtocolIndex = 0;
const uint32_t kDefaultHeartBeatTimeout = 0;
const uint16_t kDefaultMaxSupportedProtocolVersion = 5;
const uint16_t kDefautTransportManagerTCPPort = 12345;
const uint16_t kDefaultCloudAppRetryTimeout = 1000;
const uint16_t kDefaultCloudAppMaxRetryAttempts = 5;
const uint16_t kDefaultServerPort = 8087;
const uint16_t kDefaultVideoStreamingPort = 5050;
const uint16_t kDefaultAudioStreamingPort = 5080;
const uint32_t kDefaultStopStreamingTimeout = 1;
const uint16_t kDefaultTimeTestingPort = 5090;
const uint32_t kDefaultMaxCmdId = 2000000000;
const uint32_t kDefaultPutFileRequestInNone = 5;
const uint32_t kDefaultDeleteFileRequestInNone = 5;
const uint32_t kDefaultListFilesRequestInNone = 5;
const uint32_t kDefaultTimeout = 10000;
const uint32_t kDefaultAppResumingTimeout = 3000;
const uint32_t kDefaultAppSavePersistentDataTimeout = 10000;
const uint32_t kDefaultResumptionDelayBeforeIgn = 30;
const uint32_t kDefaultResumptionDelayAfterIgn = 30;
const uint32_t kDefaultHashStringSize = 32;
const uint32_t kDefaultListFilesResponseSize = 1000;

const uint32_t kDefaultDirQuota = 104857600;
const uint32_t kDefaultAppTimeScaleMaxRequests = 0;
const uint32_t kDefaultAppRequestsTimeScale = 0;
const uint32_t kDefaultAppHmiLevelNoneTimeScaleMaxRequests = 100;
const uint32_t kDefaultAppHmiLevelNoneRequestsTimeScale = 10;
const uint32_t kDefaultPendingRequestsAmount = 0;
const uint32_t kDefaultTransportManagerDisconnectTimeout = 0;
const uint32_t kDefaultApplicationListUpdateTimeout = 1;
const std::pair<uint32_t, uint32_t> kReadDIDFrequency = {5, 1};
const std::pair<uint32_t, uint32_t> kGetVehicleDataFrequency = {5, 1};
const std::pair<uint32_t, uint32_t> kGetInteriorVehicleDataFrequency = {20, 1};
const std::pair<uint32_t, uint32_t> kStartStreamRetryAmount = {3, 1};
const uint32_t kDefaultMaxThreadPoolSize = 2;
const int kDefaultIAP2HubConnectAttempts = 0;
const int kDefaultIAPHubConnectionWaitTimeout = 10000;
const int kDefaultIAPArmEventTimeout = 500;
const uint16_t kDefaultTTSGlobalPropertiesTimeout = 20;
// TCP MTU - header size = 1500 - 12
const size_t kDefaultMaximumPayloadSize = 1500 - 12;
const size_t kDefaultFrequencyCount = 1000;
const size_t kDefaultFrequencyTime = 1000;
const bool kDefaulMalformedMessageFiltering = true;
const size_t kDefaultMalformedFrequencyCount = 10;
const size_t kDefaultMalformedFrequencyTime = 1000;
const uint32_t kDefaultExpectedConsecutiveFramesTimeout = 10000;
const uint16_t kDefaultAttemptsToOpenPolicyDB = 5;
const uint16_t kDefaultOpenAttemptTimeoutMs = 500;
const uint32_t kDefaultAppIconsFolderMaxSize = 104857600;
const uint32_t kDefaultAppIconsAmountToRemove = 1;
const uint16_t kDefaultAttemptsToOpenResumptionDB = 5;
const size_t kDefaultMaximumControlPayloadSize = 0;
const size_t kDefaultMaximumRpcPayloadSize = 0;
const size_t kDefaultMaximumAudioPayloadSize = 0;
const size_t kDefaultMaximumVideoPayloadSize = 0;
const uint16_t kDefaultOpenAttemptTimeoutMsResumptionDB = 500;
const uint16_t kDefaultAppLaunchWaitTime = 5000;
const uint16_t kDefaultAppLaunchMaxRetryAttempt = 3;
const uint16_t kDefaultAppLaunchRetryWaitTime = 15000;
const uint16_t kDefaultRemoveBundleIDattempts = 3;
const uint16_t kDefaultMaxNumberOfiOSDevice = 10;
const uint16_t kDefaultWaitTimeBetweenApps = 4000;
const bool kDefaultEnableAppLaunchIOS = true;
const uint32_t kDefaultAppTransportChangeTimer = 500u;
const uint32_t kDefaultAppTransportChangeTimerAddition = 0u;
const int32_t kDefaultLowVoltageSignalOffset = 1;
const int32_t kDefaultWakeUpSignalOffset = 2;
const int32_t kDefaultIgnitionOffSignalOffset = 3;
const std::string kAllowedSymbols =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.-";
const bool kDefaultMultipleTransportsEnabled = false;
const char* kDefaultLowBandwidthResumptionLevel = "NONE";
}  // namespace

namespace profile {

CREATE_LOGGERPTR_GLOBAL(logger_, "Profile")

Profile::Profile()
    : sdl_version_(kDefaultSDLVersion)
    , launch_hmi_(true)
    ,
#ifdef WEB_HMI
    link_to_web_hmi_(kDefaultLinkToWebHMI)
    ,
#endif  // WEB_HMI
    app_config_folder_()
    , app_storage_folder_()
    , app_resource_folder_()
    , app_icons_folder_()
    , app_icons_folder_max_size_(kDefaultAppIconsFolderMaxSize)
    , app_icons_amount_to_remove_(kDefaultAppIconsAmountToRemove)
    , maximum_control_payload_size_(kDefaultMaximumControlPayloadSize)
    , maximum_rpc_payload_size_(kDefaultMaximumRpcPayloadSize)
    , maximum_audio_payload_size_(kDefaultMaximumAudioPayloadSize)
    , maximum_video_payload_size_(kDefaultMaximumVideoPayloadSize)
    , config_file_name_(kDefaultConfigFileName)
    , server_address_(kDefaultServerAddress)
    , server_port_(kDefaultServerPort)
    , video_streaming_port_(kDefaultVideoStreamingPort)
    , audio_streaming_port_(kDefaultAudioStreamingPort)
    , stop_streaming_timeout_(kDefaultStopStreamingTimeout)
    , time_testing_port_(kDefaultTimeTestingPort)
    , hmi_capabilities_file_name_(kDefaultHmiCapabilitiesFileName)
    , help_prompt_()
    , time_out_promt_()
    , min_tread_stack_size_(threads::Thread::kMinStackSize)
    , is_mixing_audio_supported_(false)
    , is_redecoding_enabled_(false)
    , max_cmd_id_(kDefaultMaxCmdId)
    , default_timeout_(kDefaultTimeout)
    , app_resuming_timeout_(kDefaultAppResumingTimeout)
    , app_resumption_save_persistent_data_timeout_(
          kDefaultAppSavePersistentDataTimeout)
    , app_dir_quota_(kDefaultDirQuota)
    , app_hmi_level_none_time_scale_max_requests_(
          kDefaultAppHmiLevelNoneTimeScaleMaxRequests)
    , app_hmi_level_none_requests_time_scale_(
          kDefaultAppHmiLevelNoneRequestsTimeScale)
    , app_time_scale_max_requests_(kDefaultAppTimeScaleMaxRequests)
    , app_requests_time_scale_(kDefaultAppRequestsTimeScale)
    , pending_requests_amount_(kDefaultPendingRequestsAmount)
    , put_file_in_none_(kDefaultPutFileRequestInNone)
    , delete_file_in_none_(kDefaultDeleteFileRequestInNone)
    , list_files_in_none_(kDefaultListFilesRequestInNone)
    , list_files_response_size_(kDefaultListFilesResponseSize)
    , app_info_storage_(kDefaultAppInfoFileName)
    , heart_beat_timeout_(kDefaultHeartBeatTimeout)
    , max_supported_protocol_version_(kDefaultMaxSupportedProtocolVersion)
    , policy_snapshot_file_name_(kDefaultPoliciesSnapshotFileName)
    , enable_policy_(false)
    , use_full_app_id_(true)
    , transport_manager_disconnect_timeout_(
          kDefaultTransportManagerDisconnectTimeout)
    , use_last_state_(false)
    , supported_diag_modes_()
    , system_files_path_(kDefaultSystemFilesPath)
    , transport_manager_tcp_adapter_port_(kDefautTransportManagerTCPPort)
    , cloud_app_retry_timeout_(kDefaultCloudAppRetryTimeout)
    , cloud_app_max_retry_attempts_(kDefaultCloudAppMaxRetryAttempts)
    , tts_delimiter_(kDefaultTtsDelimiter)
    , audio_data_stopped_timeout_(kDefaultAudioDataStoppedTimeout)
    , video_data_stopped_timeout_(kDefaultVideoDataStoppedTimeout)
    , event_mq_name_(kDefaultEventMQ)
    , ack_mq_name_(kDefaultAckMQ)
    , recording_file_source_(kDefaultRecordingFileSourceName)
    , recording_file_name_(kDefaultRecordingFileName)
    , application_list_update_timeout_(kDefaultApplicationListUpdateTimeout)
    , max_thread_pool_size_(kDefaultMaxThreadPoolSize)
    , default_hub_protocol_index_(kDefaultHubProtocolIndex)
    , iap_legacy_protocol_mask_(kDefaultLegacyProtocolMask)
    , iap_hub_protocol_mask_(kDefaultHubProtocolMask)
    , iap_pool_protocol_mask_(kDefaultPoolProtocolMask)
    , iap_system_config_(kDefaultIAPSystemConfig)
    , iap2_system_config_(kDefaultIAP2SystemConfig)
    , iap2_hub_connect_attempts_(kDefaultIAP2HubConnectAttempts)
    , iap_hub_connection_wait_timeout_(kDefaultIAPHubConnectionWaitTimeout)
    , tts_global_properties_timeout_(kDefaultTTSGlobalPropertiesTimeout)
    , attempts_to_open_policy_db_(kDefaultAttemptsToOpenPolicyDB)
    , open_attempt_timeout_ms_(kDefaultAttemptsToOpenPolicyDB)
    , resumption_delay_before_ign_(kDefaultResumptionDelayBeforeIgn)
    , resumption_delay_after_ign_(kDefaultResumptionDelayAfterIgn)
    , hash_string_size_(kDefaultHashStringSize)
    , use_db_for_resumption_(false)
    , attempts_to_open_resumption_db_(kDefaultAttemptsToOpenResumptionDB)
    , open_attempt_timeout_ms_resumption_db_(
          kDefaultOpenAttemptTimeoutMsResumptionDB)
    , navigation_lowbandwidth_resumption_level_(
          kDefaultLowBandwidthResumptionLevel)
    , projection_lowbandwidth_resumption_level_(
          kDefaultLowBandwidthResumptionLevel)
    , media_lowbandwidth_resumption_level_(kDefaultLowBandwidthResumptionLevel)
    , app_launch_wait_time_(kDefaultAppLaunchWaitTime)
    , app_launch_max_retry_attempt_(kDefaultAppLaunchMaxRetryAttempt)
    , app_launch_retry_wait_time_(kDefaultAppLaunchRetryWaitTime)
    , remove_bundle_id_attempts_(kDefaultRemoveBundleIDattempts)
    , max_number_of_ios_device_(kDefaultMaxNumberOfiOSDevice)
    , wait_time_between_apps_(kDefaultWaitTimeBetweenApps)
    , enable_app_launch_ios_(kDefaultEnableAppLaunchIOS)
    , app_tranport_change_timer_(kDefaultAppTransportChangeTimer)
    , app_tranport_change_timer_addition_(
          kDefaultAppTransportChangeTimerAddition)
    , multiple_transports_enabled_(kDefaultMultipleTransportsEnabled)
    , error_occured_(false)
    , error_description_()
    , low_voltage_signal_offset_(kDefaultLowVoltageSignalOffset)
    , wake_up_signal_offset_(kDefaultWakeUpSignalOffset)
    , ignition_off_signal_offset_(kDefaultIgnitionOffSignalOffset) {
  // SDL version
  ReadStringValue(
      &sdl_version_, kDefaultSDLVersion, kMainSection, kSDLVersionKey);
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

int Profile::low_voltage_signal_offset() const {
  return low_voltage_signal_offset_;
}

int Profile::wake_up_signal_offset() const {
  return wake_up_signal_offset_;
}

int Profile::ignition_off_signal_offset() const {
  return ignition_off_signal_offset_;
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

bool Profile::use_full_app_id() const {
  return use_full_app_id_;
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

const std::string& Profile::transport_manager_tcp_adapter_network_interface()
    const {
  return transport_manager_tcp_adapter_network_interface_;
}

uint32_t Profile::cloud_app_retry_timeout() const {
  return cloud_app_retry_timeout_;
}

uint16_t Profile::cloud_app_max_retry_attempts() const {
  return cloud_app_max_retry_attempts_;
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

const std::pair<uint32_t, int32_t>&
Profile::get_interior_vehicle_data_frequency() const {
  return get_interior_vehicle_data_frequency_;
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
                kDefaultMaximumPayloadSize,
                kProtocolHandlerSection,
                kMaximumPayloadSizeKey);
  return maximum_payload_size;
}

size_t Profile::message_frequency_count() const {
  size_t message_frequency_count = 0;
  ReadUIntValue(&message_frequency_count,
                kDefaultFrequencyCount,
                kProtocolHandlerSection,
                kFrequencyCount);
  return message_frequency_count;
}

size_t Profile::message_frequency_time() const {
  size_t message_frequency_time = 0;
  ReadUIntValue(&message_frequency_time,
                kDefaultFrequencyTime,
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
                kDefaultMalformedFrequencyCount,
                kProtocolHandlerSection,
                kMalformedFrequencyCount);
  return malformed_frequency_count;
}

size_t Profile::malformed_frequency_time() const {
  size_t malformed_frequency_time = 0;
  ReadUIntValue(&malformed_frequency_time,
                kDefaultMalformedFrequencyTime,
                kProtocolHandlerSection,
                kMalformedFrequencyTime);
  return malformed_frequency_time;
}
uint32_t Profile::multiframe_waiting_timeout() const {
  uint32_t multiframe_waiting_timeout = 0;
  ReadUIntValue(&multiframe_waiting_timeout,
                kDefaultExpectedConsecutiveFramesTimeout,
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

const std::map<std::string, std::vector<std::string> >&
Profile::transport_required_for_resumption_map() const {
  return transport_required_for_resumption_map_;
}

const std::string& Profile::navigation_lowbandwidth_resumption_level() const {
  return navigation_lowbandwidth_resumption_level_;
}

const std::string& Profile::projection_lowbandwidth_resumption_level() const {
  return projection_lowbandwidth_resumption_level_;
}

const std::string& Profile::media_lowbandwidth_resumption_level() const {
  return media_lowbandwidth_resumption_level_;
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

const bool Profile::multiple_transports_enabled() const {
  return multiple_transports_enabled_;
}

const std::vector<std::string>& Profile::secondary_transports_for_bluetooth()
    const {
  return secondary_transports_for_bluetooth_;
}

const std::vector<std::string>& Profile::secondary_transports_for_usb() const {
  return secondary_transports_for_usb_;
}

const std::vector<std::string>& Profile::secondary_transports_for_wifi() const {
  return secondary_transports_for_wifi_;
}

const std::vector<std::string>& Profile::audio_service_transports() const {
  return audio_service_transports_;
}

const std::vector<std::string>& Profile::video_service_transports() const {
  return video_service_transports_;
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
      &sdl_version_, kDefaultSDLVersion, kMainSection, kSDLVersionKey);

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
      &link_to_web_hmi_, kDefaultLinkToWebHMI, kHmiSection, kLinkToWebHMI);
  LOG_UPDATED_BOOL_VALUE(link_to_web_hmi_, kLinkToWebHMI, kHmiSection);
#endif  // WEB_HMI

#ifdef ENABLE_SECURITY

  force_protected_service_ =
      ReadIntContainer(kSecuritySection, kForceProtectedService, NULL);

  force_unprotected_service_ =
      ReadIntContainer(kSecuritySection, kForceUnprotectedService, NULL);

  ReadStringValue(&security_manager_protocol_name_,
                  kDefaultSecurityProtocol,
                  kSecuritySection,
                  kSecurityProtocolKey);

  ReadStringValue(
      &cert_path_, "", kSecuritySection, kSecurityCertificatePathKey);
  ReadStringValue(
      &ca_cert_path_, "", kSecuritySection, kSecurityCACertificatePathKey);

  ReadStringValue(
      &ssl_mode_, kDefaultSSLMode, kSecuritySection, kSecuritySSLModeKey);

  ReadStringValue(&key_path_, "", kSecuritySection, kSecurityKeyPathKey);

  ReadStringValue(
      &ciphers_list_, SSL_TXT_ALL, kSecuritySection, kSecurityCipherListKey);

  ReadBoolValue(&verify_peer_,
                kDefaultVerifyPeer,
                kSecuritySection,
                kSecurityVerifyPeerKey);

  ReadUIntValue(&update_before_hours_,
                kDefaultBeforeUpdateHours,
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
                kDefaultAppIconsFolderMaxSize,
                kSDL4Section,
                kAppIconsFolderMaxSizeKey);

  if (app_icons_folder_max_size_ < kDefaultAppIconsFolderMaxSize) {
    app_icons_folder_max_size_ = kDefaultAppIconsFolderMaxSize;
  }

  LOG_UPDATED_VALUE(
      app_icons_folder_max_size_, kAppIconsFolderMaxSizeKey, kSDL4Section);

  // Application icon folder maximum size
  ReadUIntValue(&app_icons_amount_to_remove_,
                kDefaultAppIconsAmountToRemove,
                kSDL4Section,
                kAppIconsAmountToRemoveKey);

  LOG_UPDATED_VALUE(
      app_icons_amount_to_remove_, kAppIconsAmountToRemoveKey, kSDL4Section);

  ReadUIntValue(&maximum_control_payload_size_,
                kDefaultMaximumControlPayloadSize,
                kSDL5Section,
                kMaximumControlPayloadSizeKey);

  LOG_UPDATED_VALUE(maximum_control_payload_size_,
                    kMaximumControlPayloadSizeKey,
                    kSDL5Section);

  ReadUIntValue(&maximum_rpc_payload_size_,
                kDefaultMaximumRpcPayloadSize,
                kSDL5Section,
                kMaximumRpcPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_rpc_payload_size_, kMaximumRpcPayloadSizeKey, kSDL5Section);

  ReadUIntValue(&maximum_audio_payload_size_,
                kDefaultMaximumAudioPayloadSize,
                kSDL5Section,
                kMaximumAudioPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_audio_payload_size_, kMaximumAudioPayloadSizeKey, kSDL5Section);

  ReadUIntValue(&maximum_video_payload_size_,
                kDefaultMaximumVideoPayloadSize,
                kSDL5Section,
                kMaximumVideoPayloadSizeKey);

  LOG_UPDATED_VALUE(
      maximum_video_payload_size_, kMaximumVideoPayloadSizeKey, kSDL5Section);

  // Application info file name
  ReadStringValue(&app_info_storage_,
                  kDefaultAppInfoFileName,
                  kAppInfoSection,
                  kAppInfoStorageKey);

  LOG_UPDATED_VALUE(app_info_storage_, kAppInfoStorageKey, kAppInfoSection);

  // Server address
  ReadStringValue(
      &server_address_, kDefaultServerAddress, kHmiSection, kServerAddressKey);

  LOG_UPDATED_VALUE(server_address_, kServerAddressKey, kHmiSection);

  // HMI capabilities
  ReadStringValue(&hmi_capabilities_file_name_,
                  kDefaultHmiCapabilitiesFileName,
                  kMainSection,
                  kHmiCapabilitiesKey);

  hmi_capabilities_file_name_ =
      app_config_folder_ + "/" + hmi_capabilities_file_name_;

  LOG_UPDATED_VALUE(
      hmi_capabilities_file_name_, kHmiCapabilitiesKey, kMainSection);

  // Server port
  ReadUIntValue(&server_port_, kDefaultServerPort, kHmiSection, kServerPortKey);

  LOG_UPDATED_VALUE(server_port_, kServerPortKey, kHmiSection);

  // Video streaming port
  ReadUIntValue(&video_streaming_port_,
                kDefaultVideoStreamingPort,
                kHmiSection,
                kVideoStreamingPortKey);

  LOG_UPDATED_VALUE(video_streaming_port_, kVideoStreamingPortKey, kHmiSection);
  // Audio streaming port
  ReadUIntValue(&audio_streaming_port_,
                kDefaultAudioStreamingPort,
                kHmiSection,
                kAudioStreamingPortKey);

  LOG_UPDATED_VALUE(audio_streaming_port_, kAudioStreamingPortKey, kHmiSection);

  // Streaming timeout
  ReadUIntValue(&stop_streaming_timeout_,
                kDefaultStopStreamingTimeout,
                kMediaManagerSection,
                kStopStreamingTimeout);

  stop_streaming_timeout_ =
      std::max(kDefaultStopStreamingTimeout, stop_streaming_timeout_);

  LOG_UPDATED_VALUE(
      stop_streaming_timeout_, kStopStreamingTimeout, kHmiSection);

  // Time testing port
  ReadUIntValue(&time_testing_port_,
                kDefaultTimeTestingPort,
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
                kDefaultAudioDataStoppedTimeout,
                kMediaManagerSection,
                kAudioDataStoppedTimeoutKey);

  LOG_UPDATED_VALUE(audio_data_stopped_timeout_,
                    kAudioDataStoppedTimeoutKey,
                    kMediaManagerSection);

  ReadUIntValue(&video_data_stopped_timeout_,
                kDefaultVideoDataStoppedTimeout,
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
  ReadUIntValue(&max_cmd_id_, kDefaultMaxCmdId, kMainSection, kMaxCmdIdKey);

  LOG_UPDATED_VALUE(max_cmd_id_, kMaxCmdIdKey, kMainSection);

  // PutFile restrictions
  ReadUIntValue(&put_file_in_none_,
                kDefaultPutFileRequestInNone,
                kFilesystemRestrictionsSection,
                kPutFileRequestKey);

  LOG_UPDATED_VALUE(
      put_file_in_none_, kPutFileRequestKey, kFilesystemRestrictionsSection);

  // DeleteFileRestrictions
  ReadUIntValue(&delete_file_in_none_,
                kDefaultDeleteFileRequestInNone,
                kFilesystemRestrictionsSection,
                kDeleteFileRequestKey);

  LOG_UPDATED_VALUE(delete_file_in_none_,
                    kDeleteFileRequestKey,
                    kFilesystemRestrictionsSection);

  // ListFiles restrictions
  ReadUIntValue(&list_files_in_none_,
                kDefaultListFilesRequestInNone,
                kFilesystemRestrictionsSection,
                kListFilesRequestKey);

  LOG_UPDATED_VALUE(list_files_in_none_,
                    kListFilesRequestKey,
                    kFilesystemRestrictionsSection);

  // ListFiles request size
  ReadUIntValue(&list_files_response_size_,
                kDefaultListFilesResponseSize,
                kFilesystemRestrictionsSection,
                kListFilesResponseSizeKey);

  LOG_UPDATED_VALUE(list_files_response_size_,
                    kListFilesResponseSizeKey,
                    kFilesystemRestrictionsSection);

  // Default timeout
  ReadUIntValue(
      &default_timeout_, kDefaultTimeout, kMainSection, kDefaultTimeoutKey);

  if (default_timeout_ == 0) {
    default_timeout_ = kDefaultTimeout;
  }

  LOG_UPDATED_VALUE(default_timeout_, kDefaultTimeoutKey, kMainSection);

  // Application resuming timeout
  ReadUIntValue(&app_resuming_timeout_,
                kDefaultAppResumingTimeout,
                kResumptionSection,
                kAppResumingTimeoutKey);

  if (app_resuming_timeout_ == 0) {
    app_resuming_timeout_ = kDefaultAppResumingTimeout;
  }
  // Save resumption info to File System
  LOG_UPDATED_VALUE(app_resuming_timeout_,
                    kAppSavePersistentDataTimeoutKey,
                    kResumptionSection);

  ReadUIntValue(&app_resumption_save_persistent_data_timeout_,
                kDefaultAppSavePersistentDataTimeout,
                kResumptionSection,
                kAppSavePersistentDataTimeoutKey);
  if (app_resuming_timeout_ == 0) {
    app_resuming_timeout_ = kDefaultAppSavePersistentDataTimeout;
  }

  LOG_UPDATED_VALUE(
      app_resuming_timeout_, kAppResumingTimeoutKey, kResumptionSection);
  // Open attempt timeout in ms
  ReadUIntValue(&resumption_delay_before_ign_,
                kDefaultResumptionDelayBeforeIgn,
                kResumptionSection,
                kResumptionDelayBeforeIgnKey);

  LOG_UPDATED_VALUE(resumption_delay_before_ign_,
                    kResumptionDelayBeforeIgnKey,
                    kResumptionSection);

  // Open attempt timeout in ms
  ReadUIntValue(&resumption_delay_after_ign_,
                kDefaultResumptionDelayAfterIgn,
                kResumptionSection,
                kResumptionDelayAfterIgnKey);

  LOG_UPDATED_VALUE(resumption_delay_after_ign_,
                    kResumptionDelayAfterIgnKey,
                    kResumptionSection);

  // Application directory quota
  ReadUIntValue(
      &app_dir_quota_, kDefaultDirQuota, kMainSection, kAppDirectoryQuotaKey);

  if (app_dir_quota_ == 0) {
    app_dir_quota_ = kDefaultDirQuota;
  }

  LOG_UPDATED_VALUE(app_dir_quota_, kAppDirectoryQuotaKey, kMainSection);

  // TTS delimiter
  // Should be gotten before any TTS prompts, since it should be appended back
  ReadStringValue(&tts_delimiter_,
                  kDefaultTtsDelimiter,
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
                kDefaultTTSGlobalPropertiesTimeout,
                kGlobalPropertiesSection,
                kTTSGlobalPropertiesTimeoutKey);

  LOG_UPDATED_VALUE(tts_global_properties_timeout_,
                    kTTSGlobalPropertiesTimeoutKey,
                    kGlobalPropertiesSection);

  // Application time scale maximum requests
  ReadUIntValue(&app_time_scale_max_requests_,
                kDefaultAppTimeScaleMaxRequests,
                kMainSection,
                kAppTimeScaleMaxRequestsKey);

  LOG_UPDATED_VALUE(
      app_time_scale_max_requests_, kAppTimeScaleMaxRequestsKey, kMainSection);

  // Application time scale
  ReadUIntValue(&app_requests_time_scale_,
                kDefaultAppRequestsTimeScale,
                kMainSection,
                kAppRequestsTimeScaleKey);

  LOG_UPDATED_VALUE(
      app_requests_time_scale_, kAppRequestsTimeScaleKey, kMainSection);

  // Application HMI level NONE time scale maximum requests
  ReadUIntValue(&app_hmi_level_none_time_scale_max_requests_,
                kDefaultAppHmiLevelNoneTimeScaleMaxRequests,
                kMainSection,
                kAppHmiLevelNoneTimeScaleMaxRequestsKey);

  LOG_UPDATED_VALUE(app_hmi_level_none_time_scale_max_requests_,
                    kAppHmiLevelNoneTimeScaleMaxRequestsKey,
                    kMainSection);

  // Application HMI level NONE requests time scale
  ReadUIntValue(&app_hmi_level_none_requests_time_scale_,
                kDefaultAppHmiLevelNoneRequestsTimeScale,
                kMainSection,
                kAppHmiLevelNoneRequestsTimeScaleKey);

  LOG_UPDATED_VALUE(app_hmi_level_none_requests_time_scale_,
                    kAppHmiLevelNoneRequestsTimeScaleKey,
                    kMainSection);

  // Amount of pending requests
  ReadUIntValue(&pending_requests_amount_,
                kDefaultPendingRequestsAmount,
                kMainSection,
                kPendingRequestsAmoundKey);

  if (pending_requests_amount_ == 0) {
    pending_requests_amount_ = kDefaultPendingRequestsAmount;
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
                  kDefaultSystemFilesPath,
                  kMainSection,
                  kSystemFilesPathKey);

  LOG_UPDATED_VALUE(system_files_path_, kSystemFilesPathKey, kMainSection);

  // Plugins folder
  ReadStringValue(
      &plugins_folder_, kDefaultPluginsPath, kMainSection, kPluginsFolderKey);
  LOG_UPDATED_VALUE(plugins_folder_, kPluginsFolderKey, kMainSection);
  // Heartbeat timeout
  ReadUIntValue(&heart_beat_timeout_,
                kDefaultHeartBeatTimeout,
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

  // Transport manager TCP network interface
  ReadStringValue(&transport_manager_tcp_adapter_network_interface_,
                  kDefaultTransportManagerTCPAdapterNetworkInterface,
                  kTransportManagerSection,
                  kTCPAdapterNetworkInterfaceKey);

  LOG_UPDATED_VALUE(transport_manager_tcp_adapter_network_interface_,
                    kTCPAdapterNetworkInterfaceKey,
                    kTransportManagerSection);

  ReadUIntValue(&cloud_app_retry_timeout_,
                kDefaultCloudAppRetryTimeout,
                kCloudAppTransportSection,
                kCloudAppRetryTimeoutKey);

  LOG_UPDATED_VALUE(cloud_app_retry_timeout_,
                    kCloudAppRetryTimeoutKey,
                    kCloudAppTransportSection);

  ReadUIntValue(&cloud_app_max_retry_attempts_,
                kDefaultCloudAppMaxRetryAttempts,
                kCloudAppTransportSection,
                kCloudAppMaxRetryAttemptsKey);

  LOG_UPDATED_VALUE(cloud_app_max_retry_attempts_,
                    kCloudAppMaxRetryAttemptsKey,
                    kCloudAppTransportSection);

  // Event MQ
  ReadStringValue(
      &event_mq_name_, kDefaultEventMQ, kTransportManagerSection, kEventMQKey);

  LOG_UPDATED_VALUE(event_mq_name_, kEventMQKey, kTransportManagerSection);

  // Ack MQ
  ReadStringValue(
      &ack_mq_name_, kDefaultAckMQ, kTransportManagerSection, kAckMQKey);

  LOG_UPDATED_VALUE(ack_mq_name_, kAckMQKey, kTransportManagerSection);

  // Transport manager disconnect timeout
  ReadUIntValue(&transport_manager_disconnect_timeout_,
                kDefaultTransportManagerDisconnectTimeout,
                kTransportManagerSection,
                kTransportManagerDisconnectTimeoutKey);

  LOG_UPDATED_VALUE(transport_manager_disconnect_timeout_,
                    kTransportManagerDisconnectTimeoutKey,
                    kTransportManagerSection);

  // Recording file
  ReadStringValue(&recording_file_name_,
                  kDefaultRecordingFileName,
                  kMediaManagerSection,
                  kRecordingFileNameKey);

  LOG_UPDATED_VALUE(
      recording_file_name_, kRecordingFileNameKey, kMediaManagerSection);

  // Recording file source
  ReadStringValue(&recording_file_source_,
                  kDefaultRecordingFileSourceName,
                  kMediaManagerSection,
                  kRecordingFileSourceKey);

  LOG_UPDATED_VALUE(
      recording_file_source_, kRecordingFileSourceKey, kMediaManagerSection);

  // Policy preloaded file
  ReadStringValue(&preloaded_pt_file_,
                  kDefaultPreloadedPTFileName,
                  kPolicySection,
                  kPreloadedPTKey);

  preloaded_pt_file_ = app_config_folder_ + '/' + preloaded_pt_file_;

  LOG_UPDATED_VALUE(preloaded_pt_file_, kPreloadedPTKey, kPolicySection);

  // Policy snapshot file
  ReadStringValue(&policy_snapshot_file_name_,
                  kDefaultPoliciesSnapshotFileName,
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
                kDefaultAttemptsToOpenPolicyDB,
                kPolicySection,
                kAttemptsToOpenPolicyDBKey);

  LOG_UPDATED_VALUE(
      attempts_to_open_policy_db_, kAttemptsToOpenPolicyDBKey, kPolicySection);

  // Open attempt timeout in ms
  ReadUIntValue(&open_attempt_timeout_ms_,
                kDefaultOpenAttemptTimeoutMs,
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

  // Use full app ID internally?
  std::string use_full_id_string;
  if (ReadValue(&use_full_id_string, kPolicySection, kUseFullAppID) &&
      0 == strcmp("true", use_full_id_string.c_str())) {
    use_full_app_id_ = true;
  } else {
    use_full_app_id_ = false;
  }

  // Max protocol version
  ReadUIntValue(&max_supported_protocol_version_,
                kDefaultMaxSupportedProtocolVersion,
                kProtocolHandlerSection,
                kMaxSupportedProtocolVersionKey);

  if (max_supported_protocol_version_ < 1) {
    max_supported_protocol_version_ = 1;
  } else if (max_supported_protocol_version_ >
             kDefaultMaxSupportedProtocolVersion) {
    max_supported_protocol_version_ = kDefaultMaxSupportedProtocolVersion;
  }

  LOG_UPDATED_BOOL_VALUE(enable_policy_, kEnablePolicy, kPolicySection);

  ReadUIntValue(&application_list_update_timeout_,
                kDefaultApplicationListUpdateTimeout,
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

  ReadUintIntPairValue(&get_interior_vehicle_data_frequency_,
                       kGetInteriorVehicleDataFrequency,
                       kMainSection,
                       kGetInteriorVehicleDataFrequencyKey);

  ReadUIntValue(&max_thread_pool_size_,
                kDefaultMaxThreadPoolSize,
                kApplicationManagerSection,
                kDefaultThreadPoolSize);
  if (max_thread_pool_size_ > kDefaultMaxThreadPoolSize) {
    max_thread_pool_size_ = kDefaultMaxThreadPoolSize;
  }
  LOG_UPDATED_VALUE(max_thread_pool_size_,
                    kDefaultMaxThreadPoolSize,
                    kApplicationManagerSection);

  ReadStringValue(&iap_legacy_protocol_mask_,
                  kDefaultLegacyProtocolMask,
                  kIAPSection,
                  kLegacyProtocolMaskKey);

  LOG_UPDATED_VALUE(
      iap_legacy_protocol_mask_, kLegacyProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_hub_protocol_mask_,
                  kDefaultHubProtocolMask,
                  kIAPSection,
                  kHubProtocolMaskKey);

  LOG_UPDATED_VALUE(iap_hub_protocol_mask_, kHubProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_pool_protocol_mask_,
                  kDefaultPoolProtocolMask,
                  kIAPSection,
                  kPoolProtocolMaskKey);

  LOG_UPDATED_VALUE(iap_pool_protocol_mask_, kPoolProtocolMaskKey, kIAPSection);

  ReadStringValue(&iap_system_config_,
                  kDefaultIAPSystemConfig,
                  kIAPSection,
                  kIAPSystemConfigKey);

  LOG_UPDATED_VALUE(iap_system_config_, kIAPSystemConfigKey, kIAPSection);

  ReadStringValue(&iap2_system_config_,
                  kDefaultIAP2SystemConfig,
                  kIAPSection,
                  kIAP2SystemConfigKey);

  LOG_UPDATED_VALUE(iap2_system_config_, kIAP2SystemConfigKey, kIAPSection);

  ReadIntValue(&iap2_hub_connect_attempts_,
               kDefaultIAP2HubConnectAttempts,
               kIAPSection,
               kIAP2HubConnectAttemptskey);

  LOG_UPDATED_VALUE(
      iap2_hub_connect_attempts_, kIAP2HubConnectAttemptskey, kIAPSection);

  ReadIntValue(&iap_hub_connection_wait_timeout_,
               kDefaultIAPHubConnectionWaitTimeout,
               kIAPSection,
               kIAPHubConnectionWaitTimeoutKey);

  LOG_UPDATED_VALUE(iap_hub_connection_wait_timeout_,
                    kIAPHubConnectionWaitTimeoutKey,
                    kIAPSection);

  ReadUIntValue(&default_hub_protocol_index_,
                kDefaultHubProtocolIndex,
                kIAPSection,
                kDefaultHubProtocolIndexKey);

  LOG_UPDATED_VALUE(
      default_hub_protocol_index_, kDefaultHubProtocolIndexKey, kIAPSection);

  ReadUIntValue(&hash_string_size_,
                kDefaultHashStringSize,
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
                kDefaultAttemptsToOpenResumptionDB,
                kResumptionSection,
                kAttemptsToOpenResumptionDBKey);

  LOG_UPDATED_VALUE(attempts_to_open_resumption_db_,
                    kAttemptsToOpenResumptionDBKey,
                    kResumptionSection);

  ReadUIntValue(&open_attempt_timeout_ms_resumption_db_,
                kDefaultOpenAttemptTimeoutMsResumptionDB,
                kResumptionSection,
                kOpenAttemptTimeoutMsResumptionDBKey);

  LOG_UPDATED_VALUE(open_attempt_timeout_ms_resumption_db_,
                    kOpenAttemptTimeoutMsResumptionDBKey,
                    kResumptionSection);

  {  // read parameters from TransportRequiredForResumption section
    struct KeyPair {
      const char* ini_key_name;
      const char* map_key_name;
    } keys[] = {
        {kDefaultTransportRequiredForResumptionKey, kAppHMITypeDefault},
        {kCommunicationTransportRequiredForResumptionKey,
         kAppHMITypeCommunication},
        {kMediaTransportRequiredForResumptionKey, kAppHMITypeMedia},
        {kMessagingTransportRequiredForResumptionKey, kAppHMITypeMessaging},
        {kNavigationTransportRequiredForResumptionKey, kAppHMITypeNavigation},
        {kInformationTransportRequiredForResumptionKey, kAppHMITypeInformation},
        {kSocialTransportRequiredForResumptionKey, kAppHMITypeSocial},
        {kBackgroundProcessTransportRequiredForResumptionKey,
         kAppHMITypeBackgroundProcess},
        {kTestingTransportRequiredForResumptionKey, kAppHMITypeTesting},
        {kSystemTransportRequiredForResumptionKey, kAppHMITypeSystem},
        {kProjectionTransportRequiredForResumptionKey, kAppHMITypeProjection},
        {kRemoteControlTransportRequiredForResumptionKey,
         kAppHMITypeRemoteControl},
        {kEmptyAppTransportRequiredForResumptionKey, kAppHMITypeEmptyApp},
        {NULL, NULL}};
    struct KeyPair* entry = keys;

    while (entry->ini_key_name != NULL) {
      bool exist = false;
      std::vector<std::string> transport_list =
          ReadStringContainer(kTransportRequiredForResumptionSection,
                              entry->ini_key_name,
                              &exist,
                              true);
      if (exist) {
        transport_required_for_resumption_map_[entry->map_key_name] =
            transport_list;

        const std::string list_with_comma = std::accumulate(
            transport_list.begin(),
            transport_list.end(),
            std::string(""),
            [](std::string& first, std::string& second) {
              return first.empty() ? second : first + ", " + second;
            });
        LOG_UPDATED_VALUE(list_with_comma,
                          entry->ini_key_name,
                          kTransportRequiredForResumptionSection);
      }
      entry++;
    }
  }

  // Read parameters from LowBandwidthTransportResumptionLevel section
  ReadStringValue(&navigation_lowbandwidth_resumption_level_,
                  kDefaultLowBandwidthResumptionLevel,
                  kLowBandwidthTransportResumptionLevelSection,
                  kNavigationLowBandwidthResumptionLevelKey);

  LOG_UPDATED_VALUE(navigation_lowbandwidth_resumption_level_,
                    kNavigationLowBandwidthResumptionLevelKey,
                    kLowBandwidthTransportResumptionLevelSection);

  ReadStringValue(&projection_lowbandwidth_resumption_level_,
                  kDefaultLowBandwidthResumptionLevel,
                  kLowBandwidthTransportResumptionLevelSection,
                  kProjectionLowBandwidthResumptionLevelKey);

  LOG_UPDATED_VALUE(projection_lowbandwidth_resumption_level_,
                    kProjectionLowBandwidthResumptionLevelKey,
                    kLowBandwidthTransportResumptionLevelSection);

  ReadStringValue(&media_lowbandwidth_resumption_level_,
                  kDefaultLowBandwidthResumptionLevel,
                  kLowBandwidthTransportResumptionLevelSection,
                  kMediaLowBandwidthResumptionLevelKey);

  LOG_UPDATED_VALUE(media_lowbandwidth_resumption_level_,
                    kMediaLowBandwidthResumptionLevelKey,
                    kLowBandwidthTransportResumptionLevelSection);

  // Read parameters from App Launch section
  ReadUIntValue(&app_launch_wait_time_,
                kDefaultAppLaunchWaitTime,
                kAppLaunchSection,
                kAppLaunchWaitTimeKey);

  LOG_UPDATED_VALUE(
      app_launch_wait_time_, kAppLaunchWaitTimeKey, kAppLaunchSection);

  ReadUIntValue(&app_launch_max_retry_attempt_,
                kDefaultAppLaunchMaxRetryAttempt,
                kAppLaunchSection,
                kAppLaunchMaxRetryAttemptKey);

  LOG_UPDATED_VALUE(app_launch_max_retry_attempt_,
                    kAppLaunchMaxRetryAttemptKey,
                    kAppLaunchSection);

  ReadUIntValue(&app_launch_retry_wait_time_,
                kDefaultAppLaunchRetryWaitTime,
                kAppLaunchSection,
                kAppLaunchRetryWaitTimeKey);

  LOG_UPDATED_VALUE(app_launch_retry_wait_time_,
                    kAppLaunchRetryWaitTimeKey,
                    kAppLaunchSection);

  ReadUIntValue(&remove_bundle_id_attempts_,
                kDefaultRemoveBundleIDattempts,
                kAppLaunchSection,
                kRemoveBundleIDattemptsKey);

  LOG_UPDATED_VALUE(remove_bundle_id_attempts_,
                    kRemoveBundleIDattemptsKey,
                    kAppLaunchSection);

  ReadUIntValue(&max_number_of_ios_device_,
                kDefaultMaxNumberOfiOSDevice,
                kAppLaunchSection,
                kMaxNumberOfiOSDeviceKey);

  LOG_UPDATED_VALUE(
      max_number_of_ios_device_, kMaxNumberOfiOSDeviceKey, kAppLaunchSection);

  ReadUIntValue(&wait_time_between_apps_,
                kDefaultWaitTimeBetweenApps,
                kAppLaunchSection,
                kWaitTimeBetweenAppsKey);

  LOG_UPDATED_VALUE(
      wait_time_between_apps_, kWaitTimeBetweenAppsKey, kAppLaunchSection);

  ReadBoolValue(&enable_app_launch_ios_,
                kDefaultEnableAppLaunchIOS,
                kAppLaunchSection,
                kEnableAppLaunchIOSKey);

  LOG_UPDATED_BOOL_VALUE(
      enable_app_launch_ios_, kEnableAppLaunchIOSKey, kAppLaunchSection);

  ReadUIntValue(&app_tranport_change_timer_,
                kDefaultAppTransportChangeTimer,
                kMainSection,
                kAppTransportChangeTimerKey);

  LOG_UPDATED_VALUE(
      app_tranport_change_timer_, kAppTransportChangeTimerKey, kMainSection);

  ReadUIntValue(&app_tranport_change_timer_addition_,
                kDefaultAppTransportChangeTimerAddition,
                kMainSection,
                kAppTransportChangeTimerAdditionKey);

  LOG_UPDATED_VALUE(app_tranport_change_timer_addition_,
                    kAppTransportChangeTimerAdditionKey,
                    kMainSection);

  ReadIntValue(&low_voltage_signal_offset_,
               kDefaultLowVoltageSignalOffset,
               kMainSection,
               kLowVoltageSignalOffsetKey);

  LOG_UPDATED_VALUE(
      low_voltage_signal_offset_, kLowVoltageSignalOffsetKey, kMainSection);

  ReadIntValue(&wake_up_signal_offset_,
               kDefaultWakeUpSignalOffset,
               kMainSection,
               kWakeUpSignalOffsetKey);

  LOG_UPDATED_VALUE(
      wake_up_signal_offset_, kWakeUpSignalOffsetKey, kMainSection);

  ReadIntValue(&ignition_off_signal_offset_,
               kDefaultIgnitionOffSignalOffset,
               kMainSection,
               kIgnitionOffSignalOffsetKey);

  LOG_UPDATED_VALUE(
      ignition_off_signal_offset_, kIgnitionOffSignalOffsetKey, kMainSection);

  ReadBoolValue(&multiple_transports_enabled_,
                kDefaultMultipleTransportsEnabled,
                kMultipleTransportsSection,
                kMultipleTransportsEnabledKey);

  LOG_UPDATED_BOOL_VALUE(multiple_transports_enabled_,
                         kMultipleTransportsEnabledKey,
                         kMultipleTransportsSection);

  {  // Secondary Transports and ServicesMap
    struct KeyPair {
      std::vector<std::string>* ini_vector;
      const char* ini_section_name;
      const char* ini_key_name;
    } keys[] = {{&secondary_transports_for_bluetooth_,
                 kMultipleTransportsSection,
                 kSecondaryTransportForBluetoothKey},
                {&secondary_transports_for_usb_,
                 kMultipleTransportsSection,
                 kSecondaryTransportForUSBKey},
                {&secondary_transports_for_wifi_,
                 kMultipleTransportsSection,
                 kSecondaryTransportForWiFiKey},
                {&audio_service_transports_,
                 kServicesMapSection,
                 kAudioServiceTransportsKey},
                {&video_service_transports_,
                 kServicesMapSection,
                 kVideoServiceTransportsKey},
                {NULL, NULL, NULL}};
    struct KeyPair* entry = keys;

    while (entry->ini_vector != NULL) {
      bool exist = false;
      std::vector<std::string> profile_entry = ReadStringContainer(
          entry->ini_section_name, entry->ini_key_name, &exist, true);
      if (exist) {
        *entry->ini_vector = profile_entry;

        const std::string list_with_comma = std::accumulate(
            profile_entry.begin(),
            profile_entry.end(),
            std::string(""),
            [](std::string& first, std::string& second) {
              return first.empty() ? second : first + ", " + second;
            });
        LOG_UPDATED_VALUE(
            list_with_comma, entry->ini_key_name, entry->ini_section_name);
      }
      entry++;
    }
  }
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
  return ReadValueEmpty(value, pSection, pKey) && "\0" != *value;
}

bool Profile::ReadValueEmpty(std::string* value,
                             const char* const pSection,
                             const char* const pKey) const {
  DCHECK(value);
  bool ret = false;

  char buf[INI_LINE_LEN + 1];
  *buf = '\0';
  if (0 != ini_read_value(config_file_name_.c_str(), pSection, pKey, buf)) {
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

std::string trim_string(const std::string& str) {
  const char* delims = " \t";

  size_t start = str.find_first_not_of(delims);
  if (std::string::npos == start) {
    return std::string();
  }
  size_t end = str.find_last_not_of(delims);

  return str.substr(start, end - start + 1);
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
    bool* out_result,
    bool allow_empty) const {
  std::string string;
  bool result;
  if (allow_empty) {
    result = ReadValueEmpty(&string, pSection, pKey);
  } else {
    result = ReadValue(&string, pSection, pKey);
  }
  if (out_result)
    *out_result = result;
  std::vector<std::string> value_container;
  if (result) {
    std::istringstream iss(string);
    std::string temp_str;
    while (iss) {
      if (!getline(iss, temp_str, ','))
        break;
      value_container.push_back(trim_string(temp_str));
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
