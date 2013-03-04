#include "../include/JSONHandler/ALRPCObjects/V1/ResetGlobalProperties_response.h"
#include "ResetGlobalProperties_responseMarshaller.h"
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
ResetGlobalProperties_response& ResetGlobalProperties_response::operator =(const ResetGlobalProperties_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

ResetGlobalProperties_response::~ResetGlobalProperties_response(void)
{}

ResetGlobalProperties_response::ResetGlobalProperties_response(const ResetGlobalProperties_response& c)
{
    *this = c;
}

bool ResetGlobalProperties_response::checkIntegrity(void)
{
    return ResetGlobalProperties_responseMarshaller::checkIntegrity(*this);
}

ResetGlobalProperties_response::ResetGlobalProperties_response(void)
    : ALRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_RESETGLOBALPROPERTIES_RESPONSE)
{}
