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

std::pair<const char*, FunctionID::eType> kFunctionIDsInitializer[] = {
  std::make_pair("RegisterAppInterface", FunctionID::RegisterAppInterfaceID),
  std::make_pair("UnregisterAppInterface", FunctionID::UnregisterAppInterfaceID),
  std::make_pair("SetGlobalProperties", FunctionID::SetGlobalPropertiesID),
  std::make_pair("ResetGlobalProperties", FunctionID::ResetGlobalPropertiesID),
  std::make_pair("AddCommand", FunctionID::AddCommandID),
  std::make_pair("DeleteCommand", FunctionID::DeleteCommandID),
  std::make_pair("AddSubMenu", FunctionID::AddSubMenuID),
  std::make_pair("DeleteSubMenu", FunctionID::DeleteSubMenuID),
  std::make_pair("CreateInteractionChoiceSet", FunctionID::CreateInteractionChoiceSetID),
  std::make_pair("PerformInteraction", FunctionID::PerformInteractionID),
  std::make_pair("DeleteInteractionChoiceSet", FunctionID::DeleteInteractionChoiceSetID),
  std::make_pair("Alert", FunctionID::AlertID),
  std::make_pair("Show", FunctionID::ShowID),
  std::make_pair("Speak", FunctionID::SpeakID),
  std::make_pair("SetMediaClockTimer", FunctionID::SetMediaClockTimerID),
  std::make_pair("EncodedSyncPData", FunctionID::EncodedSyncPDataID),
  std::make_pair("SyncPData", FunctionID::SyncPDataID),
  std::make_pair("PerformAudioPassThru", FunctionID::PerformAudioPassThruID),
  std::make_pair("EndAudioPassThru", FunctionID::EndAudioPassThruID),
  std::make_pair("SubscribeButton", FunctionID::SubscribeButtonID),
  std::make_pair("UnsubscribeButton", FunctionID::UnsubscribeButtonID),
  std::make_pair("SubscribeVehicleData", FunctionID::SubscribeVehicleDataID),
  std::make_pair("UnsubscribeVehicleData", FunctionID::UnsubscribeVehicleDataID),
  std::make_pair("GetVehicleData", FunctionID::GetVehicleDataID),
  std::make_pair("ReadDID", FunctionID::ReadDIDID),
  std::make_pair("GetDTCs", FunctionID::GetDTCsID),
  std::make_pair("DiagnosticMessage", FunctionID::DiagnosticMessageID),
  std::make_pair("SystemRequest", FunctionID::SystemRequestID),
  std::make_pair("ScrollableMessage", FunctionID::ScrollableMessageID),
  std::make_pair("Slider", FunctionID::SliderID),
  std::make_pair("ShowConstantTBT", FunctionID::ShowConstantTBTID),
  std::make_pair("AlertManeuver", FunctionID::AlertManeuverID),
  std::make_pair("UpdateTurnList", FunctionID::UpdateTurnListID),
  std::make_pair("ChangeRegistration", FunctionID::ChangeRegistrationID),
  std::make_pair("GenericResponse", FunctionID::GenericResponseID),
  std::make_pair("PutFile", FunctionID::PutFileID),
  std::make_pair("DeleteFile", FunctionID::DeleteFileID),
  std::make_pair("ListFiles",  FunctionID::ListFilesID),
  std::make_pair("SetAppIcon", FunctionID::SetAppIconID),
  std::make_pair("SetDisplayLayout", FunctionID::SetDisplayLayoutID),
  std::make_pair("OnHMIStatus", FunctionID::OnHMIStatusID),
  std::make_pair("OnAppInterfaceUnregistered", FunctionID::OnAppInterfaceUnregisteredID),
  std::make_pair("OnButtonEvent", FunctionID::OnButtonEventID),
  std::make_pair("OnButtonPress", FunctionID::OnButtonPressID),
  std::make_pair("OnVehicleData", FunctionID::OnVehicleDataID),
  std::make_pair("OnCommand", FunctionID::OnCommandID),
  std::make_pair("OnEncodedSyncPData", FunctionID::OnEncodedSyncPDataID),
  std::make_pair("OnTBTClientState", FunctionID::OnTBTClientStateID),
  std::make_pair("OnPermissionsChange", FunctionID::OnPermissionsChangeID),
  std::make_pair("OnAudioPassThru", FunctionID::OnAudioPassThruID),
  std::make_pair("OnLanguageChange", FunctionID::OnLanguageChangeID),
  std::make_pair("OnDriverDistraction", FunctionID::OnDriverDistractionID),
  std::make_pair("OnSyncPData", FunctionID::EncodedSyncPDataID)
};
std::map<std::string, FunctionID::eType> kFunctionIDs(kFunctionIDsInitializer,
                                                      kFunctionIDsInitializer +
                                                      ARRAYSIZE(kFunctionIDsInitializer));

std::pair<const char*, mobile_apis::HMILevel::eType> kHmiLevelsInitializer[] = {
  std::make_pair("FULL", mobile_apis::HMILevel::HMI_FULL),
  std::make_pair("LIMITED", mobile_apis::HMILevel::HMI_LIMITED),
  std::make_pair("BACKGROUND", mobile_apis::HMILevel::HMI_BACKGROUND),
  std::make_pair("NONE", mobile_apis::HMILevel::HMI_NONE)
};

std::map<std::string, mobile_apis::HMILevel::eType> kHmiLevels(kHmiLevelsInitializer,
                                                               kHmiLevelsInitializer +
                                                               ARRAYSIZE(kHmiLevelsInitializer));
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

bool PoliciesManager::Init() {
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
      for (uint32_t j = 0; j < levels.size(); ++j) {
        hmi_levels.push_back(kHmiLevels[levels[j].asString()]);
      }

      items_.insert(PoliciesItem(kFunctionIDs[item.asString()], hmi_levels));
    }
  } catch (...) {
    return false;
  }

  return true;
}

bool PoliciesManager::IsValidHmiStatus(FunctionID::eType function,
                                          mobile_apis::HMILevel::eType status) {
  if (items_.find(function) == items_.end()) {
    return false;
  }

  HmiLevels levels = items_[function];
  if (std::find(levels.begin(), levels.end(), status) != levels.end()) {
    return true;
  }

  return false;
}

}  // namespace policies_manager
}  // namespace application_manager

