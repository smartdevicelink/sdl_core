#include "AppMgr/IAppCommand.h"

namespace NsAppManager
{
	
IAppCommand::IAppCommand(const RegistryItem& receiver)
	:mReceiver(receiver)
{
}

IAppCommand::~IAppCommand()
{	
}

}
