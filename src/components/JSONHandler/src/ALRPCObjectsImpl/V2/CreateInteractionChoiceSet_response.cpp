#include "../include/JSONHandler/ALRPCObjects/V2/CreateInteractionChoiceSet_response.h"
#include "CreateInteractionChoiceSet_responseMarshaller.h"

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
CreateInteractionChoiceSet_response& CreateInteractionChoiceSet_response::operator =(
    const CreateInteractionChoiceSet_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;

    return *this;
}

CreateInteractionChoiceSet_response::~CreateInteractionChoiceSet_response(void)
{}

CreateInteractionChoiceSet_response::CreateInteractionChoiceSet_response(
    const CreateInteractionChoiceSet_response& c)
{
    *this = c;
}

bool CreateInteractionChoiceSet_response::checkIntegrity(void)
{
    return CreateInteractionChoiceSet_responseMarshaller::checkIntegrity(*this);
}

CreateInteractionChoiceSet_response::CreateInteractionChoiceSet_response(void)
    : NsAppLinkRPC::ALRPCResponse(PROTOCOL_VERSION)
{}
