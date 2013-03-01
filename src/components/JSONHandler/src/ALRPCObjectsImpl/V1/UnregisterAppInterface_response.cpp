#include "../include/JSONHandler/ALRPCObjects/V1/UnregisterAppInterface_response.h"
#include "UnregisterAppInterface_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 1;
}

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    robok0der
*/

using namespace NsAppLinkRPC;
UnregisterAppInterface_response& UnregisterAppInterface_response::operator =(const UnregisterAppInterface_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

UnregisterAppInterface_response::~UnregisterAppInterface_response(void)
{}

UnregisterAppInterface_response::UnregisterAppInterface_response(const UnregisterAppInterface_response& c)
{
    *this = c;
}

bool UnregisterAppInterface_response::checkIntegrity(void)
{
    return UnregisterAppInterface_responseMarshaller::checkIntegrity(*this);
}

UnregisterAppInterface_response::UnregisterAppInterface_response(void)
    : ALRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_UNREGISTERAPPINTERFACE_RESPONSE)
{}
