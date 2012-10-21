#include "JSONHandler/RPC2Command.h"

using namespace RPC2Communication;

RPC2Command::RPC2Command( )
{}

RPC2Command::RPC2Command( int method )
:mMethod( method )
{}

RPC2Command::~RPC2Command()
{}

void RPC2Command::setMethod( int method )
{
    mMethod = method;
}

int RPC2Command::getMethod() const
{
    return mMethod;
}

void RPC2Command::setCommandType( RPC2Command::CommandType commandType )
{
    mCommandType = commandType;
}

RPC2Command::CommandType RPC2Command::getCommandType( ) const
{
    return mCommandType;
}