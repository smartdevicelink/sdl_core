#include "JSONHandler/RPC2Error.h"

using namespace RPC2Communication;

RPC2Error::RPC2Error()
{}
 
RPC2Error::~RPC2Error()
{}

int RPC2Error::getCode( ) const
{
    return mCode;
}
    
std::string RPC2Error::getMessage() const
{
    return mMessage;
}

void RPC2Error::setCode( int code )
{
    mCode = code;
}

void RPC2Error::setMessage( const std::string & message )
{
    mMessage = message;
}