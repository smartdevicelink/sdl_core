/**
 * @file v4_protocol_v1_2_no_extra.h
 * @brief Generated class v4_protocol_v1_2_no_extra header file.
 *
 * This class is a part of SmartObjects solution. It provides
 * factory functionallity which allows client to use SmartSchemas
 * in accordance with definitions from v4_protocol_v1_2_no_extra.xml file
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

#ifndef __CSMARTFACTORY_V4_PROTOCOL_V1_2_NO_EXTRA_956e457ecba511e38904902b34794c53_H__
#define __CSMARTFACTORY_V4_PROTOCOL_V1_2_NO_EXTRA_956e457ecba511e38904902b34794c53_H__

namespace NsSmartDeviceLinkRPC {
namespace V1 {
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
   * @brief Choice.
   */
  Choice,

  /**
   * @brief SyncMsgVersion.
   */
  SyncMsgVersion,

  /**
   * @brief StartTime.
   */
  StartTime,

  /**
   * @brief TextField.
   */
  TextField,

  /**
   * @brief DisplayCapabilities.
   */
  DisplayCapabilities,

  /**
   * @brief ButtonCapabilities.
   */
  ButtonCapabilities,

  /**
   * @brief MenuParams.
   */
  MenuParams,

  /**
   * @brief TTSChunk.
   */
  TTSChunk
};
} // StructIdentifiers

namespace FunctionID {
/**
 * @brief Enumeration FunctionID.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief RegisterAppInterface.
   */
  RegisterAppInterface,

  /**
   * @brief UnregisterAppInterface.
   */
  UnregisterAppInterface,

  /**
   * @brief SetGlobalProperties.
   */
  SetGlobalProperties,

  /**
   * @brief ResetGlobalProperties.
   */
  ResetGlobalProperties,

  /**
   * @brief AddCommand.
   */
  AddCommand,

  /**
   * @brief DeleteCommand.
   */
  DeleteCommand,

  /**
   * @brief AddSubMenu.
   */
  AddSubMenu,

  /**
   * @brief DeleteSubMenu.
   */
  DeleteSubMenu,

  /**
   * @brief CreateInteractionChoiceSet.
   */
  CreateInteractionChoiceSet,

  /**
   * @brief PerformInteraction.
   */
  PerformInteraction,

  /**
   * @brief DeleteInteractionChoiceSet.
   */
  DeleteInteractionChoiceSet,

  /**
   * @brief Alert.
   */
  Alert,

  /**
   * @brief Show.
   */
  Show,

  /**
   * @brief Speak.
   */
  Speak,

  /**
   * @brief SetMediaClockTimer.
   */
  SetMediaClockTimer,

  /**
   * @brief EncodedSyncPData.
   */
  EncodedSyncPData,

  /**
   * @brief SubscribeButton.
   */
  SubscribeButton,

  /**
   * @brief UnsubscribeButton.
   */
  UnsubscribeButton,

  /**
   * @brief GenericResponse.
   */
  GenericResponse,

  /**
   * @brief OnHMIStatus.
   */
  OnHMIStatus,

  /**
   * @brief OnAppInterfaceUnregistered.
   */
  OnAppInterfaceUnregistered,

  /**
   * @brief OnButtonEvent.
   */
  OnButtonEvent,

  /**
   * @brief OnButtonPress.
   */
  OnButtonPress,

  /**
   * @brief OnCommand.
   */
  OnCommand,

  /**
   * @brief OnEncodedSyncPData.
   */
  OnEncodedSyncPData,

  /**
   * @brief OnTBTClientState.
   */
  OnTBTClientState,

  /**
   * @brief OnDriverDistraction.
   */
  OnDriverDistraction
};
} // FunctionID

namespace messageType {
/**
 * @brief Enumeration messageType.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief request.
   */
  request,

  /**
   * @brief response.
   */
  response,

  /**
   * @brief notification.
   */
  notification
};
} // messageType

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
   * @brief INVALID_DATA.
   *
   * The data sent is invalid. For example
   * Invalid Json syntax
   * Parameters out of bounds (number or enum range)
   * Mandatory parameters not provided
   * Parameter provided with wrong type
   * Invalid characters
   * Empty string
   */
  INVALID_DATA,

  /**
   * @brief UNSUPPORTED_REQUEST.
   *
   * The request is not supported by Sync
   */
  UNSUPPORTED_REQUEST,

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
   * @brief INVALID_ID.
   *
   * One of the provided IDs is not valid. For example
   * This applies to CorrelationID, SubscriptionID [@TODO if SubscriptionID is used], CommandID, MenuID, [@TODO: missed one?]
   */
  INVALID_ID,

  /**
   * @brief DUPLICATE_NAME.
   *
   * There was a conflict with an registered name (application or menu item) or vr command
   */
  DUPLICATE_NAME,

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
   * @brief UNSUPPORTED_VERSION.
   *
   * Sync doesn't support the protocol that is requested by the mobile application
   */
  UNSUPPORTED_VERSION,

  /**
   * @brief WRONG_LANGUAGE.
   *
   * The requested language is currently not supported.
   * Might be because of a mismatch of the currently active language on Sync and the requested language
   */
  WRONG_LANGUAGE,

  /**
   * @brief APPLICATION_NOT_REGISTERED.
   *
   * An command can not be executed because no application has been registered with RegisterApplication.
   */
  APPLICATION_NOT_REGISTERED,

  /**
   * @brief IN_USE.
   *
   * The data may not be changed, because it is currently in use.
   * For example when trying to delete a command set that is currently involved in an interaction.
   */
  IN_USE,

  /**
   * @brief SUBSCRIBED_ALREADY.
   *
   * The item to subscribe to is already subscribed to.
   */
  SUBSCRIBED_ALREADY,

  /**
   * @brief REJECTED.
   *
   * The requested command was rejected, e.g. because mobile app is in background and cannot perform any HMI commands.
   * Or an HMI command (e.g. Speak) is rejected because a higher priority HMI command (e.g. Alert) is playing.
   */
  REJECTED,

  /**
   * @brief ABORTED.
   *
   * A command was aborted, for example due to user interaction (e.g. user pressed button).
   * Or an HMI command (e.g. Speak) is aborted because a higher priority HMI command (e.g. Alert) was requested.
   */
  ABORTED,

  /**
   * @brief IGNORED.
   *
   * A command was ignored, because the intended result is already in effect.
   * For example, SetMediaClockTimer was used to pause the media clock although the clock is paused already.
   * NOTE: potentially replaces SUBSCRIBED_ALREADY
   */
  IGNORED,

  /**
   * @brief UNSUPPORTED_BUTTON.
   *
   * A button that was requested for subscription is not supported under the current system.
   * NOTE: could become a more generic UNSUPPORTED_RESOURCE by merging with VEHIVLE_DATA_NOT_AVAILABLE.
   */
  UNSUPPORTED_BUTTON,

  /**
   * @brief FILE_NOT_FOUND.
   *
   * A specified file could not be found on Sync.
   */
  FILE_NOT_FOUND,

  /**
   * @brief GENERIC_ERROR.
   *
   * Provided data is valid but something went wrong in the lower layers.
   */
  GENERIC_ERROR
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
   * A button was released, after it was pressed for a long time
   * Actual timing is defined by Sync and may vary
   */
  LONG,

  /**
   * @brief SHORT.
   *
   * A button was released, after it was pressed for a short time
   * Actual timing is defined by Sync and may vary
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
 *
 * The set of potential languages
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief EN_US.
   *
   * US English
   */
  EN_US,

  /**
   * @brief ES_MX.
   *
   * Mexican Spanish
   */
  ES_MX,

  /**
   * @brief FR_CA.
   *
   * Canadian French
   */
  FR_CA
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
  RESUME
};
} // UpdateMode

namespace InteractionMode {
/**
 * @brief Enumeration InteractionMode.
 *
 * For application-requested interactions, this mode indicates the method in which the user is notified and uses the interaction.
 * This mode causes the interaction to only occur on the display, meaning the choices are provided only via the display.
 * Selections are made with the OK and Seek Right and Left, Tune Up and Down buttons.
 * This mode causes the interaction to only occur using V4.
 * Selections are made by saying the command.
 * This mode causes both a VR and display selection option for an interaction.
 * Selections can be made either from the menu display or by speaking the command.
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
   * @brief NOT_AUDIBLE.
   */
  NOT_AUDIBLE
};
} // AudioStreamingState

namespace SystemContext {
/**
 * @brief Enumeration SystemContext.
 *
 * Enumeraction that describes possible contexts and app's HMI might be in.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief SYSCTXT_MAIN.
   */
  SYSCTXT_MAIN,

  /**
   * @brief SYSCTXT_VRSESSION.
   */
  SYSCTXT_VRSESSION,

  /**
   * @brief SYSCTXT_MENU.
   */
  SYSCTXT_MENU
};
} // SystemContext

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
  FACTORY_DEFAULTS
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
  TS_VR
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
   * @brief TEXT.
   */
  TEXT,

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
   * @brief Text.
   */
  Text
};
} // VrCapabilities

namespace AudioCaptureQuality {
/**
 * @brief Enumeration AudioCaptureQuality.
 *
 * Describes different quality options for PerformAudioCapture.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief QUALITY_8KHZ.
   */
  QUALITY_8KHZ,

  /**
   * @brief QUALITY_16KHZ.
   */
  QUALITY_16KHZ
};
} // AudioCaptureQuality

namespace ButtonName {
/**
 * @brief Enumeration ButtonName.
 *
 * Defines the hard (physical) and soft (touchscreen) buttons available from SYNC
 *
 * @note Are these all supported buttons?
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
  PRESET_9
};
} // ButtonName

namespace MediaClockFormat {
/**
 * @brief Enumeration MediaClockFormat.
 *
 * @note First complete values draft. Review needed.
 */
enum eType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief CLOCK1.
   *
   * minutesFieldWidth = 2;minutesFieldMax = 19;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 19;maxMinutes = 59;maxSeconds = 59;
   * used for Type II and CID headunits
   */
  CLOCK1,

  /**
   * @brief CLOCK2.
   *
   * minutesFieldWidth = 3;minutesFieldMax = 199;secondsFieldWidth = 2;secondsFieldMax = 99;maxHours = 59;maxMinutes = 59;maxSeconds = 59;
   * used for Type V headunit
   */
  CLOCK2,

  /**
   * @brief CLOCKTEXT1.
   *
   * 5 characters possible
   * Format:      1|sp   c   :|sp   c   c
   * 1|sp : digit "1" or space
   * c    : character out of following character set: sp|0-9|[letters, see TypeII column in XLS. See [@TODO: create file ref]]
   * :|sp : colon or space
   * used for Type II headunit
   */
  CLOCKTEXT1,

  /**
   * @brief CLOCKTEXT2.
   *
   * 5 chars possible
   * Format:      1|sp   c   :|sp   c   c
   * 1|sp : digit "1" or space
   * c    : character out of following character set: sp|0-9|[letters, see CID column in XLS. See [@TODO: create file ref]]
   * :|sp : colon or space
   * used for CID headunit
   * NOTE: difference between CLOCKTEXT1 and CLOCKTEXT2 is the supported character set
   */
  CLOCKTEXT2,

  /**
   * @brief CLOCKTEXT3.
   *
   * 6 chars possible
   * Format:      1|sp   c   c   :|sp   c   c
   * 1|sp : digit "1" or space
   * c    : character out of following character set: sp|0-9|[letters, see Type 5 column in XLS]. See [@TODO: create file ref]
   * :|sp : colon or space
   * used for Type V headunit
   */
  CLOCKTEXT3
};
} // MediaClockFormat

namespace DisplayType {
/**
 * @brief Enumeration DisplayType.
 *
 * See DAES for further infos regarding the displays]
 *
 * @note Meaningful description is missing
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
  MFD5
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
   * The first line of the main field of persistent display
   * Applies to "Show"
   */
  mainField1,

  /**
   * @brief mainField2.
   *
   * The second line of the main field of persistent display
   * Applies to "Show"
   */
  mainField2,

  /**
   * @brief statusBar.
   *
   * The status bar on Nav
   * Applies to "Show"
   */
  statusBar,

  /**
   * @brief mediaClock.
   *
   * Text value for MediaClock field. Has to be properly formatted by Mobile App according to Sync capabilities.
   * Applies to "Show"
   */
  mediaClock,

  /**
   * @brief mediaTrack.
   *
   * The track field of NGN type ACMs.
   * This field is only available for media applications.
   * Applies to "Show"
   */
  mediaTrack,

  /**
   * @brief alertText1.
   *
   * The first line of the alert text field
   * Applies to "Alert"
   */
  alertText1,

  /**
   * @brief alertText2.
   *
   * The second line of the alert text field
   * Applies to "Alert"
   */
  alertText2
};
} // TextFieldName

namespace CharacterSet {
/**
 * @brief Enumeration CharacterSet.
 *
 * The list of potential character sets
 *
 * @note Real sets have not been defined yet?
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
 * Enumeration that describes possible states of turn-by-turn module.
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
  ROUTE_CANCELLED
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
  TIMEOUTPROMPT
};
} // GlobalProperty
} // NsSmartDeviceLinkRPC
} // V1
#endif //__CSMARTFACTORY_V4_PROTOCOL_V1_2_NO_EXTRA_956e457ecba511e38904902b34794c53_H__


