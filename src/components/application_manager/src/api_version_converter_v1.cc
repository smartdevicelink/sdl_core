//
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
//

#include "application_manager/api_version_converter_v1.h"
#include "JSONHandler/CSmartFactory.hpp"

namespace application_manager {

const std::string APIVersionConverterV1::RESULT_CODE = "resultCode";

const std::string APIVersionConverterV1::REG_APP_INTERFACE = "RegisterAppInterface";
const std::string APIVersionConverterV1::UNREG_APP_INTERFACE = "UnregisterAppInterface";
const std::string APIVersionConverterV1::SET_GLOBAL_PROPERTIES = "SetGlobalProperties";
const std::string APIVersionConverterV1::RESET_GLOBAL_PROPERTIES = "ResetGlobalProperties";
const std::string APIVersionConverterV1::ADD_COMMAND = "AddCommand";
const std::string APIVersionConverterV1::DELETE_COMMAND = "DeleteCommand";
const std::string APIVersionConverterV1::ADD_SUBMENU = "AddSubMenu";
const std::string APIVersionConverterV1::DELETE_SUBMENU = "DeleteSubMenu";
const std::string APIVersionConverterV1::CREATE_INTERACTION_CHOICE_SET = "CreateInteractionChoiceSet";
const std::string APIVersionConverterV1::PERFORM_INTERACTION = "PerformInteraction";
const std::string APIVersionConverterV1::DELETE_INTERACTION_CHOICE_SET = "DeleteInteractionChoiceSet";
const std::string APIVersionConverterV1::ALERT = "Alert";
const std::string APIVersionConverterV1::SHOW = "Show";
const std::string APIVersionConverterV1::SPEAK = "Speak";
const std::string APIVersionConverterV1::SET_MEDIA_CLOCK_TIMER = "SetMediaClockTimer";
const std::string APIVersionConverterV1::ENCODED_SYNC_PDATA = "EncodedSyncPData";
const std::string APIVersionConverterV1::SUBSRIBE_BUTTON = "SubscribeButton";
const std::string APIVersionConverterV1::UNSUBSRIBE_BUTTON = "UnsubscribeButton";
const std::string APIVersionConverterV1::GENERIC_RESPONSE = "GenericResponse";
const std::string APIVersionConverterV1::ON_HMI_STATUS = "OnHMIStatus";
const std::string APIVersionConverterV1::ON_APP_INTERFACE_UNREG = "OnAppInterfaceUnregistered";
const std::string APIVersionConverterV1::ON_BUTTON_EVENT = "OnButtonEvent";
const std::string APIVersionConverterV1::ON_BUTTON_PRESS = "OnButtonPress";
const std::string APIVersionConverterV1::ON_COMMAND = "OnCommand";
const std::string APIVersionConverterV1::ON_ENCODED_SYNC_PDATA = "OnEncodedSyncPData";
const std::string APIVersionConverterV1::ON_TBT_CLIENT_STATE = "OnTBTClientState";
const std::string APIVersionConverterV1::ON_DRIVE_DISTRACTION = "OnDriverDistraction";

FunctionMap APIVersionConverterV1::functionMap_ =
{
    {NsSmartDeviceLinkRPC::V2::FunctionID::RegisterAppInterfaceID, REG_APP_INTERFACE},
    {NsSmartDeviceLinkRPC::V2::FunctionID::UnregisterAppInterfaceID, UNREG_APP_INTERFACE},
    {NsSmartDeviceLinkRPC::V2::FunctionID::SetGlobalPropertiesID, SET_GLOBAL_PROPERTIES},
    {NsSmartDeviceLinkRPC::V2::FunctionID::ResetGlobalPropertiesID, RESET_GLOBAL_PROPERTIES},
    {NsSmartDeviceLinkRPC::V2::FunctionID::AddCommandID, ADD_COMMAND},
    {NsSmartDeviceLinkRPC::V2::FunctionID::DeleteCommandID, DELETE_COMMAND},
    {NsSmartDeviceLinkRPC::V2::FunctionID::AddSubMenuID, ADD_SUBMENU},
    {NsSmartDeviceLinkRPC::V2::FunctionID::DeleteSubMenuID, DELETE_SUBMENU},
    {NsSmartDeviceLinkRPC::V2::FunctionID::CreateInteractionChoiceSetID, CREATE_INTERACTION_CHOICE_SET},
    {NsSmartDeviceLinkRPC::V2::FunctionID::PerformInteractionID, PERFORM_INTERACTION},
    {NsSmartDeviceLinkRPC::V2::FunctionID::DeleteInteractionChoiceSetID, DELETE_INTERACTION_CHOICE_SET},
    {NsSmartDeviceLinkRPC::V2::FunctionID::AlertID, ALERT},
    {NsSmartDeviceLinkRPC::V2::FunctionID::ShowID, SHOW},
    {NsSmartDeviceLinkRPC::V2::FunctionID::SpeakID, SPEAK},
    {NsSmartDeviceLinkRPC::V2::FunctionID::SetMediaClockTimerID, SET_MEDIA_CLOCK_TIMER},
    {NsSmartDeviceLinkRPC::V2::FunctionID::EncodedSyncPDataID, ENCODED_SYNC_PDATA},
    {NsSmartDeviceLinkRPC::V2::FunctionID::SubscribeButtonID, SUBSRIBE_BUTTON},
    {NsSmartDeviceLinkRPC::V2::FunctionID::UnsubscribeButtonID, UNSUBSRIBE_BUTTON},
    {NsSmartDeviceLinkRPC::V2::FunctionID::GenericResponseID, GENERIC_RESPONSE},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnHMIStatusID, ON_HMI_STATUS},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnAppInterfaceUnregisteredID, ON_APP_INTERFACE_UNREG},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnButtonEventID, ON_BUTTON_EVENT},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnButtonPressID, ON_BUTTON_PRESS},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnCommandID, ON_COMMAND},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnEncodedSyncPDataID, ON_ENCODED_SYNC_PDATA},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnTBTClientStateID, ON_TBT_CLIENT_STATE},
    {NsSmartDeviceLinkRPC::V2::FunctionID::OnDriverDistractionID, ON_DRIVE_DISTRACTION}
};

ResultCodeMap APIVersionConverterV1::resultCodeMap_ =
{
    {NsSmartDeviceLinkRPC::V2::Result::SUCCESS, NsSmartDeviceLinkRPC::V1::Result::SUCCESS},
    {NsSmartDeviceLinkRPC::V2::Result::INVALID_DATA, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::OUT_OF_MEMORY, NsSmartDeviceLinkRPC::V1::Result::OUT_OF_MEMORY},
    {NsSmartDeviceLinkRPC::V2::Result::TOO_MANY_PENDING_REQUESTS, NsSmartDeviceLinkRPC::V1::Result::TOO_MANY_PENDING_REQUESTS},
    {NsSmartDeviceLinkRPC::V2::Result::INVALID_ID, NsSmartDeviceLinkRPC::V1::Result::INVALID_ID},
    {NsSmartDeviceLinkRPC::V2::Result::DUPLICATE_NAME, NsSmartDeviceLinkRPC::V1::Result::DUPLICATE_NAME},
    {NsSmartDeviceLinkRPC::V2::Result::TOO_MANY_APPLICATIONS, NsSmartDeviceLinkRPC::V1::Result::TOO_MANY_APPLICATIONS},
    {NsSmartDeviceLinkRPC::V2::Result::APPLICATION_REGISTERED_ALREADY, NsSmartDeviceLinkRPC::V1::Result::APPLICATION_REGISTERED_ALREADY},
    {NsSmartDeviceLinkRPC::V2::Result::UNSUPPORTED_VERSION, NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_VERSION},
    {NsSmartDeviceLinkRPC::V2::Result::WRONG_LANGUAGE, NsSmartDeviceLinkRPC::V1::Result::WRONG_LANGUAGE},
    {NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED, NsSmartDeviceLinkRPC::V1::Result::APPLICATION_NOT_REGISTERED},
    {NsSmartDeviceLinkRPC::V2::Result::IN_USE, NsSmartDeviceLinkRPC::V1::Result::IN_USE},
    {NsSmartDeviceLinkRPC::V2::Result::SUBSCRIBED_ALREADY, NsSmartDeviceLinkRPC::V1::Result::SUBSCRIBED_ALREADY},
    {NsSmartDeviceLinkRPC::V2::Result::VEHICLE_DATA_NOT_ALLOWED, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::VEHICLE_DATA_NOT_AVAILABLE, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::REJECTED, NsSmartDeviceLinkRPC::V1::Result::REJECTED},
    {NsSmartDeviceLinkRPC::V2::Result::ABORTED, NsSmartDeviceLinkRPC::V1::Result::ABORTED},
    {NsSmartDeviceLinkRPC::V2::Result::IGNORED, NsSmartDeviceLinkRPC::V1::Result::IGNORED},
    {NsSmartDeviceLinkRPC::V2::Result::UNSUPPORTED_BUTTON, NsSmartDeviceLinkRPC::V1::Result::UNSUPPORTED_BUTTON},
    {NsSmartDeviceLinkRPC::V2::Result::FILE_NOT_FOUND, NsSmartDeviceLinkRPC::V1::Result::FILE_NOT_FOUND},
    {NsSmartDeviceLinkRPC::V2::Result::GENERIC_ERROR, NsSmartDeviceLinkRPC::V1::Result::GENERIC_ERROR},
    {NsSmartDeviceLinkRPC::V2::Result::DISALLOWED, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::USER_DISALLOWED, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::TIMED_OUT, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::CANCEL_ROUTE, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::CLOSED, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::ENCRYPTED, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::RETRY, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
    {NsSmartDeviceLinkRPC::V2::Result::WARNINGS, NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA},
};


APIVersionConverterV1::APIVersionConverterV1() : APIVersionConverter()
, messageValid_(true) {
}

APIVersionConverterV1::~APIVersionConverterV1() {
}

tSharedPtr APIVersionConverterV1::convertToV2(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj) {
  using namespace NsSmartDeviceLink::NsJSONHandler::strings;

  NsSmartDeviceLink::NsSmartObjects::CSmartObject tmpObj = smartObj;
  tmpObj[S_PARAMS][S_FUNCTION_ID] =
      findV2FunctionId(tmpObj[S_PARAMS][S_FUNCTION_ID].asString());

  return tSharedPtr(new NsSmartDeviceLink::NsSmartObjects::CSmartObject(tmpObj));
}

tSharedPtr APIVersionConverterV1::convertFromV2(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj) {
  using namespace NsSmartDeviceLink::NsJSONHandler::strings;

  NsSmartDeviceLink::NsSmartObjects::CSmartObject tmpObj = smartObj;
  tmpObj[S_PARAMS][S_FUNCTION_ID] =
      findV1FunctionName(tmpObj[S_PARAMS][S_FUNCTION_ID].asInt());

  if (true == tmpObj[S_MSG_PARAMS].keyExists(RESULT_CODE)) {
    tmpObj[S_PARAMS][RESULT_CODE] =
      findV1ResultCode(tmpObj[S_MSG_PARAMS][RESULT_CODE].asInt());
  }

  return tSharedPtr(new NsSmartDeviceLink::NsSmartObjects::CSmartObject(tmpObj));
}


bool APIVersionConverterV1::isMessageValid() const {
  return messageValid_;
}

int APIVersionConverterV1::findV2FunctionId(const std::string& functionName) const {
  int retValue = NsSmartDeviceLinkRPC::V2::FunctionID::INVALID_ENUM;
  FunctionMap::const_iterator iter;

  for (iter = functionMap_.begin(); iter != functionMap_.end(); ++iter) {
    if (iter->second == functionName) {
      retValue = iter->first;
      break;
    }
  }

  return retValue;
}

std::string APIVersionConverterV1::findV1FunctionName(int functionId) const {
  std::string retValue = "";
  FunctionMap::const_iterator iter;

  for (iter = functionMap_.begin(); iter != functionMap_.end(); ++iter) {
    if (iter->first == functionId) {
      retValue = iter->second;
      break;
    }
  }

  return retValue;
}

int APIVersionConverterV1::findV1ResultCode(int resultCode) {
  int retValue = NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA;
  ResultCodeMap::const_iterator iter;

  for (iter = resultCodeMap_.begin(); iter != resultCodeMap_.end(); ++iter) {
    if (iter->first == resultCode) {
      retValue = iter->second;
      break;
    }
  }

  if (NsSmartDeviceLinkRPC::V1::Result::INVALID_DATA == retValue) {
    messageValid_ = false;
  }

  return retValue;
}

}  // namespace application_manager
