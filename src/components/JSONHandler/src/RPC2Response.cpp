/**
* \file RPC2Response.cpp
* \brief RPC2Response class source file.
* \author PVyshnevska
*/


#include "JSONHandler/RPC2Response.h"

using namespace RPC2Communication;


RPC2Response::RPC2Response() :
RPC2Command(RESPONSE)
, mId(0)
, mResultCode(-1)
{
}

RPC2Response::RPC2Response(int method) :
RPC2Command(RESPONSE, method)
, mId(0)
, mResultCode(-1)
{
}


RPC2Response::RPC2Response(int method,unsigned int id) :
RPC2Command(RESPONSE, method)
, mId(id)
, mResultCode(-1)
{
}


RPC2Response::RPC2Response(int method,unsigned int id,int res) :
RPC2Command(RESPONSE, method)
, mId(id)
, mResultCode(res)
{
}


RPC2Response::~RPC2Response()
{
}

void RPC2Response::setId(unsigned int id)
{
  mId=id;
}

unsigned int RPC2Response::getId() const
{
  return mId;
}


int RPC2Response::getResult() const
{
  return mResultCode;
}

void RPC2Response::setResult(int r)
{
  mResultCode=r;
}
