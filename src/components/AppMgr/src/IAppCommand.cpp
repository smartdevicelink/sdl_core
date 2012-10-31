#include "AppMgr/IAppCommand.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
	
log4cplus::Logger IAppCommand::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCommands"));

IAppCommand::IAppCommand(const RegistryItem *receiver)
	:mReceiver(receiver)
	,mParams(NULL)
{
}

IAppCommand::IAppCommand(const IAppCommand &)
    :mReceiver(0)
    ,mParams(0)
{
}

IAppCommand::IAppCommand(const RegistryItem* receiver, const void* params)
	:mReceiver(receiver)
	,mParams(params)
{
}

IAppCommand::~IAppCommand()
{	
    if(mParams)
    {
        delete mParams;
    }
}

}
