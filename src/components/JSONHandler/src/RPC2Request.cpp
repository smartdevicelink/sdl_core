/**
* \file RPC2Request.cpp
* \brief RPC2Request class source file.
* Copyright (c) 2013 Ford Motor Company
*/


#include "JSONHandler/RPC2Request.h"

using namespace NsRPC2Communication;


RPC2Request::RPC2Request() :
RPC2Command(REQUEST)
, mId(0) 
{
}

RPC2Request::RPC2Request(int method) :
RPC2Command(REQUEST, method)
, mId(0) 
{
}

RPC2Request::RPC2Request(int method,unsigned int id) :
RPC2Command(REQUEST, method)
, mId(id) 
{
}

RPC2Request::~RPC2Request()
{
}

void RPC2Request::setId(unsigned int id)
{
  mId=id;
}

unsigned int RPC2Request::getId() const
{
  return mId;
}



