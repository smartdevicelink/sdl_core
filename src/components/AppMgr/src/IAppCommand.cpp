#include "AppMgr/IAppCommand.h"

namespace NsAppManager
{
	
IAppCommand::IAppCommand(Application* receiver)
{
	mReceiver = receiver;
}

IAppCommand::~IAppCommand()
{	
}

IAppCommand::IAppCommand()
	:mReceiver(0)
{
}

}
