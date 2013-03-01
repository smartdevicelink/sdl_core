#include "../include/JSONHandler/ALRPCObjects/V1/GenericResponse_response.h"
#include "GenericResponse_responseMarshaller.h"
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
GenericResponse_response& GenericResponse_response::operator =(const GenericResponse_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

GenericResponse_response::~GenericResponse_response(void)
{}

GenericResponse_response::GenericResponse_response(const GenericResponse_response& c)
{
    *this = c;
}

bool GenericResponse_response::checkIntegrity(void)
{
    return GenericResponse_responseMarshaller::checkIntegrity(*this);
}

GenericResponse_response::GenericResponse_response(void)
    : ALRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_GENERICRESPONSE_RESPONSE)
{}
