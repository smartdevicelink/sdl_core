/**
* \file RPC2Error.cpp
* \brief RPC2Error class source file.
* \author PVyshnevska
*/


#include "JSONHandler/RPC2Error.h"

using namespace NsRPC2Communication;

RPC2Error::RPC2Error(void) :
RPC2Command(ERROR)
, mId(0)
, mCode(0)
, mMessage("")
{
}

RPC2Error::RPC2Error(int code, const std::string& message,int id ) :
RPC2Command(ERROR)
, mId(id)
, mCode(code)
, mMessage(message)
{
}

RPC2Error::RPC2Error(int code, const std::string& message) :
RPC2Command(ERROR)
, mId(0)
, mCode(code)
, mMessage(message)
{
}

RPC2Error::~RPC2Error()
{
}

RPC2Error::RPC2Error(const RPC2Error& r) :
RPC2Command(ERROR)
{
  *this=r;
}

void RPC2Error::setId(int id)
{
  mId=id;
}

void RPC2Error::resetId(void)
{
  mId=0;
}

int RPC2Error::getId() const
{
  return mId;
}

int RPC2Error::getErrorCode() const
{
  return mCode;
}

void RPC2Error::setErrorCode(int code)
{
  mCode=code;
}


const std::string& RPC2Error::getErrorString() const
{
  return mMessage;
}

void RPC2Error::setErrorString(const std::string& message)
{
  mMessage=message;
}

