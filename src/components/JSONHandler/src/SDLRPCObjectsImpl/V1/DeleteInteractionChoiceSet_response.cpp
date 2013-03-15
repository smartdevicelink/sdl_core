//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteInteractionChoiceSet_response.h"
#include "DeleteInteractionChoiceSet_responseMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"

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
  author    RC
*/

using namespace NsSmartDeviceLinkRPC;
DeleteInteractionChoiceSet_response& DeleteInteractionChoiceSet_response::operator =(
    const DeleteInteractionChoiceSet_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}


DeleteInteractionChoiceSet_response::~DeleteInteractionChoiceSet_response(void)
{
    if (info)
    {
        delete info;
    }
}


DeleteInteractionChoiceSet_response::DeleteInteractionChoiceSet_response(const DeleteInteractionChoiceSet_response& c)
{
    *this = c;
}


bool DeleteInteractionChoiceSet_response::checkIntegrity(void)
{
    return DeleteInteractionChoiceSet_responseMarshaller::checkIntegrity(*this);
}


DeleteInteractionChoiceSet_response::DeleteInteractionChoiceSet_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_DELETEINTERACTIONCHOICESET_RESPONSE)
{}
