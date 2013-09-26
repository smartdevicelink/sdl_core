/**
  * Copyright (c) 2013, Ford Motor Company
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

var ButtonEventMode = {
  BUTTONUP: 0,
  BUTTONDOWN: 1
}

var IgnitionStatus = {
  IS_UNKNOWN: 0,
  IS_OFF: 1,
  IS_ACCESSORY: 2,
  IS_RUN: 3,
  IS_START: 4,
  IS_INVALID: 5
}

var ApplicationsCloseReason = {
  IGNITION_OFF: 0,
  MASTER_RESET: 1,
  FACTORY_DEFAULTS: 2
}

var PRNDL = {
  PARK: 0,
  REVERSE: 1,
  NEUTRAL: 2,
  DRIVE: 3,
  SPORT: 4,
  LOWGEAR: 5,
  FIRST: 6,
  SECOND: 7,
  THIRD: 8,
  FOURTH: 9,
  FIFTH: 10,
  SIXTH: 11
}

var ButtonName = {
  OK: 0,
  SEEKLEFT: 1,
  SEEKRIGHT: 2,
  TUNEUP: 3,
  TUNEDOWN: 4,
  PRESET_0: 5,
  PRESET_1: 6,
  PRESET_2: 7,
  PRESET_3: 8,
  PRESET_4: 9,
  PRESET_5: 10,
  PRESET_6: 11,
  PRESET_7: 12,
  PRESET_8: 13,
  PRESET_9: 14,
  CUSTOM_BUTTON: 15
}

var TextFieldName = {
  mainField1: 0,
  mainField2: 1,
  mainField3: 2,
  mainField4: 3,
  statusBar: 4,
  mediaClock: 5,
  mediaTrack: 6,
  alertText1: 7,
  alertText2: 8,
  alertText3: 9,
  scrollableMessageBody: 10,
  initialInteractionText: 11,
  navigationText1: 12,
  navigationText2: 13,
  ETA: 14,
  totalDistance: 15,
  navigationText: 16,
  audioPassThruDisplayText1: 17,
  audioPassThruDisplayText2: 18,
  sliderHeader: 19,
  sliderFooter: 20,
  notificationText: 21
}

var Language = {
  EN_US: 0,
  ES_MX: 1,
  FR_CA: 2,
  DE_DE: 3,
  ES_ES: 4,
  EN_GB: 5,
  RU_RU: 6,
  TR_TR: 7,
  PL_PL: 8,
  FR_FR: 9,
  IT_IT: 10,
  SV_SE: 11,
  PT_PT: 12,
  NL_NL: 13,
  EN_AU: 14,
  ZH_CN: 15,
  ZH_TW: 16,
  JA_JP: 17,
  AR_SA: 18,
  KO_KR: 19,
  PT_BR: 20,
  CS_CZ: 21,
  DA_DK: 22,
  NO_NO: 23
}

var TBTState = {
  ROUTE_UPDATE_REQUEST: 0,
  ROUTE_ACCEPTED: 1,
  ROUTE_REFUSED: 2,
  ROUTE_CANCELLED: 3,
  ETA_REQUEST: 4,
  NEXT_TURN_REQUEST: 5,
  ROUTE_STATUS_REQUEST: 6,
  ROUTE_SUMMARY_REQUEST: 7,
  TRIP_STATUS_REQUEST: 8,
  ROUTE_UPDATE_REQUEST_TIMEOUT: 9
}

var Result = {
  SUCCESS: 0,
  UNSUPPORTED_REQUEST: 1,
  UNSUPPORTED_RESOURCE: 2,
  DISALLOWED: 3,
  REJECTED: 4,
  ABORTED: 5,
  IGNORED: 6,
  RETRY: 7,
  IN_USE: 8,
  DATA_NOT_AVAILABLE: 9,
  TIMED_OUT: 10,
  INVALID_DATA: 11,
  CHAR_LIMIT_EXCEEDED: 12,
  INVALID_ID: 13,
  DUPLICATE_NAME: 14,
  APPLICATION_NOT_REGISTERED: 15,
  WRONG_LANGUAGE: 16,
  OUT_OF_MEMORY: 17,
  TOO_MANY_PENDING_REQUESTS: 18,
  NO_APPS_REGISTERED: 19,
  NO_DEVICES_CONNECTED: 20,
  WARNINGS: 21,
  GENERIC_ERROR: 22,
  USER_DISALLOWED: 23
}

var VehicleDataResultCode = {
  VDRC_SUCCESS: 0,
  VDRC_TRUNCATED_DATA: 1,
  VDRC_DISALLOWED: 2,
  VDRC_USER_DISALLOWED: 3,
  VDRC_INVALID_ID: 4,
  VDRC_DATA_NOT_AVAILABLE: 5,
  VDRC_DATA_ALREADY_SUBSCRIBED: 6,
  VDRC_DATA_NOT_SUBSCRIBED: 7,
  VDRC_IGNORED: 8
}

var ClockUpdateMode = {
  COUNTUP: 0,
  COUNTDOWN: 1,
  PAUSE: 2,
  RESUME: 3,
  CLEAR: 4
}

var IgnitionStableStatus = {
  IGNITION_SWITCH_NOT_STABLE: 0,
  IGNITION_SWITCH_STABLE: 1,
  MISSING_FROM_TRANSMITTER: 2
}

var TextAlignment = {
  LEFT_ALIGNED: 0,
  RIGHT_ALIGNED: 1,
  CENTERED: 2
}

var ButtonPressMode = {
  LONG: 0,
  SHORT: 1
}

var SystemContext = {
  SYSCTXT_MAIN: 0,
  SYSCTXT_VRSESSION: 1,
  SYSCTXT_MENU: 2,
  SYSCTXT_HMI_OBSCURED: 3,
  SYSCTXT_ALERT: 4
}

var MediaClockFormat = {
  CLOCK1: 0,
  CLOCK2: 1,
  CLOCK3: 2,
  CLOCKTEXT1: 3,
  CLOCKTEXT2: 4,
  CLOCKTEXT3: 5,
  CLOCKTEXT4: 6
}

var AppHMIType = {
  DEFAULT: 0,
  COMMUNICATION: 1,
  MEDIA: 2,
  MESSAGING: 3,
  NAVIGATION: 4,
  INFORMATION: 5,
  SOCIAL: 6,
  BACKGROUND_PROCESS: 7,
  TESTING: 8,
  SYSTEM: 9
}

var PrimaryAudioSource = {
  NO_SOURCE_SELECTED: 0,
  USB: 1,
  USB2: 2,
  BLUETOOTH_STEREO_BTST: 3,
  LINE_IN: 4,
  IPOD: 5,
  MOBILE_APP: 6
}

var CompassDirection = {
  NORTH: 0,
  NORTHWEST: 1,
  WEST: 2,
  SOUTHWEST: 3,
  SOUTH: 4,
  SOUTHEAST: 5,
  EAST: 6,
  NORTHEAST: 7
}

var SystemAction = {
  DEFAULT_ACTION: 0,
  STEAL_FOCUS: 1,
  KEEP_CONTEXT: 2
}

var DriverDistractionState = {
  DD_ON: 0,
  DD_OFF: 1
}

var ImageType = {
  STATIC: 0,
  DYNAMIC: 1
}

var WiperStatus = {
  OFF: 0,
  AUTO_OFF: 1,
  OFF_MOVING: 2,
  MAN_INT_OFF: 3,
  MAN_INT_ON: 4,
  MAN_LOW: 5,
  MAN_HIGH: 6,
  MAN_FLICK: 7,
  WASH: 8,
  AUTO_LOW: 9,
  AUTO_HIGH: 10,
  COURTESYWIPE: 11,
  AUTO_ADJUST: 12,
  STALLED: 13,
  NO_DATA_EXISTS: 14
}

var VehicleDataStatus = {
  VDS_NO_DATA_EXISTS: 0,
  VDS_OFF: 1,
  VDS_ON: 2
}

var VehicleDataEventStatus = {
  VDES_NO_EVENT: 0,
  VDES_NO: 1,
  VDES_YES: 2,
  VDES_NOT_SUPPORTED: 3,
  VDES_FAULT: 4
}

var ComponentVolumeStatus = {
  CVS_UNKNOWN: 0,
  CVS_NORMAL: 1,
  CVS_LOW: 2,
  CVS_FAULT: 3,
  CVS_ALERT: 4,
  CVS_NOT_SUPPORTED: 5
}

var SoftButtonType = {
  SBT_TEXT: 0,
  SBT_IMAGE: 1,
  SBT_BOTH: 2
}

var Dimension = {
  Dimension_NO_FIX: 0,
  Dimension_2D: 1,
  Dimension_3D: 2
}

var WarningLightStatus = {
  WLS_OFF: 0,
  WLS_ON: 1,
  WLS_FLASH: 2
}

var DeviceLevelStatus = {
  ZERO_LEVEL_BARS: 0,
  ONE_LEVEL_BARS: 1,
  TWO_LEVEL_BARS: 2,
  THREE_LEVEL_BARS: 3,
  FOUR_LEVEL_BARS: 4,
  NOT_PROVIDED: 5
}

var DeactivateReason = {
  AUDIO: 0,
  PHONECALL: 1,
  NAVIGATIONMAP: 2,
  PHONEMENU: 3,
  SYNCSETTINGS: 4,
  GENERAL: 5
}

var DisplayType = {
  CID: 0,
  TYPE2: 1,
  TYPE5: 2,
  NGN: 3,
  GEN2_8_DMA: 4,
  GEN2_6_DMA: 5,
  MFD3: 6,
  MFD4: 7,
  MFD5: 8
}

var HmiZoneCapabilities = {
  FRONT: 0,
  BACK: 1
}

