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

#include "config_profile/profile_default_values.h"

namespace profile {
namespace default_values {

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
const char* kApplicationManagerSection = "ApplicationManager";
const char* kFilesystemRestrictionsSection = "FILESYSTEM RESTRICTIONS";
const char* kIAPSection = "IAP";
const char* kProtocolHandlerSection = "ProtocolHandler";
const char* kSDL4Section = "SDL4";
const char* kSDL5Section = "SDL5";
const char* kResumptionSection = "Resumption";
const char* kAppLaunchSection = "AppLaunch";

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
const char* kEventMQKey = "EventMQ";
const char* kAckMQKey = "AckMQ";
const char* kApplicationListUpdateTimeoutKey = "ApplicationListUpdateTimeout";
const char* kReadDIDFrequencykey = "ReadDIDRequest";
const char* kGetVehicleDataFrequencyKey = "GetVehicleDataRequest";
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
#ifdef WEB_HMI
const char* kDefaultLinkToWebHMI = "HMI//index.html";
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
const uint16_t kDefaultServerPort = 8087;
const uint16_t kDefaultVideoStreamingPort = 5050;
const uint16_t kDefaultAudioStreamingPort = 5080;
const uint32_t kDefaultStopStreamingTimeout = 10000;
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
const std::string kAllowedSymbols =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_.-";

}  // namespace default_values
}  // namespace profile
