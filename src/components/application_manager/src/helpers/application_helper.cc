#include "application_manager/helpers/application_helper.h"
#include <string>
#include <vector>
#include "application_manager/message_helper.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace {
using namespace application_manager;
void DeleteWayPoints(ApplicationSharedPtr app,
                     ApplicationManager& app_manager) {
  std::set<uint32_t> subscribed_apps =
      app_manager.GetAppsSubscribedForWayPoints();
  bool send_unsubscribe =
      subscribed_apps.size() <= 1 && app_manager.IsSubscribedToHMIWayPoints();
  if (send_unsubscribe) {
    auto request = MessageHelper::CreateUnsubscribeWayPointsRequest(
        app_manager.GetNextHMICorrelationID());
    app_manager.GetRPCService().ManageHMICommand(request);
  }
  app_manager.UnsubscribeAppFromWayPoints(app, send_unsubscribe);
}

void DeleteCommands(ApplicationSharedPtr app, ApplicationManager& app_manager) {
  auto accessor = app->commands_map();
  const auto commands_map = accessor.GetData();

  for (const auto cmd : commands_map) {
    auto delete_UI_msg = MessageHelper::CreateDeleteUICommandRequest(
        cmd.second, app->app_id(), app_manager.GetNextHMICorrelationID());
    app_manager.GetRPCService().ManageHMICommand(delete_UI_msg);

    auto delete_VR_msg = MessageHelper::CreateDeleteVRCommandRequest(
        cmd.second, app, app_manager.GetNextHMICorrelationID());
    app_manager.GetRPCService().ManageHMICommand(delete_VR_msg);

    app->RemoveCommand(cmd.first);
    app->help_prompt_manager().OnVrCommandDeleted(cmd.first, true);
  }
}

void DeleteSubmenus(ApplicationSharedPtr app, ApplicationManager& app_manager) {
  auto accessor = app->sub_menu_map();
  const auto sub_menu_map = accessor.GetData();

  for (const auto smenu : sub_menu_map) {
    MessageHelper::SendDeleteSubmenuRequest(smenu.second, app, app_manager);
    app->RemoveSubMenu(smenu.first);
  }
}

void DeleteChoiceSets(ApplicationSharedPtr app,
                      ApplicationManager& app_manager) {
  auto accessor = app->choice_set_map();
  const auto choices = accessor.GetData();

  for (const auto choice : choices) {
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

SDL_CREATE_LOG_VARIABLE("ApplicationManager")

void DeleteApplicationData(ApplicationSharedPtr app,
                           ApplicationManager& app_manager) {
  SDL_LOG_AUTO_TRACE();
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
