#include "../include/JSONHandler/SDLRPCObjects/V2/DeleteInteractionChoiceSet_response.h"
#include "DeleteInteractionChoiceSet_responseMarshaller.h"

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

using namespace NsSmartDeviceLinkRPCV2;
DeleteInteractionChoiceSet_response& DeleteInteractionChoiceSet_response::operator =(
    const DeleteInteractionChoiceSet_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

DeleteInteractionChoiceSet_response::~DeleteInteractionChoiceSet_response(void)
{}

DeleteInteractionChoiceSet_response::DeleteInteractionChoiceSet_response(const DeleteInteractionChoiceSet_response& c)
{
    *this = c;
}

bool DeleteInteractionChoiceSet_response::checkIntegrity(void)
{
    return DeleteInteractionChoiceSet_responseMarshaller::checkIntegrity(*this);
}

DeleteInteractionChoiceSet_response::DeleteInteractionChoiceSet_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
{}
