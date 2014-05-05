/**
 * @file MOBILE_API.h
 * @brief Generated class MOBILE_API header file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from MOBILE_API.xml file
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __CSMARTFACTORY_MOBILE_API_8275382ecba511e3a201902b34794c53_H__
#define __CSMARTFACTORY_MOBILE_API_8275382ecba511e3a201902b34794c53_H__

namespace mobile_apis {
namespace StructIdentifiers {
/**
 * @brief Enumeration StructIdentifiers.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief AudioPassThruCapabilities.
   */
  AudioPassThruCapabilities,

  /**
   * @brief Image.
   */
  Image,

  /**
   * @brief SoftButton.
   */
  SoftButton,

  /**
   * @brief Choice.
   */
  Choice,

  /**
   * @brief VrHelpItem.
   */
  VrHelpItem,

  /**
   * @brief SyncMsgVersion.
   */
  SyncMsgVersion,

  /**
   * @brief SingleTireStatus.
   */
  SingleTireStatus,

  /**
   * @brief BeltStatus.
   */
  BeltStatus,

  /**
   * @brief BodyInformation.
   */
  BodyInformation,

  /**
   * @brief DeviceStatus.
   */
  DeviceStatus,

  /**
   * @brief HeadLampStatus.
   */
  HeadLampStatus,

  /**
   * @brief engineInfo.
   */
  engineInfo,

  /**
   * @brief DeviceInfo.
   */
  DeviceInfo,

  /**
   * @brief ECallInfo.
   */
  ECallInfo,

  /**
   * @brief AirbagStatus.
   */
  AirbagStatus,

  /**
   * @brief EmergencyEvent.
   */
  EmergencyEvent,

  /**
   * @brief ClusterModeStatus.
   */
  ClusterModeStatus,

  /**
   * @brief MyKey.
   */
  MyKey,

  /**
   * @brief TireStatus.
   */
  TireStatus,

  /**
   * @brief GPSData.
   */
  GPSData,

  /**
   * @brief VehicleDataResult.
   */
  VehicleDataResult,

  /**
   * @brief DIDResult.
   */
  DIDResult,

  /**
   * @brief StartTime.
   */
  StartTime,

  /**
   * @brief TextField.
   */
  TextField,

  /**
   * @brief ImageResolution.
   */
  ImageResolution,

  /**
   * @brief ImageField.
   */
  ImageField,

  /**
   * @brief TouchCoord.
   */
  TouchCoord,

  /**
   * @brief TouchEvent.
   */
  TouchEvent,

  /**
   * @brief Coordinate.
   */
  Coordinate,

  /**
   * @brief TouchArea.
   */
  TouchArea,

  /**
   * @brief TouchEventCapabilities.
   */
  TouchEventCapabilities,

  /**
   * @brief ScreenParams.
   */
  ScreenParams,

  /**
   * @brief HMIPermissions.
   */
  HMIPermissions,

  /**
   * @brief ParameterPermissions.
   */
  ParameterPermissions,

  /**
   * @brief PermissionItem.
   */
  PermissionItem,

  /**
   * @brief DisplayCapabilities.
   */
  DisplayCapabilities,

  /**
   * @brief ButtonCapabilities.
   */
  ButtonCapabilities,

  /**
   * @brief SoftButtonCapabilities.
   */
  SoftButtonCapabilities,

  /**
   * @brief PresetBankCapabilities.
   */
  PresetBankCapabilities,

  /**
   * @brief MenuParams.
   */
  MenuParams,

  /**
   * @brief TTSChunk.
   */
  TTSChunk,

  /**
   * @brief Turn.
   */
  Turn,

  /**
   * @brief VehicleType.
   */
  VehicleType,

  /**
   * @brief KeyboardProperties.
   */
  KeyboardProperties
};
} // StructIdentifiers

namespace Result {
/**
 * @brief Enumeration Result.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SUCCESS.
   *
   * The request succeeded
   */
  SUCCESS,

  /**
   * @brief UNSUPPORTED_REQUEST.
   *
   * The request is not supported by Sync
   */
  UNSUPPORTED_REQUEST,

  /**
   * @brief UNSUPPORTED_RESOURCE.
   *
   * 
   *       	A button that was requested for subscription is not supported under the current system.
   *       	NOTE: could become a more generic UNSUPPORTED_RESOURCE by merging with VEHIVLE_DATA_NOT_AVAILABLE.
   *       
   */
  UNSUPPORTED_RESOURCE,

  /**
   * @brief DISALLOWED.
   *
   * RPC is not authorized in local policy table.
   */
  DISALLOWED,

  /**
   * @brief REJECTED.
   *
   * 
   *       	The requested command was rejected, e.g. because mobile app is in background and cannot perform any HMI commands.
   *       	Or an HMI command (e.g. Speak) is rejected because a higher priority HMI command (e.g. Alert) is playing.
   *       
   */
  REJECTED,

  /**
   * @brief ABORTED.
   *
   * 
   *       	A command was aborted, for example due to user interaction (e.g. user pressed button).
   *       	Or an HMI command (e.g. Speak) is aborted because a higher priority HMI command (e.g. Alert) was requested.
   *       
   */
  ABORTED,

  /**
   * @brief IGNORED.
   *
   * 
   *       	A command was ignored, because the intended result is already in effect.
   *       	For example, SetMediaClockTimer was used to pause the media clock although the clock is paused already.
   *       	NOTE: potentially replaces SUBSCRIBED_ALREADY
   *       
   */
  IGNORED,

  /**
   * @brief RETRY.
   *
   * The user interrupted the RPC (e.g. PerformAudioPassThru) and indicated to start over.  Note, the app must issue the new RPC.
   */
  RETRY,

  /**
   * @brief IN_USE.
   *
   * 
   *       	The data may not be changed, because it is currently in use.
   *       	For example when trying to delete a command set that is currently involved in an interaction.
   *       
   */
  IN_USE,

  /**
   * @brief VEHICLE_DATA_NOT_AVAILABLE.
   *
   * The requested vehicle data is not available on this vehicle or is not published.
   */
  VEHICLE_DATA_NOT_AVAILABLE,

  /**
   * @brief TIMED_OUT.
   *
   * Overlay reached the maximum timeout and closed.
   */
  TIMED_OUT,

  /**
   * @brief INVALID_DATA.
   *
   * 
   *       	The data sent is invalid. For example:
   * 		Invalid Json syntax
   *       	Parameters out of bounds (number or enum range)
   *       	Mandatory parameters not provided
   *       	Parameter provided with wrong type
   *       	Invalid characters
   *       	Empty string
   *       
   */
  INVALID_DATA,

  /**
   * @brief CHAR_LIMIT_EXCEEDED.
   */
  CHAR_LIMIT_EXCEEDED,

  /**
   * @brief INVALID_ID.
   *
   * 
   *       	One of the provided IDs is not valid. For example
   *       	This applies to CorrelationID, SubscriptionID [@TODO if SubscriptionID is used], CommandID, MenuID, [@TODO: missed one?]
   *       
   */
  INVALID_ID,

  /**
   * @brief DUPLICATE_NAME.
   *
   * There was a conflict with an registered name (application or menu item) or vr command
   */
  DUPLICATE_NAME,

  /**
   * @brief APPLICATION_NOT_REGISTERED.
   *
   * An command can not be executed because no application has been registered with RegisterApplication.
   */
  APPLICATION_NOT_REGISTERED,

  /**
   * @brief WRONG_LANGUAGE.
   *
   * 
   *       	The requested language is currently not supported.
   *       	Might be because of a mismatch of the currently active language on Sync and the requested language
   *       
   */
  WRONG_LANGUAGE,

  /**
   * @brief OUT_OF_MEMORY.
   *
   * The system could not process the request because the necessary memory couldn't be allocated
   */
  OUT_OF_MEMORY,

  /**
   * @brief TOO_MANY_PENDING_REQUESTS.
   *
   * There are too many requests pending (means, that the response has not been delivered, yet).
   *
   * There may be a maximum of 1000 pending requests at a time.
   */
  TOO_MANY_PENDING_REQUESTS,

  /**
   * @brief TOO_MANY_APPLICATIONS.
   *
   * There are already too many registered applications
   */
  TOO_MANY_APPLICATIONS,

  /**
   * @brief APPLICATION_REGISTERED_ALREADY.
   *
   * RegisterApplication has been called again, after a RegisterApplication was successful before.
   */
  APPLICATION_REGISTERED_ALREADY,

  /**
   * @brief WARNINGS.
   *
   * The RPC (e.g. SubscribeVehicleData) executed successfully but one or more items have a warning or failure.
   */
  WARNINGS,

  /**
   * @brief GENERIC_ERROR.
   *
   * Provided data is valid but something went wrong in the lower layers.
   */
  GENERIC_ERROR,

  /**
   * @brief USER_DISALLOWED.
   *
   * RPC is included in a functional group explicitly blocked by the user.
   */
  USER_DISALLOWED,

  /**
   * @brief UNSUPPORTED_VERSION.
   *
   * Sync doesn't support the protocol that is requested by the mobile application
   */
  UNSUPPORTED_VERSION,

  /**
   * @brief VEHICLE_DATA_NOT_ALLOWED.
   *
   * The user has turned off access to vehicle data, and it is globally unavailable to mobile applications.
   */
  VEHICLE_DATA_NOT_ALLOWED,

  /**
   * @brief FILE_NOT_FOUND.
   *
   * A specified file could not be found on Sync.
   */
  FILE_NOT_FOUND,

  /**
   * @brief CANCEL_ROUTE.
   *
   * User selected to Cancel Route.
   */
  CANCEL_ROUTE,

  /**
   * @brief TRUNCATED_DATA.
   *
   * The RPC (e.g. ReadDID) executed successfully but the data exceeded the platform maximum threshold and thus, only part of the data is available.
   */
  TRUNCATED_DATA,

  /**
   * @brief SAVED.
   *
   * The RPC (e.g. Slider) executed successfully and the user elected to save the current position / value.
   */
  SAVED,

  /**
   * @brief INVALID_CERT.
   *
   * The certificate provided during authentication is invalid.
   */
  INVALID_CERT,

  /**
   * @brief EXPIRED_CERT.
   *
   * The certificate provided during authentication is expired.
   */
  EXPIRED_CERT,

  /**
   * @brief RESUME_FAILED.
   *
   * The provided hash ID does not match the hash of the current set of registered data or the core could not resume the previous data.
   */
  RESUME_FAILED
};
} // Result

namespace ButtonPressMode {
/**
 * @brief Enumeration ButtonPressMode.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief LONG.
   *
   * 
   *       	A button was released, after it was pressed for a long time
   *       	Actual timing is defined by Sync and may vary
   *       
   */
  LONG,

  /**
   * @brief SHORT.
   *
   * 
   *       	A button was released, after it was pressed for a short time
   *       	Actual timing is defined by Sync and may vary
   *       
   */
  SHORT
};
} // ButtonPressMode

namespace ButtonEventMode {
/**
 * @brief Enumeration ButtonEventMode.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief BUTTONUP.
   *
   * A button has been released up
   */
  BUTTONUP,

  /**
   * @brief BUTTONDOWN.
   *
   * A button has been pressed down
   */
  BUTTONDOWN
};
} // ButtonEventMode

namespace Language {
/**
 * @brief Enumeration Language.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief EN_US.
   *
   * English - US
   */
  EN_US,

  /**
   * @brief ES_MX.
   *
   * Spanish - Mexico
   */
  ES_MX,

  /**
   * @brief FR_CA.
   *
   * French - Canada
   */
  FR_CA,

  /**
   * @brief DE_DE.
   *
   * German - Germany
   */
  DE_DE,

  /**
   * @brief ES_ES.
   *
   * Spanish - Spain
   */
  ES_ES,

  /**
   * @brief EN_GB.
   *
   * English - GB
   */
  EN_GB,

  /**
   * @brief RU_RU.
   *
   * Russian - Russia
   */
  RU_RU,

  /**
   * @brief TR_TR.
   *
   * Turkish - Turkey
   */
  TR_TR,

  /**
   * @brief PL_PL.
   *
   * Polish - Poland
   */
  PL_PL,

  /**
   * @brief FR_FR.
   *
   * French - France
   */
  FR_FR,

  /**
   * @brief IT_IT.
   *
   * Italian - Italy
   */
  IT_IT,

  /**
   * @brief SV_SE.
   *
   * Swedish - Sweden
   */
  SV_SE,

  /**
   * @brief PT_PT.
   *
   * Portuguese - Portugal
   */
  PT_PT,

  /**
   * @brief NL_NL.
   *
   * Dutch (Standard) - Netherlands
   */
  NL_NL,

  /**
   * @brief EN_AU.
   *
   * English - Australia
   */
  EN_AU,

  /**
   * @brief ZH_CN.
   *
   * Mandarin - China
   */
  ZH_CN,

  /**
   * @brief ZH_TW.
   *
   * Mandarin - Taiwan
   */
  ZH_TW,

  /**
   * @brief JA_JP.
   *
   * Japanese - Japan
   */
  JA_JP,

  /**
   * @brief AR_SA.
   *
   * Arabic - Saudi Arabia
   */
  AR_SA,

  /**
   * @brief KO_KR.
   *
   * Korean - South Korea
   */
  KO_KR,

  /**
   * @brief PT_BR.
   *
   * Portuguese - Brazil
   */
  PT_BR,

  /**
   * @brief CS_CZ.
   *
   * Czech - Czech Republic
   */
  CS_CZ,

  /**
   * @brief DA_DK.
   *
   * Danish - Denmark
   */
  DA_DK,

  /**
   * @brief NO_NO.
   *
   * Norwegian - Norway
   */
  NO_NO
};
} // Language

namespace UpdateMode {
/**
 * @brief Enumeration UpdateMode.
 *
 * Describes how the media clock timer should behave on the platform
 * Starts the media clock timer counting upwards, as in time elapsed.
 * Starts the media clock timer counting downwards, as in time remaining.
 * Pauses the media clock timer
 * Resume the media clock timer
 * Clears the media clock timer (previously done through Show->mediaClock)
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief COUNTUP.
   */
  COUNTUP,

  /**
   * @brief COUNTDOWN.
   */
  COUNTDOWN,

  /**
   * @brief PAUSE.
   */
  PAUSE,

  /**
   * @brief RESUME.
   */
  RESUME,

  /**
   * @brief CLEAR.
   */
  CLEAR
};
} // UpdateMode

namespace TimerMode {
/**
 * @brief Enumeration TimerMode.
 *
 * Causes the media clock timer to update from 0:00 to a specified time
 * Causes the media clock timer to update from a specified time to 0:00
 * Indicates to not use the media clock timer
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief UP.
   */
  UP,

  /**
   * @brief DOWN.
   */
  DOWN,

  /**
   * @brief NONE.
   */
  NONE
};
} // TimerMode

namespace InteractionMode {
/**
 * @brief Enumeration InteractionMode.
 *
 * For application-requested interactions, this mode indicates the method in which the user is notified and uses the interaction.
 * 
 *     	This mode causes the interaction to only occur on the display, meaning the choices are provided only via the display.
 *     	Selections are made with the OK and Seek Right and Left, Tune Up and Down buttons.
 *     
 * 
 *     	This mode causes the interaction to only occur using V4.
 *     	Selections are made by saying the command.
 *     
 * 
 *     	This mode causes both a VR and display selection option for an interaction.
 *     	Selections can be made either from the menu display or by speaking the command.
 *     
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief MANUAL_ONLY.
   */
  MANUAL_ONLY,

  /**
   * @brief VR_ONLY.
   */
  VR_ONLY,

  /**
   * @brief BOTH.
   */
  BOTH
};
} // InteractionMode

namespace LayoutMode {
/**
 * @brief Enumeration LayoutMode.
 *
 * For touchscreen interactions, the mode of how the choices are presented.
 * 
 *     	This mode causes the interaction to display the previous set of choices as icons.
 *     
 * 
 *     	This mode causes the interaction to display the previous set of choices as icons along with a search field in the HMI.
 *     
 * 
 *     	This mode causes the interaction to display the previous set of choices as a list.
 *     
 * 
 *     	This mode causes the interaction to display the previous set of choices as a list along with a search field in the HMI.
 *     
 * 
 *     	This mode causes the interaction to immediately display a keyboard entry through the HMI.
 *     
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief ICON_ONLY.
   */
  ICON_ONLY,

  /**
   * @brief ICON_WITH_SEARCH.
   */
  ICON_WITH_SEARCH,

  /**
   * @brief LIST_ONLY.
   */
  LIST_ONLY,

  /**
   * @brief LIST_WITH_SEARCH.
   */
  LIST_WITH_SEARCH,

  /**
   * @brief KEYBOARD.
   */
  KEYBOARD
};
} // LayoutMode

namespace HMILevel {
/**
 * @brief Enumeration HMILevel.
 *
 * Enumeraction that describes current levels of HMI.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief HMI_FULL.
   */
  HMI_FULL,

  /**
   * @brief HMI_LIMITED.
   */
  HMI_LIMITED,

  /**
   * @brief HMI_BACKGROUND.
   */
  HMI_BACKGROUND,

  /**
   * @brief HMI_NONE.
   */
  HMI_NONE
};
} // HMILevel

namespace AudioStreamingState {
/**
 * @brief Enumeration AudioStreamingState.
 *
 * Enumeraction that describes possible states of audio streaming.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief AUDIBLE.
   */
  AUDIBLE,

  /**
   * @brief ATTENUATED.
   */
  ATTENUATED,

  /**
   * @brief NOT_AUDIBLE.
   */
  NOT_AUDIBLE
};
} // AudioStreamingState

namespace SystemAction {
/**
 * @brief Enumeration SystemAction.
 *
 * Enumeration that describes system actions that can be triggered.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief DEFAULT_ACTION.
   *
   * Default action occurs.  Standard behavior (e.g. SoftButton clears overlay).
   */
  DEFAULT_ACTION,

  /**
   * @brief STEAL_FOCUS.
   *
   * App is brought into HMI_FULL.
   */
  STEAL_FOCUS,

  /**
   * @brief KEEP_CONTEXT.
   *
   * Current system context is maintained.  An overlay is persisted even though a SoftButton has been pressed and the notification sent.
   */
  KEEP_CONTEXT
};
} // SystemAction

namespace SystemContext {
/**
 * @brief Enumeration SystemContext.
 *
 * Enumeration that describes possible contexts an app's HMI might be in.
 * Communicated to whichever app is in HMI FULL, except Alert.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SYSCTXT_MAIN.
   *
   * The app's persistent display (whether media/non-media/navigation) is fully visible onscreen.
   */
  SYSCTXT_MAIN,

  /**
   * @brief SYSCTXT_VRSESSION.
   *
   * The system is currently in a VR session (with whatever dedicated VR screen being overlaid onscreen).
   */
  SYSCTXT_VRSESSION,

  /**
   * @brief SYSCTXT_MENU.
   *
   * The system is currently displaying an in-App menu onscreen.
   */
  SYSCTXT_MENU,

  /**
   * @brief SYSCTXT_HMI_OBSCURED.
   *
   * The app's display HMI is currently being obscured by either a system or other app's overlay.
   */
  SYSCTXT_HMI_OBSCURED,

  /**
   * @brief SYSCTXT_ALERT.
   *
   * Broadcast only to whichever app has an alert currently being displayed.
   */
  SYSCTXT_ALERT
};
} // SystemContext

namespace SoftButtonType {
/**
 * @brief Enumeration SoftButtonType.
 *
 * Contains information about the SoftButton capabilities.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SBT_TEXT.
   */
  SBT_TEXT,

  /**
   * @brief SBT_IMAGE.
   */
  SBT_IMAGE,

  /**
   * @brief SBT_BOTH.
   */
  SBT_BOTH
};
} // SoftButtonType

namespace AppInterfaceUnregisteredReason {
/**
 * @brief Enumeration AppInterfaceUnregisteredReason.
 *
 * Error code, which comes from sync side.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief USER_EXIT.
   */
  USER_EXIT,

  /**
   * @brief IGNITION_OFF.
   */
  IGNITION_OFF,

  /**
   * @brief BLUETOOTH_OFF.
   */
  BLUETOOTH_OFF,

  /**
   * @brief USB_DISCONNECTED.
   */
  USB_DISCONNECTED,

  /**
   * @brief REQUEST_WHILE_IN_NONE_HMI_LEVEL.
   */
  REQUEST_WHILE_IN_NONE_HMI_LEVEL,

  /**
   * @brief TOO_MANY_REQUESTS.
   */
  TOO_MANY_REQUESTS,

  /**
   * @brief DRIVER_DISTRACTION_VIOLATION.
   */
  DRIVER_DISTRACTION_VIOLATION,

  /**
   * @brief LANGUAGE_CHANGE.
   */
  LANGUAGE_CHANGE,

  /**
   * @brief MASTER_RESET.
   */
  MASTER_RESET,

  /**
   * @brief FACTORY_DEFAULTS.
   */
  FACTORY_DEFAULTS,

  /**
   * @brief APP_UNAUTHORIZED.
   */
  APP_UNAUTHORIZED
};
} // AppInterfaceUnregisteredReason

namespace TriggerSource {
/**
 * @brief Enumeration TriggerSource.
 *
 * Indicates the source from where the command was triggered.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief TS_MENU.
   */
  TS_MENU,

  /**
   * @brief TS_VR.
   */
  TS_VR,

  /**
   * @brief TS_KEYBOARD.
   */
  TS_KEYBOARD
};
} // TriggerSource

namespace HmiZoneCapabilities {
/**
 * @brief Enumeration HmiZoneCapabilities.
 *
 * Contains information about the HMI zone capabilities.
 * For future use.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief FRONT.
   */
  FRONT,

  /**
   * @brief BACK.
   */
  BACK
};
} // HmiZoneCapabilities

namespace SpeechCapabilities {
/**
 * @brief Enumeration SpeechCapabilities.
 *
 * Contains information about the TTS capabilities.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SC_TEXT.
   */
  SC_TEXT,

  /**
   * @brief SAPI_PHONEMES.
   */
  SAPI_PHONEMES,

  /**
   * @brief LHPLUS_PHONEMES.
   */
  LHPLUS_PHONEMES,

  /**
   * @brief PRE_RECORDED.
   */
  PRE_RECORDED,

  /**
   * @brief SILENCE.
   */
  SILENCE
};
} // SpeechCapabilities

namespace VrCapabilities {
/**
 * @brief Enumeration VrCapabilities.
 *
 * Contains information about the VR capabilities.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VR_TEXT.
   */
  VR_TEXT
};
} // VrCapabilities

namespace PrerecordedSpeech {
/**
 * @brief Enumeration PrerecordedSpeech.
 *
 * Contains a list of prerecorded speech items present on the platform.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief HELP_JINGLE.
   */
  HELP_JINGLE,

  /**
   * @brief INITIAL_JINGLE.
   */
  INITIAL_JINGLE,

  /**
   * @brief LISTEN_JINGLE.
   */
  LISTEN_JINGLE,

  /**
   * @brief POSITIVE_JINGLE.
   */
  POSITIVE_JINGLE,

  /**
   * @brief NEGATIVE_JINGLE.
   */
  NEGATIVE_JINGLE
};
} // PrerecordedSpeech

namespace SamplingRate {
/**
 * @brief Enumeration SamplingRate.
 *
 * Describes different sampling options for PerformAudioPassThru.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SamplingRate_8KHZ.
   */
  SamplingRate_8KHZ,

  /**
   * @brief SamplingRate_16KHZ.
   */
  SamplingRate_16KHZ,

  /**
   * @brief SamplingRate_22KHZ.
   */
  SamplingRate_22KHZ,

  /**
   * @brief SamplingRate_44KHZ.
   */
  SamplingRate_44KHZ
};
} // SamplingRate

namespace BitsPerSample {
/**
 * @brief Enumeration BitsPerSample.
 *
 * Describes different quality options for PerformAudioPassThru.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief BitsPerSample_8_BIT.
   */
  BitsPerSample_8_BIT,

  /**
   * @brief BitsPerSample_16_BIT.
   */
  BitsPerSample_16_BIT
};
} // BitsPerSample

namespace AudioType {
/**
 * @brief Enumeration AudioType.
 *
 * Describes different audio type options for PerformAudioPassThru.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief PCM.
   */
  PCM
};
} // AudioType

namespace VehicleDataType {
/**
 * @brief Enumeration VehicleDataType.
 *
 * Defines the data types that can be published and subscribed to.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VEHICLEDATA_GPS.
   *
   * Notifies GPSData may be subscribed
   */
  VEHICLEDATA_GPS,

  /**
   * @brief VEHICLEDATA_SPEED.
   */
  VEHICLEDATA_SPEED,

  /**
   * @brief VEHICLEDATA_RPM.
   */
  VEHICLEDATA_RPM,

  /**
   * @brief VEHICLEDATA_FUELLEVEL.
   */
  VEHICLEDATA_FUELLEVEL,

  /**
   * @brief VEHICLEDATA_FUELLEVEL_STATE.
   */
  VEHICLEDATA_FUELLEVEL_STATE,

  /**
   * @brief VEHICLEDATA_FUELCONSUMPTION.
   */
  VEHICLEDATA_FUELCONSUMPTION,

  /**
   * @brief VEHICLEDATA_EXTERNTEMP.
   */
  VEHICLEDATA_EXTERNTEMP,

  /**
   * @brief VEHICLEDATA_VIN.
   */
  VEHICLEDATA_VIN,

  /**
   * @brief VEHICLEDATA_PRNDL.
   */
  VEHICLEDATA_PRNDL,

  /**
   * @brief VEHICLEDATA_TIREPRESSURE.
   */
  VEHICLEDATA_TIREPRESSURE,

  /**
   * @brief VEHICLEDATA_ODOMETER.
   */
  VEHICLEDATA_ODOMETER,

  /**
   * @brief VEHICLEDATA_BELTSTATUS.
   */
  VEHICLEDATA_BELTSTATUS,

  /**
   * @brief VEHICLEDATA_BODYINFO.
   */
  VEHICLEDATA_BODYINFO,

  /**
   * @brief VEHICLEDATA_DEVICESTATUS.
   */
  VEHICLEDATA_DEVICESTATUS,

  /**
   * @brief VEHICLEDATA_ECALLINFO.
   */
  VEHICLEDATA_ECALLINFO,

  /**
   * @brief VEHICLEDATA_AIRBAGSTATUS.
   */
  VEHICLEDATA_AIRBAGSTATUS,

  /**
   * @brief VEHICLEDATA_EMERGENCYEVENT.
   */
  VEHICLEDATA_EMERGENCYEVENT,

  /**
   * @brief VEHICLEDATA_CLUSTERMODESTATUS.
   */
  VEHICLEDATA_CLUSTERMODESTATUS,

  /**
   * @brief VEHICLEDATA_MYKEY.
   */
  VEHICLEDATA_MYKEY,

  /**
   * @brief VEHICLEDATA_BRAKING.
   */
  VEHICLEDATA_BRAKING,

  /**
   * @brief VEHICLEDATA_WIPERSTATUS.
   */
  VEHICLEDATA_WIPERSTATUS,

  /**
   * @brief VEHICLEDATA_HEADLAMPSTATUS.
   */
  VEHICLEDATA_HEADLAMPSTATUS,

  /**
   * @brief VEHICLEDATA_BATTVOLTAGE.
   */
  VEHICLEDATA_BATTVOLTAGE,

  /**
   * @brief VEHICLEDATA_ENGINETORQUE.
   */
  VEHICLEDATA_ENGINETORQUE,

  /**
   * @brief VEHICLEDATA_ACCPEDAL.
   */
  VEHICLEDATA_ACCPEDAL,

  /**
   * @brief VEHICLEDATA_STEERINGWHEEL.
   */
  VEHICLEDATA_STEERINGWHEEL
};
} // VehicleDataType

namespace ButtonName {
/**
 * @brief Enumeration ButtonName.
 *
 * Defines the hard (physical) and soft (touchscreen) buttons available from SYNC
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief OK.
   */
  OK,

  /**
   * @brief SEEKLEFT.
   */
  SEEKLEFT,

  /**
   * @brief SEEKRIGHT.
   */
  SEEKRIGHT,

  /**
   * @brief TUNEUP.
   */
  TUNEUP,

  /**
   * @brief TUNEDOWN.
   */
  TUNEDOWN,

  /**
   * @brief PRESET_0.
   */
  PRESET_0,

  /**
   * @brief PRESET_1.
   */
  PRESET_1,

  /**
   * @brief PRESET_2.
   */
  PRESET_2,

  /**
   * @brief PRESET_3.
   */
  PRESET_3,

  /**
   * @brief PRESET_4.
   */
  PRESET_4,

  /**
   * @brief PRESET_5.
   */
  PRESET_5,

  /**
   * @brief PRESET_6.
   */
  PRESET_6,

  /**
   * @brief PRESET_7.
   */
  PRESET_7,

  /**
   * @brief PRESET_8.
   */
  PRESET_8,

  /**
   * @brief PRESET_9.
   */
  PRESET_9,

  /**
   * @brief CUSTOM_BUTTON.
   */
  CUSTOM_BUTTON,

  /**
   * @brief SEARCH.
   */
  SEARCH
};
} // ButtonName

namespace MediaClockFormat {
/**
 * @brief Enumeration MediaClockFormat.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief CLOCK1.
   *
   * 
   *       	minutesFieldWidth = 2;minutesFieldMax = 19;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 19;maxMinutes = 59;maxSeconds = 59;
   *       	used for Type II and CID headunits
   *       
   */
  CLOCK1,

  /**
   * @brief CLOCK2.
   *
   * 
   *       	minutesFieldWidth = 3;minutesFieldMax = 199;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 59;maxMinutes = 59;maxSeconds = 59;
   * 	  	used for Type V headunit
   * 	  
   */
  CLOCK2,

  /**
   * @brief CLOCK3.
   *
   * 
   *       	minutesFieldWidth = 2;minutesFieldMax = 59;secondsFieldWidth = 2;secondsFieldMax = 59;maxHours = 9;maxMinutes = 59;maxSeconds = 59;
   * 	  	used for GEN1.1 MFD3/4/5 headunits
   * 	  
   */
  CLOCK3,

  /**
   * @brief CLOCKTEXT1.
   *
   * 
   *       	5 characters possible
   *       	Format:      1|sp   c   :|sp   c   c
   *       	1|sp : digit "1" or space
   *       	c    : character out of following character set: sp|0-9|[letters, see TypeII column in XLS. See [@TODO: create file ref]]
   *       	:|sp : colon or space
   * 	  	used for Type II headunit
   * 	  
   */
  CLOCKTEXT1,

  /**
   * @brief CLOCKTEXT2.
   *
   * 
   *       	5 chars possible
   *       	Format:      1|sp   c   :|sp   c   c
   *       	1|sp : digit "1" or space
   *       	c    : character out of following character set: sp|0-9|[letters, see CID column in XLS. See [@TODO: create file ref]]
   *       	:|sp : colon or space
   * 	  	used for CID headunit
   * 	  	NOTE: difference between CLOCKTEXT1 and CLOCKTEXT2 is the supported character set
   * 	  
   */
  CLOCKTEXT2,

  /**
   * @brief CLOCKTEXT3.
   *
   * 
   *       	6 chars possible
   *       	Format:      1|sp   c   c   :|sp   c   c
   *       	1|sp : digit "1" or space
   *       	c    : character out of following character set: sp|0-9|[letters, see Type 5 column in XLS]. See [@TODO: create file ref]
   *       	:|sp : colon or space
   * 	  	used for Type V headunit
   * 	  
   */
  CLOCKTEXT3,

  /**
   * @brief CLOCKTEXT4.
   *
   * 
   *       	6 chars possible
   *       	Format:      c   :|sp   c   c   :   c   c
   *       	:|sp : colon or space
   *       	c    : character out of following character set: sp|0-9|[letters].
   * 	  	used for GEN1.1 MFD3/4/5 headunits
   * 	  
   */
  CLOCKTEXT4
};
} // MediaClockFormat

namespace DisplayType {
/**
 * @brief Enumeration DisplayType.
 *
 * See DAES for further infos regarding the displays
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief CID.
   */
  CID,

  /**
   * @brief TYPE2.
   */
  TYPE2,

  /**
   * @brief TYPE5.
   */
  TYPE5,

  /**
   * @brief NGN.
   */
  NGN,

  /**
   * @brief GEN2_8_DMA.
   */
  GEN2_8_DMA,

  /**
   * @brief GEN2_6_DMA.
   */
  GEN2_6_DMA,

  /**
   * @brief MFD3.
   */
  MFD3,

  /**
   * @brief MFD4.
   */
  MFD4,

  /**
   * @brief MFD5.
   */
  MFD5,

  /**
   * @brief GEN3_8_INCH.
   */
  GEN3_8_INCH
};
} // DisplayType

namespace TextFieldName {
/**
 * @brief Enumeration TextFieldName.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief mainField1.
   *
   * The first line of first set of main fields of the persistent display; applies to "Show"
   */
  mainField1,

  /**
   * @brief mainField2.
   *
   * The second line of first set of main fields of the persistent display; applies to "Show"
   */
  mainField2,

  /**
   * @brief mainField3.
   *
   * The first line of second set of main fields of persistent display; applies to "Show"
   */
  mainField3,

  /**
   * @brief mainField4.
   *
   * The second line of second set of main fields of the persistent display; applies to "Show"
   */
  mainField4,

  /**
   * @brief statusBar.
   *
   * The status bar on NGN; applies to "Show"
   */
  statusBar,

  /**
   * @brief mediaClock.
   *
   * Text value for MediaClock field; applies to "Show"
   */
  mediaClock,

  /**
   * @brief mediaTrack.
   *
   * The track field of NGN and GEN1.1 MFD displays. This field is only available for media applications; applies to "Show"
   */
  mediaTrack,

  /**
   * @brief alertText1.
   *
   * The first line of the alert text field; applies to "Alert"
   */
  alertText1,

  /**
   * @brief alertText2.
   *
   * The second line of the alert text field; applies to "Alert"
   */
  alertText2,

  /**
   * @brief alertText3.
   *
   * The third line of the alert text field; applies to "Alert"
   */
  alertText3,

  /**
   * @brief scrollableMessageBody.
   *
   * Long form body of text that can include newlines and tabs; applies to "ScrollableMessage"
   */
  scrollableMessageBody,

  /**
   * @brief initialInteractionText.
   *
   *  First line suggestion for a user response (in the case of VR enabled interaction)
   */
  initialInteractionText,

  /**
   * @brief navigationText1.
   *
   *  First line of navigation text
   */
  navigationText1,

  /**
   * @brief navigationText2.
   *
   *  Second line of navigation text
   */
  navigationText2,

  /**
   * @brief ETA.
   *
   *  Estimated Time of Arrival time for navigation
   */
  ETA,

  /**
   * @brief totalDistance.
   *
   *  Total distance to destination for navigation
   */
  totalDistance,

  /**
   * @brief audioPassThruDisplayText1.
   *
   *  First line of text for audio pass thru
   */
  audioPassThruDisplayText1,

  /**
   * @brief audioPassThruDisplayText2.
   *
   *  Second line of text for audio pass thru
   */
  audioPassThruDisplayText2,

  /**
   * @brief sliderHeader.
   *
   *  Header text for slider
   */
  sliderHeader,

  /**
   * @brief sliderFooter.
   *
   *  Footer text for slider
   */
  sliderFooter,

  /**
   * @brief notificationText.
   *
   *  Footer text for slider
   */
  notificationText,

  /**
   * @brief menuName.
   *
   *  Primary text for Choice
   */
  menuName,

  /**
   * @brief secondaryText.
   *
   *  Secondary text for Choice
   */
  secondaryText,

  /**
   * @brief tertiaryText.
   *
   *  Tertiary text for Choice
   */
  tertiaryText,

  /**
   * @brief timeToDestination.
   *
   *  Time to destination
   */
  timeToDestination,

  /**
   * @brief turnText.
   *
   *  Turn text
   */
  turnText,

  /**
   * @brief menuTitle.
   *
   *  Optional text to label an app menu button (for certain touchscreen platforms).
   */
  menuTitle
};
} // TextFieldName

namespace ImageFieldName {
/**
 * @brief Enumeration ImageFieldName.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief softButtonImage.
   *
   * The image field for SoftButton
   */
  softButtonImage,

  /**
   * @brief choiceImage.
   *
   * The first image field for Choice
   */
  choiceImage,

  /**
   * @brief choiceSecondaryImage.
   *
   * The secondary image field for Choice
   */
  choiceSecondaryImage,

  /**
   * @brief vrHelpItem.
   *
   * The image field for vrHelpItem
   */
  vrHelpItem,

  /**
   * @brief turnIcon.
   *
   * The image field for Turn
   */
  turnIcon,

  /**
   * @brief menuIcon.
   *
   * The image field for the menu icon in SetGlobalProperties
   */
  menuIcon,

  /**
   * @brief cmdIcon.
   *
   * The image field for AddCommand
   */
  cmdIcon,

  /**
   * @brief appIcon.
   *
   * The image field for the app icon (set by setAppIcon)
   */
  appIcon,

  /**
   * @brief graphic.
   *
   * The image field for Show
   */
  graphic,

  /**
   * @brief showConstantTBTIcon.
   *
   * The primary image field for ShowConstantTBT
   */
  showConstantTBTIcon,

  /**
   * @brief showConstantTBTNextTurnIcon.
   *
   * The secondary image field for ShowConstantTBT
   */
  showConstantTBTNextTurnIcon
};
} // ImageFieldName

namespace PredefinedLayout {
/**
 * @brief Enumeration PredefinedLayout.
 *
 * Predefined screen layout.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief DEFAULT.
   *
   * 
   *                 Default media / non-media screen.
   *                 Can be set as a root screen.
   *         
   */
  DEFAULT,

  /**
   * @brief MEDIA.
   *
   * 
   *                 Default Media screen.
   *                 Can be set as a root screen.
   *         
   */
  MEDIA,

  /**
   * @brief NON_MEDIA.
   *
   * 
   *                 Default Non-media screen.
   *                 Can be set as a root screen.
   *         
   */
  NON_MEDIA,

  /**
   * @brief ONSCREEN_PRESETS.
   *
   * 
   *                 Custom root media screen containing app-defined onscreen presets.
   *                 Can be set as a root screen.
   *         
   */
  ONSCREEN_PRESETS,

  /**
   * @brief NAV_FULLSCREEN_MAP.
   *
   * 
   *                 Custom root template screen containing full screen map with navigation controls.
   *                 Can be set as a root screen.
   *         
   */
  NAV_FULLSCREEN_MAP,

  /**
   * @brief NAV_LIST.
   *
   * 
   *                 Custom root template screen containing video represented list.
   *                 Can be set as a root screen.
   *         
   */
  NAV_LIST,

  /**
   * @brief NAV_KEYBOARD.
   *
   * 
   *                 Custom root template screen containing video represented keyboard.
   *                 Can be set as a root screen.
   *         
   */
  NAV_KEYBOARD,

  /**
   * @brief GRAPHIC_WITH_TEXT.
   *
   * 
   *                 Custom root template screen containing half-screen graphic with lines of text.
   *                 Can be set as a root screen.
   *         
   */
  GRAPHIC_WITH_TEXT,

  /**
   * @brief TEXT_WITH_GRAPHIC.
   *
   * 
   *                 Custom root template screen containing lines of text with half-screen graphic.
   *                 Can be set as a root screen.
   *         
   */
  TEXT_WITH_GRAPHIC,

  /**
   * @brief TILES_ONLY.
   *
   * 
   *                 Custom root template screen containing only tiled SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  TILES_ONLY,

  /**
   * @brief TEXTBUTTONS_ONLY.
   *
   * 
   *                 Custom root template screen containing only text SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  TEXTBUTTONS_ONLY,

  /**
   * @brief GRAPHIC_WITH_TILES.
   *
   * 
   *                 Custom root template screen containing half-screen graphic with tiled SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  GRAPHIC_WITH_TILES,

  /**
   * @brief TILES_WITH_GRAPHIC.
   *
   * 
   *                 Custom root template screen containing tiled SoftButtons with half-screen graphic.
   *                 Can be set as a root screen.
   *         
   */
  TILES_WITH_GRAPHIC,

  /**
   * @brief GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS.
   *
   * 
   *                 Custom root template screen containing half-screen graphic with text and SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  GRAPHIC_WITH_TEXT_AND_SOFTBUTTONS,

  /**
   * @brief TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC.
   *
   * 
   *                 Custom root template screen containing text and SoftButtons with half-screen graphic.
   *                 Can be set as a root screen.
   *         
   */
  TEXT_AND_SOFTBUTTONS_WITH_GRAPHIC,

  /**
   * @brief GRAPHIC_WITH_TEXTBUTTONS.
   *
   * 
   *                 Custom root template screen containing half-screen graphic with text only SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  GRAPHIC_WITH_TEXTBUTTONS,

  /**
   * @brief TEXTBUTTONS_WITH_GRAPHIC.
   *
   * 
   *                 Custom root template screen containing text only SoftButtons with half-screen graphic.
   *                 Can be set as a root screen.
   *         
   */
  TEXTBUTTONS_WITH_GRAPHIC,

  /**
   * @brief LARGE_GRAPHIC_WITH_SOFTBUTTONS.
   *
   * 
   *                 Custom root template screen containing a large graphic and SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  LARGE_GRAPHIC_WITH_SOFTBUTTONS,

  /**
   * @brief DOUBLE_GRAPHIC_WITH_SOFTBUTTONS.
   *
   * 
   *                 Custom root template screen containing two graphics and SoftButtons.
   *                 Can be set as a root screen.
   *         
   */
  DOUBLE_GRAPHIC_WITH_SOFTBUTTONS,

  /**
   * @brief LARGE_GRAPHIC_ONLY.
   *
   * 
   *                 Custom root template screen containing only a large graphic.
   *                 Can be set as a root screen.
   *         
   */
  LARGE_GRAPHIC_ONLY
};
} // PredefinedLayout

namespace CharacterSet {
/**
 * @brief Enumeration CharacterSet.
 *
 * The list of potential character sets
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief TYPE2SET.
   *
   * See [@TODO: create file ref]
   */
  TYPE2SET,

  /**
   * @brief TYPE5SET.
   *
   * See [@TODO: create file ref]
   */
  TYPE5SET,

  /**
   * @brief CID1SET.
   *
   * See [@TODO: create file ref]
   */
  CID1SET,

  /**
   * @brief CID2SET.
   *
   * See [@TODO: create file ref]
   */
  CID2SET
};
} // CharacterSet

namespace TextAlignment {
/**
 * @brief Enumeration TextAlignment.
 *
 * The list of possible alignments, left, right, or centered
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief LEFT_ALIGNED.
   */
  LEFT_ALIGNED,

  /**
   * @brief RIGHT_ALIGNED.
   */
  RIGHT_ALIGNED,

  /**
   * @brief CENTERED.
   */
  CENTERED
};
} // TextAlignment

namespace TBTState {
/**
 * @brief Enumeration TBTState.
 *
 * Enumeration that describes possible states of turn-by-turn client or AppLink app.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief ROUTE_UPDATE_REQUEST.
   */
  ROUTE_UPDATE_REQUEST,

  /**
   * @brief ROUTE_ACCEPTED.
   */
  ROUTE_ACCEPTED,

  /**
   * @brief ROUTE_REFUSED.
   */
  ROUTE_REFUSED,

  /**
   * @brief ROUTE_CANCELLED.
   */
  ROUTE_CANCELLED,

  /**
   * @brief ETA_REQUEST.
   */
  ETA_REQUEST,

  /**
   * @brief NEXT_TURN_REQUEST.
   */
  NEXT_TURN_REQUEST,

  /**
   * @brief ROUTE_STATUS_REQUEST.
   */
  ROUTE_STATUS_REQUEST,

  /**
   * @brief ROUTE_SUMMARY_REQUEST.
   */
  ROUTE_SUMMARY_REQUEST,

  /**
   * @brief TRIP_STATUS_REQUEST.
   */
  TRIP_STATUS_REQUEST,

  /**
   * @brief ROUTE_UPDATE_REQUEST_TIMEOUT.
   */
  ROUTE_UPDATE_REQUEST_TIMEOUT
};
} // TBTState

namespace DriverDistractionState {
/**
 * @brief Enumeration DriverDistractionState.
 *
 * Enumeration that describes possible states of driver distraction.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief DD_ON.
   */
  DD_ON,

  /**
   * @brief DD_OFF.
   */
  DD_OFF
};
} // DriverDistractionState

namespace ImageType {
/**
 * @brief Enumeration ImageType.
 *
 * Contains information about the type of image.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief STATIC.
   */
  STATIC,

  /**
   * @brief DYNAMIC.
   */
  DYNAMIC
};
} // ImageType

namespace GlobalProperty {
/**
 * @brief Enumeration GlobalProperty.
 *
 * The different global properties.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief HELPPROMPT.
   *
   * The property helpPrompt of setGlobalProperties
   */
  HELPPROMPT,

  /**
   * @brief TIMEOUTPROMPT.
   *
   * The property timeoutPrompt of setGlobalProperties
   */
  TIMEOUTPROMPT,

  /**
   * @brief VRHELPTITLE.
   *
   * The property vrHelpTitle of setGlobalProperties
   */
  VRHELPTITLE,

  /**
   * @brief VRHELPITEMS.
   *
   * The property array of vrHelp of setGlobalProperties
   */
  VRHELPITEMS,

  /**
   * @brief MENUNAME.
   *
   * The property in-app menu name of setGlobalProperties
   */
  MENUNAME,

  /**
   * @brief MENUICON.
   *
   * The property in-app menu icon of setGlobalProperties
   */
  MENUICON,

  /**
   * @brief KEYBOARDPROPERTIES.
   *
   * The on-screen keyboard configuration of setGlobalProperties
   */
  KEYBOARDPROPERTIES
};
} // GlobalProperty

namespace CompassDirection {
/**
 * @brief Enumeration CompassDirection.
 *
 * The list of potential compass directions
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief NORTH.
   */
  NORTH,

  /**
   * @brief NORTHWEST.
   */
  NORTHWEST,

  /**
   * @brief WEST.
   */
  WEST,

  /**
   * @brief SOUTHWEST.
   */
  SOUTHWEST,

  /**
   * @brief SOUTH.
   */
  SOUTH,

  /**
   * @brief SOUTHEAST.
   */
  SOUTHEAST,

  /**
   * @brief EAST.
   */
  EAST,

  /**
   * @brief NORTHEAST.
   */
  NORTHEAST
};
} // CompassDirection

namespace Dimension {
/**
 * @brief Enumeration Dimension.
 *
 * The supported dimensions of the GPS
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief Dimension_NO_FIX.
   *
   * No GPS at all
   */
  Dimension_NO_FIX,

  /**
   * @brief Dimension_2D.
   *
   * Longitude and lattitude
   */
  Dimension_2D,

  /**
   * @brief Dimension_3D.
   *
   * Longitude and lattitude and altitude
   */
  Dimension_3D
};
} // Dimension

namespace PRNDL {
/**
 * @brief Enumeration PRNDL.
 *
 * The selected gear.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief PARK.
   *
   * Parking
   */
  PARK,

  /**
   * @brief REVERSE.
   *
   * Reverse gear
   */
  REVERSE,

  /**
   * @brief NEUTRAL.
   *
   * No gear
   */
  NEUTRAL,

  /**
   * @brief DRIVE.
   */
  DRIVE,

  /**
   * @brief SPORT.
   *
   * Drive Sport mode
   */
  SPORT,

  /**
   * @brief LOWGEAR.
   *
   * 1st gear hold
   */
  LOWGEAR,

  /**
   * @brief FIRST.
   */
  FIRST,

  /**
   * @brief SECOND.
   */
  SECOND,

  /**
   * @brief THIRD.
   */
  THIRD,

  /**
   * @brief FOURTH.
   */
  FOURTH,

  /**
   * @brief FIFTH.
   */
  FIFTH,

  /**
   * @brief SIXTH.
   */
  SIXTH,

  /**
   * @brief SEVENTH.
   */
  SEVENTH,

  /**
   * @brief EIGHTH.
   */
  EIGHTH,

  /**
   * @brief UNKNOWN.
   */
  UNKNOWN,

  /**
   * @brief FAULT.
   */
  FAULT
};
} // PRNDL

namespace ComponentVolumeStatus {
/**
 * @brief Enumeration ComponentVolumeStatus.
 *
 * The volume status of a vehicle component.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief CVS_UNKNOWN.
   */
  CVS_UNKNOWN,

  /**
   * @brief CVS_NORMAL.
   */
  CVS_NORMAL,

  /**
   * @brief CVS_LOW.
   */
  CVS_LOW,

  /**
   * @brief CVS_FAULT.
   */
  CVS_FAULT,

  /**
   * @brief CVS_ALERT.
   */
  CVS_ALERT,

  /**
   * @brief CVS_NOT_SUPPORTED.
   */
  CVS_NOT_SUPPORTED
};
} // ComponentVolumeStatus

namespace WarningLightStatus {
/**
 * @brief Enumeration WarningLightStatus.
 *
 * Reflects the status of a cluster instrument warning light.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief WLS_OFF.
   */
  WLS_OFF,

  /**
   * @brief WLS_ON.
   */
  WLS_ON,

  /**
   * @brief WLS_FLASH.
   */
  WLS_FLASH,

  /**
   * @brief WLS_NOT_USED.
   */
  WLS_NOT_USED
};
} // WarningLightStatus

namespace VehicleDataNotificationStatus {
/**
 * @brief Enumeration VehicleDataNotificationStatus.
 *
 * Reflects the status of a vehicle data notification.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VDNS_NOT_SUPPORTED.
   */
  VDNS_NOT_SUPPORTED,

  /**
   * @brief VDNS_NORMAL.
   */
  VDNS_NORMAL,

  /**
   * @brief VDNS_ACTIVE.
   */
  VDNS_ACTIVE,

  /**
   * @brief VDNS_NOT_USED.
   */
  VDNS_NOT_USED
};
} // VehicleDataNotificationStatus

namespace IgnitionStableStatus {
/**
 * @brief Enumeration IgnitionStableStatus.
 *
 * Reflects the ignition switch stability.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief IGNITION_SWITCH_NOT_STABLE.
   */
  IGNITION_SWITCH_NOT_STABLE,

  /**
   * @brief IGNITION_SWITCH_STABLE.
   */
  IGNITION_SWITCH_STABLE,

  /**
   * @brief MISSING_FROM_TRANSMITTER.
   */
  MISSING_FROM_TRANSMITTER
};
} // IgnitionStableStatus

namespace IgnitionStatus {
/**
 * @brief Enumeration IgnitionStatus.
 *
 * Reflects the status of ignition.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief IS_UNKNOWN.
   */
  IS_UNKNOWN,

  /**
   * @brief IS_OFF.
   */
  IS_OFF,

  /**
   * @brief IS_ACCESSORY.
   */
  IS_ACCESSORY,

  /**
   * @brief IS_RUN.
   */
  IS_RUN,

  /**
   * @brief IS_START.
   */
  IS_START,

  /**
   * @brief IS_INVALID.
   */
  IS_INVALID
};
} // IgnitionStatus

namespace VehicleDataEventStatus {
/**
 * @brief Enumeration VehicleDataEventStatus.
 *
 * Reflects the status of a vehicle data event; e.g. a seat belt event status.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VDES_NO_EVENT.
   */
  VDES_NO_EVENT,

  /**
   * @brief VDES_NO.
   */
  VDES_NO,

  /**
   * @brief VDES_YES.
   */
  VDES_YES,

  /**
   * @brief VDES_NOT_SUPPORTED.
   */
  VDES_NOT_SUPPORTED,

  /**
   * @brief VDES_FAULT.
   */
  VDES_FAULT
};
} // VehicleDataEventStatus

namespace DeviceLevelStatus {
/**
 * @brief Enumeration DeviceLevelStatus.
 *
 * Reflects the reported battery status of the connected device, if reported.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief ZERO_LEVEL_BARS.
   */
  ZERO_LEVEL_BARS,

  /**
   * @brief ONE_LEVEL_BARS.
   */
  ONE_LEVEL_BARS,

  /**
   * @brief TWO_LEVEL_BARS.
   */
  TWO_LEVEL_BARS,

  /**
   * @brief THREE_LEVEL_BARS.
   */
  THREE_LEVEL_BARS,

  /**
   * @brief FOUR_LEVEL_BARS.
   */
  FOUR_LEVEL_BARS,

  /**
   * @brief NOT_PROVIDED.
   */
  NOT_PROVIDED
};
} // DeviceLevelStatus

namespace PrimaryAudioSource {
/**
 * @brief Enumeration PrimaryAudioSource.
 *
 * Reflects the current primary audio source (if selected).
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief NO_SOURCE_SELECTED.
   */
  NO_SOURCE_SELECTED,

  /**
   * @brief USB.
   */
  USB,

  /**
   * @brief USB2.
   */
  USB2,

  /**
   * @brief BLUETOOTH_STEREO_BTST.
   */
  BLUETOOTH_STEREO_BTST,

  /**
   * @brief LINE_IN.
   */
  LINE_IN,

  /**
   * @brief IPOD.
   */
  IPOD,

  /**
   * @brief MOBILE_APP.
   */
  MOBILE_APP
};
} // PrimaryAudioSource

namespace WiperStatus {
/**
 * @brief Enumeration WiperStatus.
 *
 * Reflects the status of the wipers.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief OFF.
   */
  OFF,

  /**
   * @brief AUTO_OFF.
   */
  AUTO_OFF,

  /**
   * @brief OFF_MOVING.
   */
  OFF_MOVING,

  /**
   * @brief MAN_INT_OFF.
   */
  MAN_INT_OFF,

  /**
   * @brief MAN_INT_ON.
   */
  MAN_INT_ON,

  /**
   * @brief MAN_LOW.
   */
  MAN_LOW,

  /**
   * @brief MAN_HIGH.
   */
  MAN_HIGH,

  /**
   * @brief MAN_FLICK.
   */
  MAN_FLICK,

  /**
   * @brief WASH.
   */
  WASH,

  /**
   * @brief AUTO_LOW.
   */
  AUTO_LOW,

  /**
   * @brief AUTO_HIGH.
   */
  AUTO_HIGH,

  /**
   * @brief COURTESYWIPE.
   */
  COURTESYWIPE,

  /**
   * @brief AUTO_ADJUST.
   */
  AUTO_ADJUST,

  /**
   * @brief STALLED.
   */
  STALLED,

  /**
   * @brief NO_DATA_EXISTS.
   */
  NO_DATA_EXISTS
};
} // WiperStatus

namespace VehicleDataStatus {
/**
 * @brief Enumeration VehicleDataStatus.
 *
 * Reflects the status of a binary vehicle data item.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VDS_NO_DATA_EXISTS.
   */
  VDS_NO_DATA_EXISTS,

  /**
   * @brief VDS_OFF.
   */
  VDS_OFF,

  /**
   * @brief VDS_ON.
   */
  VDS_ON
};
} // VehicleDataStatus

namespace MaintenanceModeStatus {
/**
 * @brief Enumeration MaintenanceModeStatus.
 *
 * Reflects the status of a vehicle maintenance mode.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief MMS_NORMAL.
   */
  MMS_NORMAL,

  /**
   * @brief MMS_NEAR.
   */
  MMS_NEAR,

  /**
   * @brief MMS_ACTIVE.
   */
  MMS_ACTIVE,

  /**
   * @brief MMS_FEATURE_NOT_PRESENT.
   */
  MMS_FEATURE_NOT_PRESENT
};
} // MaintenanceModeStatus

namespace VehicleDataActiveStatus {
/**
 * @brief Enumeration VehicleDataActiveStatus.
 *
 * Reflects the status of given vehicle component.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VDAS_INACTIVE_NOT_CONFIRMED.
   */
  VDAS_INACTIVE_NOT_CONFIRMED,

  /**
   * @brief VDAS_INACTIVE_CONFIRMED.
   */
  VDAS_INACTIVE_CONFIRMED,

  /**
   * @brief VDAS_ACTIVE_NOT_CONFIRMED.
   */
  VDAS_ACTIVE_NOT_CONFIRMED,

  /**
   * @brief VDAS_ACTIVE_CONFIRMED.
   */
  VDAS_ACTIVE_CONFIRMED,

  /**
   * @brief VDAS_FAULT.
   */
  VDAS_FAULT
};
} // VehicleDataActiveStatus

namespace AmbientLightStatus {
/**
 * @brief Enumeration AmbientLightStatus.
 *
 * Reflects the status of the ambient light sensor.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief NIGHT.
   */
  NIGHT,

  /**
   * @brief TWILIGHT_1.
   */
  TWILIGHT_1,

  /**
   * @brief TWILIGHT_2.
   */
  TWILIGHT_2,

  /**
   * @brief TWILIGHT_3.
   */
  TWILIGHT_3,

  /**
   * @brief TWILIGHT_4.
   */
  TWILIGHT_4,

  /**
   * @brief DAY.
   */
  DAY,

  /**
   * @brief UNKNOWN.
   */
  UNKNOWN,

  /**
   * @brief INVALID.
   */
  INVALID
};
} // AmbientLightStatus

namespace FuelCutoffStatus {
/**
 * @brief Enumeration FuelCutoffStatus.
 *
 * Reflects the status of the RCM fuel cutoff.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief FCS_TERMINATE_FUEL.
   */
  FCS_TERMINATE_FUEL,

  /**
   * @brief FCS_NORMAL_OPERATION.
   */
  FCS_NORMAL_OPERATION,

  /**
   * @brief FCS_FAULT.
   */
  FCS_FAULT
};
} // FuelCutoffStatus

namespace EmergencyEventType {
/**
 * @brief Enumeration EmergencyEventType.
 *
 * Reflects the emergency event status of the vehicle.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief EET_NO_EVENT.
   */
  EET_NO_EVENT,

  /**
   * @brief EET_FRONTAL.
   */
  EET_FRONTAL,

  /**
   * @brief EET_SIDE.
   */
  EET_SIDE,

  /**
   * @brief EET_REAR.
   */
  EET_REAR,

  /**
   * @brief EET_ROLLOVER.
   */
  EET_ROLLOVER,

  /**
   * @brief EET_NOT_SUPPORTED.
   */
  EET_NOT_SUPPORTED,

  /**
   * @brief EET_FAULT.
   */
  EET_FAULT
};
} // EmergencyEventType

namespace ECallConfirmationStatus {
/**
 * @brief Enumeration ECallConfirmationStatus.
 *
 * Reflects the status of the eCall Notification.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief ECCS_NORMAL.
   */
  ECCS_NORMAL,

  /**
   * @brief ECCS_CALL_IN_PROGRESS.
   */
  ECCS_CALL_IN_PROGRESS,

  /**
   * @brief ECCS_CALL_CANCELLED.
   */
  ECCS_CALL_CANCELLED,

  /**
   * @brief CALL_COMPLETED.
   */
  CALL_COMPLETED,

  /**
   * @brief ECCS_CALL_UNSUCCESSFUL.
   */
  ECCS_CALL_UNSUCCESSFUL,

  /**
   * @brief ECCS_ECALL_CONFIGURED_OFF.
   */
  ECCS_ECALL_CONFIGURED_OFF,

  /**
   * @brief ECCS_CALL_COMPLETE_DTMF_TIMEOUT.
   */
  ECCS_CALL_COMPLETE_DTMF_TIMEOUT
};
} // ECallConfirmationStatus

namespace FileType {
/**
 * @brief Enumeration FileType.
 *
 * Enumeration listing possible file types.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief GRAPHIC_BMP.
   */
  GRAPHIC_BMP,

  /**
   * @brief GRAPHIC_JPEG.
   */
  GRAPHIC_JPEG,

  /**
   * @brief GRAPHIC_PNG.
   */
  GRAPHIC_PNG,

  /**
   * @brief AUDIO_WAVE.
   */
  AUDIO_WAVE,

  /**
   * @brief AUDIO_MP3.
   */
  AUDIO_MP3,

  /**
   * @brief AUDIO_AAC.
   */
  AUDIO_AAC,

  /**
   * @brief BINARY.
   */
  BINARY,

  /**
   * @brief JSON.
   */
  JSON
};
} // FileType

namespace PowerModeQualificationStatus {
/**
 * @brief Enumeration PowerModeQualificationStatus.
 *
 * Reflects the status of the current power mode qualification.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief POWER_MODE_UNDEFINED.
   */
  POWER_MODE_UNDEFINED,

  /**
   * @brief POWER_MODE_EVALUATION_IN_PROGRESS.
   */
  POWER_MODE_EVALUATION_IN_PROGRESS,

  /**
   * @brief NOT_DEFINED.
   */
  NOT_DEFINED,

  /**
   * @brief POWER_MODE_OK.
   */
  POWER_MODE_OK
};
} // PowerModeQualificationStatus

namespace PowerModeStatus {
/**
 * @brief Enumeration PowerModeStatus.
 *
 * Reflects the status of the current power mode.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief KEY_OUT.
   */
  KEY_OUT,

  /**
   * @brief KEY_RECENTLY_OUT.
   */
  KEY_RECENTLY_OUT,

  /**
   * @brief KEY_APPROVED_0.
   */
  KEY_APPROVED_0,

  /**
   * @brief POST_ACCESORY_0.
   */
  POST_ACCESORY_0,

  /**
   * @brief ACCESORY_1.
   */
  ACCESORY_1,

  /**
   * @brief POST_IGNITION_1.
   */
  POST_IGNITION_1,

  /**
   * @brief IGNITION_ON_2.
   */
  IGNITION_ON_2,

  /**
   * @brief RUNNING_2.
   */
  RUNNING_2,

  /**
   * @brief CRANK_3.
   */
  CRANK_3
};
} // PowerModeStatus

namespace CarModeStatus {
/**
 * @brief Enumeration CarModeStatus.
 *
 * Reflects the status of the current car mode.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief CMS_NORMAL.
   */
  CMS_NORMAL,

  /**
   * @brief CMS_FACTORY.
   */
  CMS_FACTORY,

  /**
   * @brief CMS_TRANSPORT.
   */
  CMS_TRANSPORT,

  /**
   * @brief CMS_CRASH.
   */
  CMS_CRASH
};
} // CarModeStatus

namespace VehicleDataResultCode {
/**
 * @brief Enumeration VehicleDataResultCode.
 *
 * Enumeration that describes possible result codes of a vehicle data entry request.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief VDRC_SUCCESS.
   */
  VDRC_SUCCESS,

  /**
   * @brief VDRC_TRUNCATED_DATA.
   */
  VDRC_TRUNCATED_DATA,

  /**
   * @brief VDRC_DISALLOWED.
   */
  VDRC_DISALLOWED,

  /**
   * @brief VDRC_USER_DISALLOWED.
   */
  VDRC_USER_DISALLOWED,

  /**
   * @brief VDRC_INVALID_ID.
   */
  VDRC_INVALID_ID,

  /**
   * @brief VDRC_DATA_NOT_AVAILABLE.
   */
  VDRC_DATA_NOT_AVAILABLE,

  /**
   * @brief VDRC_DATA_ALREADY_SUBSCRIBED.
   */
  VDRC_DATA_ALREADY_SUBSCRIBED,

  /**
   * @brief VDRC_DATA_NOT_SUBSCRIBED.
   */
  VDRC_DATA_NOT_SUBSCRIBED,

  /**
   * @brief VDRC_IGNORED.
   */
  VDRC_IGNORED
};
} // VehicleDataResultCode

namespace TouchType {
/**
 * @brief Enumeration TouchType.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief BEGIN.
   */
  BEGIN,

  /**
   * @brief MOVE.
   */
  MOVE,

  /**
   * @brief END.
   */
  END
};
} // TouchType

namespace PermissionStatus {
/**
 * @brief Enumeration PermissionStatus.
 *
 * Enumeration that describes possible permission states of a policy table entry.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief PS_ALLOWED.
   */
  PS_ALLOWED,

  /**
   * @brief PS_DISALLOWED.
   */
  PS_DISALLOWED,

  /**
   * @brief PS_USER_DISALLOWED.
   */
  PS_USER_DISALLOWED,

  /**
   * @brief PS_USER_CONSENT_PENDING.
   */
  PS_USER_CONSENT_PENDING
};
} // PermissionStatus

namespace KeyboardLayout {
/**
 * @brief Enumeration KeyboardLayout.
 *
 * Enumeration listing possible keyboard layouts.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief QWERTY.
   */
  QWERTY,

  /**
   * @brief QWERTZ.
   */
  QWERTZ,

  /**
   * @brief AZERTY.
   */
  AZERTY
};
} // KeyboardLayout

namespace KeyboardEvent {
/**
 * @brief Enumeration KeyboardEvent.
 *
 * Enumeration listing possible keyboard events.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief KEYPRESS.
   */
  KEYPRESS,

  /**
   * @brief ENTRY_SUBMITTED.
   */
  ENTRY_SUBMITTED,

  /**
   * @brief ENTRY_CANCELLED.
   */
  ENTRY_CANCELLED,

  /**
   * @brief ENTRY_ABORTED.
   */
  ENTRY_ABORTED
};
} // KeyboardEvent

namespace KeypressMode {
/**
 * @brief Enumeration KeypressMode.
 *
 * Enumeration listing possible keyboard events.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SINGLE_KEYPRESS.
   *
   * Each keypress is individually sent as the user presses the keyboard keys.
   */
  SINGLE_KEYPRESS,

  /**
   * @brief QUEUE_KEYPRESSES.
   *
   * The keypresses are queued and a string is eventually sent once the user chooses to submit their entry.
   */
  QUEUE_KEYPRESSES,

  /**
   * @brief RESEND_CURRENT_ENTRY.
   *
   * The keypresses are queue and a string is sent each time the user presses a keyboard key; the string contains the entire current entry.
   */
  RESEND_CURRENT_ENTRY
};
} // KeypressMode

namespace RequestType {
/**
 * @brief Enumeration RequestType.
 *
 * Enumeration listing possible asynchronous requests.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief HTTP.
   */
  HTTP,

  /**
   * @brief FILE_RESUME.
   */
  FILE_RESUME,

  /**
   * @brief AUTH_REQUEST.
   */
  AUTH_REQUEST,

  /**
   * @brief AUTH_CHALLENGE.
   */
  AUTH_CHALLENGE,

  /**
   * @brief AUTH_ACK.
   */
  AUTH_ACK,

  /**
   * @brief PROPRIETARY.
   */
  PROPRIETARY
};
} // RequestType

namespace AppHMIType {
/**
 * @brief Enumeration AppHMIType.
 *
 * Enumeration listing possible app types.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief DEFAULT.
   */
  DEFAULT,

  /**
   * @brief COMMUNICATION.
   */
  COMMUNICATION,

  /**
   * @brief MEDIA.
   */
  MEDIA,

  /**
   * @brief MESSAGING.
   */
  MESSAGING,

  /**
   * @brief NAVIGATION.
   */
  NAVIGATION,

  /**
   * @brief INFORMATION.
   */
  INFORMATION,

  /**
   * @brief SOCIAL.
   */
  SOCIAL,

  /**
   * @brief BACKGROUND_PROCESS.
   */
  BACKGROUND_PROCESS,

  /**
   * @brief TESTING.
   */
  TESTING,

  /**
   * @brief SYSTEM.
   */
  SYSTEM
};
} // AppHMIType

namespace FunctionID {
/**
 * @brief Enumeration FunctionID.
 *
 * Enumeration linking function names with function IDs in AppLink protocol.
 * Assumes enumeration starts at value 0.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief RESERVED.
   */
  RESERVED = 0,

  /**
   * @brief RegisterAppInterfaceID.
   */
  RegisterAppInterfaceID = 1,

  /**
   * @brief UnregisterAppInterfaceID.
   */
  UnregisterAppInterfaceID = 2,

  /**
   * @brief SetGlobalPropertiesID.
   */
  SetGlobalPropertiesID = 3,

  /**
   * @brief ResetGlobalPropertiesID.
   */
  ResetGlobalPropertiesID = 4,

  /**
   * @brief AddCommandID.
   */
  AddCommandID = 5,

  /**
   * @brief DeleteCommandID.
   */
  DeleteCommandID = 6,

  /**
   * @brief AddSubMenuID.
   */
  AddSubMenuID = 7,

  /**
   * @brief DeleteSubMenuID.
   */
  DeleteSubMenuID = 8,

  /**
   * @brief CreateInteractionChoiceSetID.
   */
  CreateInteractionChoiceSetID = 9,

  /**
   * @brief PerformInteractionID.
   */
  PerformInteractionID = 10,

  /**
   * @brief DeleteInteractionChoiceSetID.
   */
  DeleteInteractionChoiceSetID = 11,

  /**
   * @brief AlertID.
   */
  AlertID = 12,

  /**
   * @brief ShowID.
   */
  ShowID = 13,

  /**
   * @brief SpeakID.
   */
  SpeakID = 14,

  /**
   * @brief SetMediaClockTimerID.
   */
  SetMediaClockTimerID = 15,

  /**
   * @brief PerformAudioPassThruID.
   */
  PerformAudioPassThruID = 16,

  /**
   * @brief EndAudioPassThruID.
   */
  EndAudioPassThruID = 17,

  /**
   * @brief SubscribeButtonID.
   */
  SubscribeButtonID = 18,

  /**
   * @brief UnsubscribeButtonID.
   */
  UnsubscribeButtonID = 19,

  /**
   * @brief SubscribeVehicleDataID.
   */
  SubscribeVehicleDataID = 20,

  /**
   * @brief UnsubscribeVehicleDataID.
   */
  UnsubscribeVehicleDataID = 21,

  /**
   * @brief GetVehicleDataID.
   */
  GetVehicleDataID = 22,

  /**
   * @brief ReadDIDID.
   */
  ReadDIDID = 23,

  /**
   * @brief GetDTCsID.
   */
  GetDTCsID = 24,

  /**
   * @brief ScrollableMessageID.
   */
  ScrollableMessageID = 25,

  /**
   * @brief SliderID.
   */
  SliderID = 26,

  /**
   * @brief ShowConstantTBTID.
   */
  ShowConstantTBTID = 27,

  /**
   * @brief AlertManeuverID.
   */
  AlertManeuverID = 28,

  /**
   * @brief UpdateTurnListID.
   */
  UpdateTurnListID = 29,

  /**
   * @brief ChangeRegistrationID.
   */
  ChangeRegistrationID = 30,

  /**
   * @brief GenericResponseID.
   */
  GenericResponseID = 31,

  /**
   * @brief PutFileID.
   */
  PutFileID = 32,

  /**
   * @brief DeleteFileID.
   */
  DeleteFileID = 33,

  /**
   * @brief ListFilesID.
   */
  ListFilesID = 34,

  /**
   * @brief SetAppIconID.
   */
  SetAppIconID = 35,

  /**
   * @brief SetDisplayLayoutID.
   */
  SetDisplayLayoutID = 36,

  /**
   * @brief DiagnosticMessageID.
   */
  DiagnosticMessageID = 37,

  /**
   * @brief SystemRequestID.
   */
  SystemRequestID = 38,

  /**
   * @brief OnHMIStatusID.
   */
  OnHMIStatusID = 32768,

  /**
   * @brief OnAppInterfaceUnregisteredID.
   */
  OnAppInterfaceUnregisteredID = 32769,

  /**
   * @brief OnButtonEventID.
   */
  OnButtonEventID = 32770,

  /**
   * @brief OnButtonPressID.
   */
  OnButtonPressID = 32771,

  /**
   * @brief OnVehicleDataID.
   */
  OnVehicleDataID = 32772,

  /**
   * @brief OnCommandID.
   */
  OnCommandID = 32773,

  /**
   * @brief OnTBTClientStateID.
   */
  OnTBTClientStateID = 32774,

  /**
   * @brief OnDriverDistractionID.
   */
  OnDriverDistractionID = 32775,

  /**
   * @brief OnPermissionsChangeID.
   */
  OnPermissionsChangeID = 32776,

  /**
   * @brief OnAudioPassThruID.
   */
  OnAudioPassThruID = 32777,

  /**
   * @brief OnLanguageChangeID.
   */
  OnLanguageChangeID = 32778,

  /**
   * @brief OnKeyboardInputID.
   */
  OnKeyboardInputID = 32779,

  /**
   * @brief OnTouchEventID.
   */
  OnTouchEventID = 32780,

  /**
   * @brief OnSystemRequestID.
   */
  OnSystemRequestID = 32781,

  /**
   * @brief OnHashChangeID.
   */
  OnHashChangeID = 32782,

  /**
   * @brief EncodedSyncPDataID.
   */
  EncodedSyncPDataID = 65536,

  /**
   * @brief SyncPDataID.
   */
  SyncPDataID = 65537,

  /**
   * @brief OnEncodedSyncPDataID.
   */
  OnEncodedSyncPDataID = 98304,

  /**
   * @brief OnSyncPDataID.
   */
  OnSyncPDataID = 98305
};
} // FunctionID

namespace messageType {
/**
 * @brief Enumeration messageType.
 *
 * 
 *     	Enumeration linking message types with function types in WiPro protocol.
 *     	Assumes enumeration starts at value 0.
 *     
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief request.
   */
  request = 0,

  /**
   * @brief response.
   */
  response = 1,

  /**
   * @brief notification.
   */
  notification = 2
};
} // messageType
} // mobile_apis
#endif //__CSMARTFACTORY_MOBILE_API_8275382ecba511e3a201902b34794c53_H__


