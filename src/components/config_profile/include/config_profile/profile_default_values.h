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

#pragma once

#include <stdint.h>
#include <utility>
#include <cstddef>
#include <string>

namespace profile {
namespace default_values {
extern const char* kDefaultConfigFileName;

extern const char* kMainSection;
#ifdef ENABLE_SECURITY
extern const char* kSecuritySection;
extern const char* kForceProtectedService;
extern const char* kForceUnprotectedService;
#endif
extern const char* kPolicySection;
extern const char* kHmiSection;
extern const char* kAppInfoSection;
extern const char* kMediaManagerSection;
extern const char* kGlobalPropertiesSection;
extern const char* kVrCommandsSection;
extern const char* kTransportManagerSection;
extern const char* kApplicationManagerSection;
extern const char* kFilesystemRestrictionsSection;
extern const char* kIAPSection;
extern const char* kProtocolHandlerSection;
extern const char* kSDL4Section;
extern const char* kSDL5Section;
extern const char* kResumptionSection;
extern const char* kAppLaunchSection;

extern const char* kSDLVersionKey;
extern const char* kHmiCapabilitiesKey;
extern const char* kPathToSnapshotKey;
extern const char* kPreloadedPTKey;
extern const char* kAttemptsToOpenPolicyDBKey;
extern const char* kOpenAttemptTimeoutMsKey;
extern const char* kServerAddressKey;
extern const char* kAppInfoStorageKey;
extern const char* kAppStorageFolderKey;
extern const char* kAppResourseFolderKey;
extern const char* kLogsEnabledKey;
extern const char* kAppConfigFolderKey;
extern const char* kAppIconsFolderKey;
extern const char* kAppIconsFolderMaxSizeKey;
extern const char* kAppIconsAmountToRemoveKey;
extern const char* kMaximumControlPayloadSizeKey;
extern const char* kMaximumRpcPayloadSizeKey;
extern const char* kMaximumAudioPayloadSizeKey;
extern const char* kMaximumVideoPayloadSizeKey;
extern const char* kLaunchHMIKey;
extern const char* kDefaultSDLVersion;
#ifdef WEB_HMI
extern const char* kLinkToWebHMI;
#endif  // WEB_HMI
extern const char* kStartStreamRetry;
extern const char* kEnableRedecodingKey;
extern const char* kVideoStreamConsumerKey;
extern const char* kAudioStreamConsumerKey;
extern const char* kNamedVideoPipePathKey;
extern const char* kNamedAudioPipePathKey;
extern const char* kVideoStreamFileKey;
extern const char* kAudioStreamFileKey;

#ifdef ENABLE_SECURITY
extern const char* kSecurityProtocolKey;
extern const char* kSecurityCertificatePathKey;
extern const char* kSecurityCACertificatePathKey;
extern const char* kSecuritySSLModeKey;
extern const char* kSecurityKeyPathKey;
extern const char* kSecurityCipherListKey;
extern const char* kSecurityVerifyPeerKey;
extern const char* kBeforeUpdateHours;
#endif

extern const char* kAudioDataStoppedTimeoutKey;
extern const char* kVideoDataStoppedTimeoutKey;
extern const char* kMixingAudioSupportedKey;
extern const char* kHelpPromptKey;
extern const char* kTimeoutPromptKey;
extern const char* kHelpTitleKey;
extern const char* kHelpCommandKey;
extern const char* kSystemFilesPathKey;
extern const char* kPluginsFolderKey;
extern const char* kHeartBeatTimeoutKey;
extern const char* kMaxSupportedProtocolVersionKey;
extern const char* kUseLastStateKey;
extern const char* kTCPAdapterPortKey;
extern const char* kServerPortKey;
extern const char* kVideoStreamingPortKey;
extern const char* kAudioStreamingPortKey;
extern const char* kStopStreamingTimeout;
extern const char* kTimeTestingPortKey;
extern const char* kThreadStackSizeKey;
extern const char* kMaxCmdIdKey;
extern const char* kPutFileRequestKey;
extern const char* kDeleteFileRequestKey;
extern const char* kListFilesRequestKey;
extern const char* kListFilesResponseSizeKey;
extern const char* kDefaultTimeoutKey;
extern const char* kAppResumingTimeoutKey;
extern const char* kAppSavePersistentDataTimeoutKey;
extern const char* kResumptionDelayBeforeIgnKey;
extern const char* kResumptionDelayAfterIgnKey;
extern const char* kAppDirectoryQuotaKey;
extern const char* kAppTimeScaleMaxRequestsKey;
extern const char* kAppRequestsTimeScaleKey;
extern const char* kAppHmiLevelNoneTimeScaleMaxRequestsKey;
extern const char* kAppHmiLevelNoneRequestsTimeScaleKey;
extern const char* kPendingRequestsAmoundKey;
extern const char* kSupportedDiagModesKey;
extern const char* kTransportManagerDisconnectTimeoutKey;
extern const char* kTTSDelimiterKey;
extern const char* kRecordingFileNameKey;
extern const char* kRecordingFileSourceKey;
extern const char* kEnablePolicy;
extern const char* kEventMQKey;
extern const char* kAckMQKey;
extern const char* kApplicationListUpdateTimeoutKey;
extern const char* kReadDIDFrequencykey;
extern const char* kGetVehicleDataFrequencyKey;
extern const char* kLegacyProtocolMaskKey;
extern const char* kHubProtocolMaskKey;
extern const char* kPoolProtocolMaskKey;
extern const char* kIAPSystemConfigKey;
extern const char* kIAP2SystemConfigKey;
extern const char* kIAP2HubConnectAttemptskey;
extern const char* kIAPHubConnectionWaitTimeoutKey;
extern const char* kDefaultHubProtocolIndexKey;
extern const char* kTTSGlobalPropertiesTimeoutKey;
extern const char* kMaximumPayloadSizeKey;
extern const char* kFrequencyCount;
extern const char* kFrequencyTime;
extern const char* kMalformedMessageFiltering;
extern const char* kMalformedFrequencyCount;
extern const char* kMalformedFrequencyTime;
extern const char* kExpectedConsecutiveFramesTimeout;
extern const char* kHashStringSizeKey;
extern const char* kUseDBForResumptionKey;
extern const char* kAttemptsToOpenResumptionDBKey;
extern const char* kOpenAttemptTimeoutMsResumptionDBKey;

extern const char* kAppLaunchWaitTimeKey;
extern const char* kAppLaunchMaxRetryAttemptKey;
extern const char* kAppLaunchRetryWaitTimeKey;
extern const char* kRemoveBundleIDattemptsKey;
extern const char* kMaxNumberOfiOSDeviceKey;
extern const char* kWaitTimeBetweenAppsKey;
extern const char* kEnableAppLaunchIOSKey;
extern const char* kAppTransportChangeTimerKey;
extern const char* kAppTransportChangeTimerAdditionKey;
#ifdef WEB_HMI
extern const char* kDefaultLinkToWebHMI;
#endif  // WEB_HMI
extern const char* kDefaultPoliciesSnapshotFileName;
extern const char* kDefaultHmiCapabilitiesFileName;
extern const char* kDefaultPreloadedPTFileName;
extern const char* kDefaultServerAddress;
extern const char* kDefaultAppInfoFileName;
extern const char* kDefaultSystemFilesPath;
extern const char* kDefaultPluginsPath;
extern const char* kDefaultTtsDelimiter;
extern const uint32_t kDefaultAudioDataStoppedTimeout;
extern const uint32_t kDefaultVideoDataStoppedTimeout;
extern const char* kDefaultEventMQ;
extern const char* kDefaultAckMQ;
extern const char* kDefaultRecordingFileSourceName;
extern const char* kDefaultRecordingFileName;
extern const char* kDefaultThreadPoolSize;
extern const char* kDefaultLegacyProtocolMask;
extern const char* kDefaultHubProtocolMask;
extern const char* kDefaultPoolProtocolMask;
extern const char* kDefaultIAPSystemConfig;
extern const char* kDefaultIAP2SystemConfig;

#ifdef ENABLE_SECURITY
extern const char* kDefaultSecurityProtocol;
extern const char* kDefaultSSLMode;
extern const bool kDefaultVerifyPeer;
extern const uint32_t kDefaultBeforeUpdateHours;
#endif  // ENABLE_SECURITY

extern const uint32_t kDefaultHubProtocolIndex;
extern const uint32_t kDefaultHeartBeatTimeout;
extern const uint16_t kDefaultMaxSupportedProtocolVersion;
extern const uint16_t kDefautTransportManagerTCPPort;
extern const uint16_t kDefaultServerPort;
extern const uint16_t kDefaultVideoStreamingPort;
extern const uint16_t kDefaultAudioStreamingPort;
extern const uint32_t kDefaultStopStreamingTimeout;
extern const uint16_t kDefaultTimeTestingPort;
extern const uint32_t kDefaultMaxCmdId;
extern const uint32_t kDefaultPutFileRequestInNone;
extern const uint32_t kDefaultDeleteFileRequestInNone;
extern const uint32_t kDefaultListFilesRequestInNone;
extern const uint32_t kDefaultTimeout;
extern const uint32_t kDefaultAppResumingTimeout;
extern const uint32_t kDefaultAppSavePersistentDataTimeout;
extern const uint32_t kDefaultResumptionDelayBeforeIgn;
extern const uint32_t kDefaultResumptionDelayAfterIgn;
extern const uint32_t kDefaultHashStringSize;
extern const uint32_t kDefaultListFilesResponseSize;

extern const uint32_t kDefaultDirQuota;
extern const uint32_t kDefaultAppTimeScaleMaxRequests;
extern const uint32_t kDefaultAppRequestsTimeScale;
extern const uint32_t kDefaultAppHmiLevelNoneTimeScaleMaxRequests;
extern const uint32_t kDefaultAppHmiLevelNoneRequestsTimeScale;
extern const uint32_t kDefaultPendingRequestsAmount;
extern const uint32_t kDefaultTransportManagerDisconnectTimeout;
extern const uint32_t kDefaultApplicationListUpdateTimeout;
extern const std::pair<uint32_t, uint32_t> kReadDIDFrequency;
extern const std::pair<uint32_t, uint32_t> kGetVehicleDataFrequency;
extern const std::pair<uint32_t, uint32_t> kStartStreamRetryAmount;
extern const uint32_t kDefaultMaxThreadPoolSize;
extern const int kDefaultIAP2HubConnectAttempts;
extern const int kDefaultIAPHubConnectionWaitTimeout;
extern const int kDefaultIAPArmEventTimeout;
extern const uint16_t kDefaultTTSGlobalPropertiesTimeout;
// TCP MTU - header size = 1500 - 12
extern const size_t kDefaultMaximumPayloadSize;
extern const size_t kDefaultFrequencyCount;
extern const size_t kDefaultFrequencyTime;
extern const bool kDefaulMalformedMessageFiltering;
extern const size_t kDefaultMalformedFrequencyCount;
extern const size_t kDefaultMalformedFrequencyTime;
extern const uint32_t kDefaultExpectedConsecutiveFramesTimeout;
extern const uint16_t kDefaultAttemptsToOpenPolicyDB;
extern const uint16_t kDefaultOpenAttemptTimeoutMs;
extern const uint32_t kDefaultAppIconsFolderMaxSize;
extern const uint32_t kDefaultAppIconsAmountToRemove;
extern const uint16_t kDefaultAttemptsToOpenResumptionDB;
extern const size_t kDefaultMaximumControlPayloadSize;
extern const size_t kDefaultMaximumRpcPayloadSize;
extern const size_t kDefaultMaximumAudioPayloadSize;
extern const size_t kDefaultMaximumVideoPayloadSize;
extern const uint16_t kDefaultOpenAttemptTimeoutMsResumptionDB;
extern const uint16_t kDefaultAppLaunchWaitTime;
extern const uint16_t kDefaultAppLaunchMaxRetryAttempt;
extern const uint16_t kDefaultAppLaunchRetryWaitTime;
extern const uint16_t kDefaultRemoveBundleIDattempts;
extern const uint16_t kDefaultMaxNumberOfiOSDevice;
extern const uint16_t kDefaultWaitTimeBetweenApps;
extern const bool kDefaultEnableAppLaunchIOS;
extern const uint32_t kDefaultAppTransportChangeTimer;
extern const uint32_t kDefaultAppTransportChangeTimerAddition;
extern const std::string kAllowedSymbols;
}  // namespace default_values
} // namespace profile
