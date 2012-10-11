#include "JSONHandler/RPC2Response.h"
#include <sstream>

RPC2Response::RPC2Response( const std::string & methodName )
:RPC2Command( methodName )
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