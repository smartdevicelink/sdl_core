/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/policies_manager/policies_manager.h"

#include <algorithm>

#include "json/json.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"

namespace FunctionID = mobile_apis::FunctionID;

namespace {
const char kRpcsKey[] = "rpcs";
const char kHmiLevelsKey[] = "hmi_levels";

std::map<std::string, FunctionID::eType> kFunctionIDs {
  { "RegisterAppInterface", FunctionID::RegisterAppInterfaceID },
  { "UnregisterAppInterface", FunctionID::UnregisterAppInterfaceID },
  { "SetGlobalProperties", FunctionID::SetGlobalPropertiesID },
  { "ResetGlobalProperties", FunctionID::ResetGlobalPropertiesID },
  { "AddCommand", FunctionID::AddCommandID },
  { "DeleteCommand", FunctionID::DeleteCommandID },
  { "AddSubMenu", FunctionID::AddSubMenuID },
  { "DeleteSubMenu", FunctionID::DeleteSubMenuID },
  { "CreateInteractionChoiceSet", FunctionID::CreateInteractionChoiceSetID },
  { "PerformInteraction", FunctionID::PerformInteractionID },
  { "DeleteInteractionChoiceSet", FunctionID::DeleteInteractionChoiceSetID },
  { "Alert", FunctionID::AlertID },
  { "Show", FunctionID::ShowID },
  { "Speak", FunctionID::SpeakID },
  { "SetMediaClockTimer", FunctionID::SetMediaClockTimerID },
  { "EncodedSyncPData", FunctionID::EncodedSyncPDataID },
  { "PerformAudioPassThru", FunctionID::PerformAudioPassThruID },
  { "EndAudioPassThru", FunctionID::EndAudioPassThruID },
  { "SubscribeButton", FunctionID::SubscribeButtonID },
  { "UnsubscribeButton", FunctionID::UnsubscribeButtonID },
  { "SubscribeVehicleData", FunctionID::SubscribeVehicleDataID },
  { "UnsubscribeVehicleData", FunctionID::UnsubscribeVehicleDataID },
  { "GetVehicleData", FunctionID::GetVehicleDataID },
  { "ReadDID", FunctionID::ReadDIDID },
  { "GetDTCs", FunctionID::GetDTCsID },
  { "ScrollableMessage", FunctionID::ScrollableMessageID },
  { "Slider", FunctionID::SliderID },
  { "ShowConstantTBT", FunctionID::ShowConstantTBTID },
  { "AlertManeuver", FunctionID::AlertManeuverID },
  { "UpdateTurnList", FunctionID::UpdateTurnListID },
  { "ChangeRegistration", FunctionID::ChangeRegistrationID },
  { "GenericResponse", FunctionID::GenericResponseID },
  { "DialNumber", FunctionID::DialNumberID },
  { "PutFile", FunctionID::PutFileID },
  { "DeleteFile", FunctionID::DeleteFileID },
  { "ListFilesID", FunctionID::ListFilesID },
  { "SetAppIcon", FunctionID::SetAppIconID },
  { "SetDisplayLayout", FunctionID::SetDisplayLayoutID },
  { "OnHMIStatus", FunctionID::OnHMIStatusID },
  { "OnAppInterfaceUnregistered", FunctionID::OnAppInterfaceUnregisteredID },
  { "OnButtonEvent", FunctionID::OnButtonEventID },
  { "OnButtonPress", FunctionID::OnButtonPressID },
  { "OnVehicleData", FunctionID::OnVehicleDataID },
  { "OnCommand", FunctionID::OnCommandID },
  { "OnEncodedSyncPData", FunctionID::OnEncodedSyncPDataID },
  { "OnTBTClientState", FunctionID::OnTBTClientStateID },
  { "OnPermissionsChange", FunctionID::OnPermissionsChangeID },
  { "OnAudioPassThru", FunctionID::OnAudioPassThruID },
  { "OnLanguageChange", FunctionID::OnLanguageChangeID },
  { "OnDriverDistraction", FunctionID::OnDriverDistractionID },
  { "OnSyncPData", FunctionID::EncodedSyncPDataID }
};

std::map<std::string, mobile_apis::HMILevel::eType> kHmiLevels {
  { "FULL", mobile_apis::HMILevel::HMI_FULL },
  { "LIMITED", mobile_apis::HMILevel::HMI_LIMITED },
  { "BACKGROUND", mobile_apis::HMILevel::HMI_BACKGROUND },
  { "NONE", mobile_apis::HMILevel::HMI_NONE }
};
}  // namespace

namespace application_manager {
namespace policies_manager {

//! ---------------------------------------------------------------------------

log4cxx::LoggerPtr PoliciesManager::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("PoliciesManager"));

//! ---------------------------------------------------------------------------

PoliciesManager::PoliciesManager() {
}

PoliciesManager::~PoliciesManager() {
}

bool PoliciesManager::init() {
  std::string json_string;
  std::string policies_file_name = profile::Profile::instance()
      ->policies_file_name();
  if (!file_system::FileExists(policies_file_name)) {
    return false;
  }

  if (!file_system::ReadFile(policies_file_name, json_string)) {
    return false;
  }

  try {
    Json::Reader reader_;
    Json::Value json_;
    bool parsedSuccess = reader_.parse(json_string, json_, false);
    if (!parsedSuccess) {
      LOG4CXX_ERROR(
          logger_,
          "Failed to parse JSON: " << reader_.getFormatedErrorMessages());
      return false;
    }

    if (!json_.isMember(kRpcsKey)) {
      LOG4CXX_ERROR(logger_, kRpcsKey << " doesn't exist.");
      return false;
    }

    const Json::Value& items = json_[kRpcsKey];
    for (Json::ValueIterator i = items.begin(); i != items.end(); ++i) {
      const Json::Value& item = i.key();
      HmiLevels hmi_levels;

      if (items.isMember(kHmiLevelsKey)) {
        continue;
      }

      const Json::Value levels = items[item.asString()][kHmiLevelsKey];
      for (unsigned int j = 0; j < levels.size(); ++j) {
        hmi_levels.push_back(kHmiLevels[levels[j].asString()]);
      }

      items_.insert(PoliciesItem(kFunctionIDs[item.asString()], hmi_levels));
    }
  } catch(...) {
    return false;
  }

  return true;
}

bool PoliciesManager::is_valid_hmi_status(
    FunctionID::eType function,
    mobile_apis::HMILevel::eType status) {
  if (items_.find(function) == items_.end()) {
    return false;
  }

  HmiLevels levels = items_[function];
  if (std::find(levels.begin(), levels.end(), status) != levels.end())
    return true;

  return false;
}

}  // namespace policies_manager
}  // namespace application_manager

