/**
* \file RPC2Command.cpp
* \brief RPC2Command class source file.
* Copyright (c) 2013 Ford Motor Company
*/


#include "JSONHandler/RPC2Command.h"

using namespace NsRPC2Communication;

RPC2Command::RPC2Command( ) :
mCommandType(UNDEFINED)
, mMethod(-1)
{
}

RPC2Command::RPC2Command( CommandType type ) :
mCommandType(type)
, mMethod(-1)
{
}

RPC2Command::RPC2Command( CommandType type, int method ) :
mCommandType(type)
, mMethod( method )
{}

RPC2Command::~RPC2Command()
{
}


void RPC2Command::setCommandType( CommandType commandType )
{
  mCommandType=commandType;
}

RPC2Command::CommandType RPC2Command::getCommandType( ) const
{
  return mCommandType;
}

int RPC2Command::getMethod() const
{
  return mMethod;
}

void RPC2Command::setMethod(int method)
{
  mMethod=method;
}
