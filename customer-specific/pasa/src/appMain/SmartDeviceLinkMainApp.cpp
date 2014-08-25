// ----------------------------------------------------------------------------

#include <algorithm>
#include <log4cxx/fileappender.h>

#include "./life_cycle.h"
#include "SmartDeviceLinkMainApp.h"
#include "signal_handlers.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/system.h"
#include "utils/signals.h"
#include "utils/file_system.h"
#include "config_profile/profile.h"

#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"
#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
// ----------------------------------------------------------------------------
// Third-Party includes

#include "CMessageBroker.hpp"
#include "mb_tcpserver.hpp"
#include "networking.h"  // cpplint: Include the directory when naming .h files
#include "system.h"      // cpplint: Include the directory when naming .h files
// ----------------------------------------------------------------------------

bool g_bTerminate = false;

namespace {
  const char kShellInterpreter[] = "sh";
  const char kPolicyInitializationScript[] = "/fs/mp/etc/AppLink/init_policy.sh";
}

// --------------------------------------------------------------------------
// Logger initialization

CREATE_LOGGERPTR_GLOBAL(logger, "SmartDeviceLinkMainApp")

bool remoteLoggingFlagFileExists(const std::string& name) {
	LOG4CXX_INFO(logger, "Check path: " << name);

	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

bool remoteLoggingFlagFileValid() {
	return true;
}

int getBootCount() {

    std::string fileContent;
    int bootCount;

    file_system::ReadFile(profile::Profile::instance()->target_boot_count_file(),
                        fileContent);

    std::stringstream(fileContent) >> bootCount;

    return bootCount;
}

void startSmartDeviceLink()
{
    LOG4CXX_INFO(logger, " Application started!");
    int bootCount = getBootCount();
    LOG4CXX_INFO(logger, "Boot count: " << getBootCount());

    // --------------------------------------------------------------------------
    // Components initialization
    profile::Profile::instance()->config_file_name(SDL_INIFILE_PATH);

    if (remoteLoggingFlagFileExists(
            profile::Profile::instance()->remote_logging_flag_file_path() +
            profile::Profile::instance()->remote_logging_flag_file()) &&
            remoteLoggingFlagFileValid()) {

      LOG4CXX_INFO(logger, "Enable logging to USB");

      std::string currentLogFilePath = profile::Profile::instance()->target_tmp_dir() +
                 "/" + profile::Profile::instance()->target_log_file_name_pattern();
      std::stringstream stream;
      stream << bootCount;
      std::string usbLogFilePath = profile::Profile::instance()->remote_logging_flag_file_path() +
                         stream.str() + "." +
                         profile::Profile::instance()->target_log_file_name_pattern();

      if (!currentLogFilePath.empty()) {
         LOG4CXX_INFO(logger, "Copy existing log info to USB from " << currentLogFilePath);

         std::vector<uint8_t> targetLogFileContent;

         file_system::ReadBinaryFile(currentLogFilePath, targetLogFileContent);

         file_system::WriteBinaryFile(usbLogFilePath, targetLogFileContent);
      }

      log4cxx::helpers::Pool p;

      std::string paramFileAppender = "LogFile";
      std::string paramConsoleAppender = "Console";

      std::string paramFileName = usbLogFilePath;

      LOG4CXX_DECODE_CHAR(logFileAppender, paramFileAppender);
      LOG4CXX_DECODE_CHAR(logConsoleAppender, paramConsoleAppender);
      LOG4CXX_DECODE_CHAR(logFileName, paramFileName );

      log4cxx::LoggerPtr rootLogger = logger->getLoggerRepository()->getRootLogger();

      log4cxx::FileAppenderPtr fileAppender = rootLogger->getAppender(logFileAppender);

      rootLogger->removeAppender(logConsoleAppender);

      if(fileAppender != NULL) {
        fileAppender->setAppend(true);
        fileAppender->setFile(logFileName);
        fileAppender->activateOptions(p);
      }
    }


    if (!main_namespace::LifeCycle::instance()->StartComponents()) {
      LOG4CXX_INFO(logger, "StartComponents failed.");
      exit(EXIT_FAILURE);
    }

    // --------------------------------------------------------------------------
    // Third-Party components initialization.

    if (!main_namespace::LifeCycle::instance()->InitMessageSystem()) {
      LOG4CXX_INFO(logger, "InitMessageBroker failed");
      exit(EXIT_FAILURE);
    }
    LOG4CXX_INFO(logger, "InitMessageBroker successful");

	// --------------------------------------------------------------------------
}



void stopSmartDeviceLink()
{
	LOG4CXX_INFO(logger, " Application stopped!");
        main_namespace::LifeCycle::instance()->StopComponents();

	g_bTerminate = true;
}

class ApplinkNotificationThreadDelegate : public threads::ThreadDelegate {
 public:
  virtual void threadMain();
};

void ApplinkNotificationThreadDelegate::threadMain() {
  struct mq_attr attributes;
  attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
  attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
  attributes.mq_flags = 0;

  mqd_t mq = mq_open(PREFIX_STR_SDL_PROXY_QUEUE, O_RDONLY | O_CREAT, 0666, &attributes);

  char buffer[MAX_QUEUE_MSG_SIZE];
  ssize_t length=0;

#if defined __QNX__
  // Policy initialization
  utils::System policy_init(kShellInterpreter);
  policy_init.Add(kPolicyInitializationScript);
  if (!policy_init.Execute(true)) {
      LOG4CXX_ERROR(logger, "QDB initialization failed.");
  }
#endif

  while (!g_bTerminate)
  {
      if ( (length = mq_receive(mq, buffer, sizeof(buffer), 0)) != -1)
      {
          switch (buffer[0])
          {
              case SDL_MSG_SDL_START:			 startSmartDeviceLink(); 	break;
              case SDL_MSG_SDL_STOP:			 stopSmartDeviceLink(); 	exit(0);
              default: break;
          }
      }
  } //while-end
}

/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv) {
  threads::Thread::MaskSignals();
  threads::Thread::SetMainThread();

  INIT_LOGGER(profile::Profile::instance()->log4cxx_config_file());

  LOG4CXX_INFO(logger, "Snapshot: {TAG}");
  LOG4CXX_INFO(logger, "Application main()");

  utils::SharedPtr<threads::Thread> applink_notification_thread = new threads::Thread("Applink notification thread", new ApplinkNotificationThreadDelegate());
  applink_notification_thread->start();

  utils::SubscribeToTerminateSignal(main_namespace::dummy_signal_handler);
  threads::Thread::UnmaskSignals();
  pause();

  LOG4CXX_INFO(logger, "Stopping application due to signal caught");
  stopSmartDeviceLink();

  return 0;
}


///EOF


