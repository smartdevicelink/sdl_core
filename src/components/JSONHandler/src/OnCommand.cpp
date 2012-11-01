#include "JSONHandler/OnCommand.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnCommand::OnCommand()
:RPC2Notification( RPC2Marshaller::METHOD_UIONCOMMAND_NOTIFICATION )
{}
 
OnCommand::~OnCommand()
{}

void OnCommand::setCommandId(int s)
{
    commandId = s;
}

int OnCommand::getCommandId() const
{
    return commandId;
}
