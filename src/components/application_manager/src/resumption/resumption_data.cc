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

#include "application_manager/resumption/resumption_data.h"
#include "utils/logger.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/vehicle_info_data.h"

namespace resumption {

CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionData::ResumptionData():
  resumption_lock_(true) {
}

smart_objects::SmartObject ResumptionData::GetApplicationCommands(
    app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject commands_array(smart_objects::SmartType_Array);
  DCHECK_OR_RETURN(application, commands_array);
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return commands_array;
  }
  const DataAccessor<CommandsMap> accessor = application->commands_map();
  const CommandsMap& commands = accessor.GetData();
  CommandsMap::const_iterator it = commands.begin();
  for (int i = 0; it != commands.end(); ++it, ++i) {
    commands_array[i] = *(it->second);
  }
  return commands_array;
}

smart_objects::SmartObject ResumptionData::GetApplicationSubMenus(
    app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK(application.get());
  smart_objects::SmartObject submenues_array = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return submenues_array;
  }
  const DataAccessor<SubMenuMap> accessor = application->sub_menu_map();
  const SubMenuMap& sub_menus = accessor.GetData();
  SubMenuMap::const_iterator it = sub_menus.begin();
  for (int i = 0;it != sub_menus.end(); ++it, ++i) {
    submenues_array[i] = *(it->second);
  }
  return submenues_array;
}

smart_objects::SmartObject ResumptionData::GetApplicationInteractionChoiseSets(
      app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK(application.get());
  smart_objects::SmartObject interaction_choice_set_array =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  if (!application) {
     LOG4CXX_ERROR(logger_, "NULL Pointer App");
     return interaction_choice_set_array;
   }
  const DataAccessor<ChoiceSetMap> accessor = application->choice_set_map();
  const ChoiceSetMap& choices = accessor.GetData();
  ChoiceSetMap::const_iterator it = choices.begin();
  for (int i = 0; it != choices.end(); ++it, ++i) {
    interaction_choice_set_array[i] = *(it->second);
  }
  return interaction_choice_set_array;
}

smart_objects::SmartObject ResumptionData::GetApplicationGlobalProperties(
      app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK(application.get());
  smart_objects::SmartObject global_properties =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return global_properties;
  }

  global_properties[strings::help_prompt] =
      PointerToSmartObj(application->help_prompt());
  global_properties[strings::timeout_prompt] =
      PointerToSmartObj(application->timeout_prompt());
  global_properties[strings::vr_help] =
      PointerToSmartObj(application->vr_help());
  global_properties[strings::vr_help_title] =
      PointerToSmartObj(application->vr_help_title());
  global_properties[strings::vr_synonyms] =
      PointerToSmartObj(application->vr_synonyms());
  global_properties[strings::keyboard_properties] =
      PointerToSmartObj(application->keyboard_props());
  global_properties[strings::menu_title] =
        PointerToSmartObj(application->menu_title());
  global_properties[strings::menu_icon] =
          PointerToSmartObj(application->menu_icon());
  return global_properties;
}

smart_objects::SmartObject ResumptionData::GetApplicationSubscriptions(
        app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(application.get());
  smart_objects::SmartObject subscriptions =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return subscriptions;
  }
  LOG4CXX_DEBUG(logger_, "app_id:" << application->app_id());

  DataAccessor<ButtonSubscriptions> button_accessor =
      application->SubscribedButtons();

  const ButtonSubscriptions& button_subscriptions = button_accessor.GetData();

  LOG4CXX_DEBUG(logger_, "SubscribedButtons:" << button_subscriptions.size());
  Append(button_subscriptions.begin(), button_subscriptions.end(),
         strings::application_buttons, subscriptions);

  DataAccessor<VehicleInfoSubscriptions> vi_accessor =
      application->SubscribedIVI();

  const VehicleInfoSubscriptions& vi_subscription = vi_accessor.GetData();

  LOG4CXX_DEBUG(logger_, "SubscribedIVI:" << vi_subscription.size());
  Append(vi_subscription.begin(), vi_subscription.end(),
         strings::application_vehicle_info, subscriptions);
  return subscriptions;
}

smart_objects::SmartObject ResumptionData::GetApplicationFiles(
    app_mngr::ApplicationConstSharedPtr application) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(application.get());
  LOG4CXX_TRACE(logger_, "ENTER app_id:"
                << application->app_id());

  smart_objects::SmartObject files =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return files;
  }

  const AppFilesMap& app_files = application->getAppFiles();
  int i = 0;
  for(AppFilesMap::const_iterator file_it = app_files.begin();
      file_it != app_files.end(); file_it++) {
    const AppFile& file = file_it->second;
    if (file.is_persistent) {
      smart_objects::SmartObject file_data =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      file_data[strings::persistent_file] = file.is_persistent;
      file_data[strings::is_download_complete] = file.is_download_complete;
      file_data[strings::sync_file_name] = file.file_name;
      file_data[strings::file_type] = file.file_type;
      files[i++] = file_data;
    }
  }
  return files;
}

smart_objects::SmartObject ResumptionData::PointerToSmartObj(
    const smart_objects::SmartObject* ptr) const {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject temp;
  if (ptr != NULL) {
    temp = *ptr;
  }
  return temp;
}
}  // namespace resumption
