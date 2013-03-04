#include "../include/JSONHandler/ALRPCObjects/V2/PutFile_response.h"
#include "PutFile_responseMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 2;
}

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

using namespace NsAppLinkRPCV2;
PutFile_response& PutFile_response::operator =(const PutFile_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    spaceAvailable = c.spaceAvailable;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}


PutFile_response::~PutFile_response(void)
{}

PutFile_response::PutFile_response(const PutFile_response& c)
{
    *this = c;
}

bool PutFile_response::checkIntegrity(void)
{
    return PutFile_responseMarshaller::checkIntegrity(*this);
}

PutFile_response::PutFile_response(void) : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}

bool PutFile_response::set_spaceAvailable(unsigned int spaceAvailable_)
{
    if (spaceAvailable_ > 2000000000)
    {
        return false;
    }
    spaceAvailable = spaceAvailable_;
    return true;
}

unsigned int PutFile_response::get_spaceAvailable(void) const
{
    return spaceAvailable;
}
