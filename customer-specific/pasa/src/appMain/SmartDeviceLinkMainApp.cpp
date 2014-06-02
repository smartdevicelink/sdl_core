// ----------------------------------------------------------------------------

#include "./life_cycle.h"
#include "SmartDeviceLinkMainApp.h"

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/system.h"
#include "utils/signals.h"
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

void startSmartDeviceLink()
{
    // --------------------------------------------------------------------------
    // Logger initialization

    INIT_LOGGER(profile::Profile::instance()->log4cxx_config_file());

    LOG4CXX_INFO(logger, " Application started!");

    // --------------------------------------------------------------------------
    // Components initialization
    profile::Profile::instance()->config_file_name(SDL_INIFILE_PATH);


    main_namespace::LifeCycle::instance()->StartComponents();

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

void stopSmartDeviceLinkOnSignal(int32_t params)
{
	LOG4CXX_INFO(logger, " Application stopped by signal!");
        main_namespace::LifeCycle::instance()->StopComponents();

	g_bTerminate = true;
}


/**
 * \brief Entry point of the program.
 * \param argc number of argument
 * \param argv array of arguments
 * \return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char** argv) {
    struct mq_attr attributes;
    attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
    attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
    attributes.mq_flags = 0;

    INIT_LOGGER(profile::Profile::instance()->log4cxx_config_file());

    LOG4CXX_INFO(logger, " Application main()");

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

	utils::SubscribeToTerminateSignal(&stopSmartDeviceLinkOnSignal);

	while (!g_bTerminate)
	{
		if ( (length = mq_receive(mq, buffer, sizeof(buffer), 0)) != -1)
		{
			switch (buffer[0])
			{
				case SDL_MSG_SDL_START:			 startSmartDeviceLink(); 	break;
				case SDL_MSG_SDL_STOP:			 stopSmartDeviceLink(); 	break;
				break;
				default:
				break;
			}
		}
	} //while-end

	return 0;
}


///EOF


