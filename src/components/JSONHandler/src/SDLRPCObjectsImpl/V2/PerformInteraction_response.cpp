#include "../include/JSONHandler/SDLRPCObjects/V2/PerformInteraction_response.h"
#include "PerformInteraction_responseMarshaller.h"
#include "TriggerSourceMarshaller.h"

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
PerformInteraction_response& PerformInteraction_response::operator =(const PerformInteraction_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    choiceID = c.choiceID ? new unsigned int(c.choiceID[0]) : 0;
    triggerSource = c.triggerSource ? new TriggerSource(c.triggerSource[0]) : 0;

    return *this;
}


PerformInteraction_response::~PerformInteraction_response(void)
{
    if (choiceID)
    {
        delete choiceID;
    }
    if (triggerSource)
    {
        delete triggerSource;
    }
}

PerformInteraction_response::PerformInteraction_response(const PerformInteraction_response& c)
{
    *this = c;
}

bool PerformInteraction_response::checkIntegrity(void)
{
    return PerformInteraction_responseMarshaller::checkIntegrity(*this);
}

PerformInteraction_response::PerformInteraction_response(void)
    : NsSmartDeviceLinkRPC::SDLRPCResponse(PROTOCOL_VERSION)
    , choiceID(0)
    , triggerSource(0)
{
}

bool PerformInteraction_response::set_choiceID(unsigned int choiceID_)
{
    if (choiceID_ > 2000000000)
    {
        return false;
    }
    delete choiceID;
    choiceID = 0;

    choiceID = new unsigned int(choiceID_);
    return true;
}

void PerformInteraction_response::reset_choiceID(void)
{
    if (choiceID)
    {
        delete choiceID;
    }
    choiceID = 0;
}

bool PerformInteraction_response::set_triggerSource(const TriggerSource& triggerSource_)
{
    if (!TriggerSourceMarshaller::checkIntegrityConst(triggerSource_))
    {
        return false;
    }
    delete triggerSource;
    triggerSource = 0;

    triggerSource = new TriggerSource(triggerSource_);
    return true;
}

void PerformInteraction_response::reset_triggerSource(void)
{
    if (triggerSource)
    {
        delete triggerSource;
    }
    triggerSource = 0;
}

const unsigned int* PerformInteraction_response::get_choiceID(void) const
{
    return choiceID;
}

const TriggerSource* PerformInteraction_response::get_triggerSource(void) const
{
    return triggerSource;
}

