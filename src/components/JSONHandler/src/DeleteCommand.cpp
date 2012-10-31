#include "JSONHandler/DeleteCommand.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteCommand::DeleteCommand()
:RPC2Request( RPC2Marshaller::METHOD_DELETECOMMAND_REQUEST )
{}
 
DeleteCommand::~DeleteCommand()
{}

void DeleteCommand::setCmdId(int id)
{
    cmdId = id;
}
        
int DeleteCommand::getCmdId() const
{
    return cmdId;
}