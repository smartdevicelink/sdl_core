#include "JSONHandler/RPC2Command.h"

RPC2Command::RPC2Command( std::string methodName )
:mMethodName( methodName )
{}

RPC2Command::~RPC2Command() {}

void RPC2Command::setMethodName( std::string methodName )
{
    mMethodName = methodName;
}

std::string RPC2Command::getMethodName() const
{
    return mMethodName;
}