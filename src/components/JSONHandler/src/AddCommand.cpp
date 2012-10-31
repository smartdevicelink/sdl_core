#include "JSONHandler/AddCommand.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

AddCommand::AddCommand()
:RPC2Request( RPC2Marshaller :: METHOD_ADDCOMMAND_REQUEST )
{}
 
AddCommand::~AddCommand()
{}

void AddCommand::setCmdId( int cmdId )
{
    this->cmdId = cmdId;
}
        
void AddCommand::setMenuParams( const MenuParams& menuParams )
{
    this->menuParams = menuParams;
}

int AddCommand::getCmdId() const
{
    return cmdId;
}

const MenuParams& AddCommand::getMenuParams() const
{
    return menuParams;
}