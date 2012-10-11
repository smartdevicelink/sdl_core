#include "JSONHandler/RPC2Request.h"
#include <sstream>

RPC2Request::RPC2Request( const std::string & methodName )
:RPC2Command( methodName )
{}

RPC2Request::~RPC2Request()
{}

void RPC2Request::setID( const std::string & id )
{
    mID = id;
}

void RPC2Request::setID( int id )
{
     std::stringstream stream;
     stream << id;
     mID = stream.str();
}

std::string RPC2Request::getIDString() const
{
    return mID;
}

int RPC2Request::getID() const
{
    if ( mID.empty() )
    {
        return 0;
    }

    return atoi(mID.c_str());
}