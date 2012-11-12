#include "AppMgr/IAppCommand.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
	
log4cplus::Logger IAppCommand::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCommands"));

/**
 * \brief Class constructor
 * \param receiver a registry item associated with application that will execute command
 */
IAppCommand::IAppCommand(const RegistryItem *receiver)
	:mReceiver(receiver)
	,mParams(NULL)
{
}

/**
 * \brief Copy constructor
 */
IAppCommand::IAppCommand(const IAppCommand &)
    :mReceiver(0)
    ,mParams(0)
{
}

/**
 * \brief Class constructor
 * \param receiver a registry item associated with application that will execute command
 * \param params command params
 */
IAppCommand::IAppCommand(const RegistryItem* receiver, const void* params)
	:mReceiver(receiver)
	,mParams(params)
{
}

/**
 * \brief Default destructor
 */
IAppCommand::~IAppCommand()
{	
    if(mParams)
    {
        delete mParams;
    }
}

}
