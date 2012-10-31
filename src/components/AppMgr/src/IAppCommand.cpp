#include "AppMgr/IAppCommand.h"

namespace NsAppManager
{
	
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
