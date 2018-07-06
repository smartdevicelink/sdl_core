#include <vector>
#include <string>
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "utils/logger.h"
#include "utils/file_system.h"

namespace {
using namespace application_manager;
void DeleteWayPoints(ApplicationSharedPtr app,
                     ApplicationManager& app_manager) {
  app_manager.UnsubscribeAppFromWayPoints(app);
  if (!app_manager.IsAnyAppSubscribedForWayPoints()) {
    MessageHelper::SendUnsubscribedWayPoints(app_manager);
  }
}

void DeleteCommands(ApplicationSharedPtr app, ApplicationManager& app_manager) {
  CommandsMap cmap = app->commands_map().GetData();

  for (auto cmd : cmap) {
    MessageHelper::SendDeleteCommandRequest(cmd.second, app, app_manager);
    app->RemoveCommand(cmd.first);
    app->help_prompt_manager().OnVrCommandDeleted(cmd.first, true);
  }
}

void DeleteSubmenus(ApplicationSharedPtr app, ApplicationManager& app_manager) {
  SubMenuMap smap = app->sub_menu_map().GetData();

  for (auto smenu : smap) {
    MessageHelper::SendDeleteSubmenuRequest(smenu.second, app, app_manager);
    app->RemoveSubMenu(smenu.first);
  }
}

void DeleteChoiceSets(ApplicationSharedPtr app,
                      ApplicationManager& app_manager) {
  ChoiceSetMap csmap = app->choice_set_map().GetData();

  for (auto choice : csmap) {
    MessageHelper::SendDeleteChoiceSetRequest(choice.second, app, app_manager);
    app->RemoveChoiceSet(choice.first);
  }
}

void DeleteGlobalProperties(ApplicationSharedPtr app,
                            ApplicationManager& app_manager) {
  using namespace smart_objects;

  const std::vector<std::string>& timeout_prompt =
      app_manager.get_settings().time_out_promt();

  SmartObject so_default_timeout_prompt = SmartObject(SmartType_Array);

  int32_t index = 0;
  for (auto prompt : timeout_prompt) {
    SmartObject timeoutPrompt = SmartObject(SmartType_Map);
    timeoutPrompt[strings::text] = timeout_prompt[static_cast<size_t>(index)];
    timeoutPrompt[strings::type] = hmi_apis::Common_SpeechCapabilities::SC_TEXT;
    so_default_timeout_prompt[index] = timeoutPrompt;
    ++index;
  }

  app->set_timeout_prompt(so_default_timeout_prompt);

  SmartObject empty_so = SmartObject(SmartType_Array);
  app->set_help_prompt(empty_so);
  app->reset_vr_help_title();
  app->reset_vr_help();
  app->set_keyboard_props(empty_so);
  app->set_menu_icon(empty_so);
  app->set_menu_title(empty_so);

  MessageHelper::SendResetPropertiesRequest(app, app_manager);
}

void DeleteButtonSubscriptions(ApplicationSharedPtr app,
                               ApplicationManager& app_manager) {
  ButtonSubscriptions buttons = app->SubscribedButtons().GetData();

  for (auto button : buttons) {
    if (mobile_apis::ButtonName::CUSTOM_BUTTON == button) {
      continue;
    }
    MessageHelper::SendUnsubscribeButtonNotification(button, app, app_manager);
    app->UnsubscribeFromButton(button);
  }
}

void CleanupAppFiles(ApplicationSharedPtr app) {
  const auto icon_file = app->app_icon_path();

  auto files = app->getAppFiles();
  for (auto file : files) {
    auto file_name = file.first;
    if (icon_file == file_name) {
      continue;
    }
    app->DeleteFile(file_name);
    file_system::DeleteFile(file_name);
  }
}
}  // namespace

namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger, "ApplicationManager")

void DeleteApplicationData(ApplicationSharedPtr app,
                           ApplicationManager& app_manager) {
  LOG4CXX_AUTO_TRACE(logger);
  DCHECK_OR_RETURN_VOID(app);

  DeleteWayPoints(app, app_manager);
  DeleteCommands(app, app_manager);
  DeleteSubmenus(app, app_manager);
  DeleteChoiceSets(app, app_manager);
  DeleteGlobalProperties(app, app_manager);
  DeleteButtonSubscriptions(app, app_manager);
  CleanupAppFiles(app);
  app_manager.GetPluginManager().ForEachPlugin(
      [&app](plugin_manager::RPCPlugin& plugin) {
        plugin.OnApplicationEvent(
            plugin_manager::ApplicationEvent::kDeleteApplicationData, app);
      });
}

}  // namespace application_manager
