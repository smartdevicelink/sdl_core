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

namespace application_manager {
namespace resumption {

CREATE_LOGGERPTR_GLOBAL(logger_, "ResumptionData")

smart_objects::SmartObject ResumptionData::GetApplicationCommands(
    ApplicationConstSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK(application.get());
  smart_objects::SmartObject commands_array = smart_objects::SmartObject(
      smart_objects::SmartType_Array);
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
    ApplicationConstSharedPtr application) {
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
      ApplicationConstSharedPtr application) {
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
      ApplicationConstSharedPtr application) {
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
        ApplicationConstSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(application.get());
  smart_objects::SmartObject subscriptions =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL Pointer App");
    return subscriptions;
  }
  LOG4CXX_DEBUG(logger_, "app_id:" << application->app_id());
  LOG4CXX_DEBUG(logger_, "SubscribedButtons:"
                << application->SubscribedButtons().size());
  Append(application->SubscribedButtons().begin(),
         application->SubscribedButtons().end(),
         strings::application_buttons, subscriptions);
  LOG4CXX_DEBUG(logger_, "SubscribesIVI:"
                << application->SubscribesIVI().size());
  Append(application->SubscribesIVI().begin(),
         application->SubscribesIVI().end(),
         strings::application_vehicle_info, subscriptions);
  return subscriptions;
}

smart_objects::SmartObject ResumptionData::GetApplicationFiles(
    ApplicationConstSharedPtr application) {
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
      file_it != app_files.end(); file_it++, ++i) {
    const AppFile& file = file_it->second;
    if (file.is_persistent) {
      smart_objects::SmartObject file_data =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      file_data[strings::persistent_file] = file.is_persistent;
      file_data[strings::is_download_complete] = file.is_download_complete;
      file_data[strings::sync_file_name] = file.file_name;
      file_data[strings::file_type] = file.file_type;
      files[i] = file_data;
    }
  }
  return files;
}

void ResumptionData::AddFiles(ApplicationSharedPtr application,
                const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (saved_app.keyExists(strings::application_files)) {
    const smart_objects::SmartObject& application_files =
        saved_app[strings::application_files];
    for (int i = 0; i < application_files.length(); ++i) {
      const smart_objects::SmartObject& file_data =
          appliction_files[i];
      const bool is_persistent = file_data.keyExists(strings::persistent_file) &&
          file_data[strings::persistent_file].asBool();
      if (is_persistent) {
        AppFile file;
        file.is_persistent = is_persistent;
        file.is_download_complete =
            file_data[strings::is_download_complete].asBool();
        file.file_name = file_data[strings::sync_file_name].asString();
        file.file_type = static_cast<mobile_apis::FileType::eType> (
            file_data[strings::file_type].asInt());
        application->AddFile(file);
      }
    }
  } else {
    LOG4CXX_FATAL(logger_, "application_files section is not exists");
  }
}

void ResumptionData::AddSubmenues(ApplicationSharedPtr application,
                                  const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (saved_app.keyExists(strings::application_submenus)) {
    const smart_objects::SmartObject& app_submenus =
        saved_app[strings::application_submenus];
    for (int i = 0; i < app_submenus.length(); ++i) {
      const smart_objects::SmartObject& submenu = app_submenues[i];
      application->AddSubMenu(submenu[strings::menu_id].asUInt(), submenu);
    }
  } else {
    LOG4CXX_FATAL(logger_, "application_submenus section is not exists");
  }
}

void ResumptionData::AddCommands(ApplicationSharedPtr application,
                                 const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (saved_app.keyExists(strings::application_commands)) {
    const smart_objects::SmartObject& app_commands =
        saved_app[strings::application_commands];
    for (int i = 0; i < app_commands.length(); ++i) {
      const smart_objects::SmartObject& command =
          app_commands[i];
      application->AddCommand(command[strings::cmd_id].asUInt(), command);
    }
  } else {
    LOG4CXX_FATAL(logger_, "application_commands section is not exists");
  }
}

void ResumptionData::AddChoicesets(ApplicationSharedPtr application,
                                   const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (saved_app.keyExists(strings::application_choice_sets)) {
    const smart_objects::SmartObject& app_choice_sets =
        saved_app[strings::application_choice_sets];
    for (int i = 0; i < app_choice_sets.length(); ++i)  {
      const smart_objects::SmartObject& choice_set =
          app_choice_sets[i];
      const int32_t choice_set_id =
          choice_set[strings::interaction_choice_set_id].asInt();
      application->AddChoiceSet(choice_set_id, choice_set);
    }
  } else {
    LOG4CXX_FATAL(logger_, "There is no any choicesets");
  }
}

void ResumptionData::SetGlobalProperties(ApplicationSharedPtr application,
                           const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (saved_app.keyExists(strings::application_global_properties)) {
    const smart_objects::SmartObject& properties_so =
        saved_app[strings::application_global_properties];
    application->load_global_properties(properties_so);
  }
}

void ResumptionData::AddSubscriptions(ApplicationSharedPtr application,
                                      const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (saved_app.keyExists(strings::application_subscribtions)) {
    const smart_objects::SmartObject& subscribtions =
        saved_app[strings::application_subscribtions];

    if (subscribtions.keyExists(strings::application_buttons)) {
      const smart_objects::SmartObject& subscribtions_buttons =
          subscribtions[strings::application_buttons];
      mobile_apis::ButtonName::eType btn;
      for (int i = 0; i < subscribtions_buttons.length(); ++i) {
        btn = static_cast<mobile_apis::ButtonName::eType>(
            (subscribtions_buttons[i]).asInt());
        application->SubscribeToButton(btn);
      }
    }

    if (subscribtions.keyExists(strings::application_vehicle_info)) {
      const smart_objects::SmartObject& subscribtions_ivi =
          subscribtions[strings::application_vehicle_info];
      VehicleDataType ivi;
      for (int i = 0; i < subscribtions_ivi.length(); ++i) {
        ivi = static_cast<VehicleDataType>((subscribtions_ivi[i]).asInt());
        application->SubscribeToIVI(ivi);
      }
    }
  }
}

smart_objects::SmartObject ResumptionData::PointerToSmartObj(
    smart_objects::SmartObject* ptr) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject temp;
  if (ptr) {
    temp = *ptr;
  }
  return temp;
}

}  // namespace resumption
}  // namespace application_manager
