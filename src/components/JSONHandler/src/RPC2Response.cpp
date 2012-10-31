#include "JSONHandler/RPC2Response.h"
#include <sstream>
#include <stdlib.h>

using namespace RPC2Communication;

RPC2Response::RPC2Response( )
:RPC2Command( )
{}

RPC2Response::RPC2Response( int method )
:RPC2Command( method )
{}

RPC2Response::~RPC2Response()
{}

void RPC2Response::setID( const std::string & id )
{
    mID = id;
}

void RPC2Response::setID( int id )
{
     std::stringstream stream;
     stream << id;
     mID = stream.str();
}

std::string RPC2Response::getIDString() const
{
    return mID;
}

int RPC2Response::getID() const
{
    if ( mID.empty() )
    {
        return 0;
    }

    return atoi(mID.c_str());
}

Result RPC2Response::getResult() const
{
    return mResultCode;
}

void RPC2Response::setResult(const Result& r)
{
    mResultCode = r;
}

RPC2Error RPC2Response::getError() const
{
    return mError;
}

void RPC2Response::setError(const RPC2Error& error)
{
    mError = error;
}