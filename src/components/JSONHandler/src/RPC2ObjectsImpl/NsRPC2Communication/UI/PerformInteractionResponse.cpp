#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 07:25:04 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


PerformInteractionResponse& PerformInteractionResponse::operator =(const PerformInteractionResponse& c)
{
  if(choiceID)  delete choiceID;
  choiceID= c.choiceID ? new unsigned int(c.choiceID[0]) : 0;
  if(triggerSource)  delete triggerSource;
  triggerSource= c.triggerSource ? new NsAppLinkRPC::TriggerSource(c.triggerSource[0]) : 0;
  return *this;
}


PerformInteractionResponse::~PerformInteractionResponse(void)
{
  if(choiceID)  delete choiceID;
  if(triggerSource)  delete triggerSource;
}


PerformInteractionResponse::PerformInteractionResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE),
  choiceID(0),
  triggerSource(0)
{
}


PerformInteractionResponse::PerformInteractionResponse(const PerformInteractionResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const unsigned int* PerformInteractionResponse::get_choiceID(void)
{
  return choiceID;
}

bool PerformInteractionResponse::set_choiceID(const unsigned int& choiceID_)
{
  if(choiceID)  delete choiceID;
  choiceID=new unsigned int(choiceID_);
  return true;
}

void PerformInteractionResponse::reset_choiceID(void)
{
  if(choiceID)  delete choiceID;
  choiceID=0;
}

const NsAppLinkRPC::TriggerSource* PerformInteractionResponse::get_triggerSource(void)
{
  return triggerSource;
}

bool PerformInteractionResponse::set_triggerSource(const NsAppLinkRPC::TriggerSource& triggerSource_)
{
  if(triggerSource)  delete triggerSource;
  triggerSource=new NsAppLinkRPC::TriggerSource(triggerSource_);
  return true;
}

void PerformInteractionResponse::reset_triggerSource(void)
{
  if(triggerSource)  delete triggerSource;
  triggerSource=0;
}

bool PerformInteractionResponse::checkIntegrity(void)
{
  return PerformInteractionResponseMarshaller::checkIntegrity(*this);
}
