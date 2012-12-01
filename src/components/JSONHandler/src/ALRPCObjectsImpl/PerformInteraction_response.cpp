#include "../include/JSONHandler/ALRPCObjects/PerformInteraction_response.h"
#include "PerformInteraction_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"
#include "TriggerSourceMarshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
PerformInteraction_response& PerformInteraction_response::operator =(const PerformInteraction_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;
  choiceID= c.choiceID ? new unsigned int(c.choiceID[0]) : 0;
  triggerSource= c.triggerSource ? new TriggerSource(c.triggerSource[0]) : 0;

  return *this;}


PerformInteraction_response::~PerformInteraction_response(void)
{
  if(info)
    delete info;
  if(choiceID)
    delete choiceID;
  if(triggerSource)
    delete triggerSource;
}


PerformInteraction_response::PerformInteraction_response(const PerformInteraction_response& c)
{
  *this=c;
}


bool PerformInteraction_response::checkIntegrity(void)
{
  return PerformInteraction_responseMarshaller::checkIntegrity(*this);
}


PerformInteraction_response::PerformInteraction_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_PERFORMINTERACTION_RESPONSE),
      info(0),
    choiceID(0),
    triggerSource(0)
{
}



bool PerformInteraction_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool PerformInteraction_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool PerformInteraction_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void PerformInteraction_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}

bool PerformInteraction_response::set_choiceID(unsigned int choiceID_)
{
  if(choiceID_>2000000000)  return false;
  delete choiceID;
  choiceID=0;

  choiceID=new unsigned int(choiceID_);
  return true;
}

void PerformInteraction_response::reset_choiceID(void)
{
  if(choiceID)
    delete choiceID;
  choiceID=0;
}

bool PerformInteraction_response::set_triggerSource(const TriggerSource& triggerSource_)
{
  if(!TriggerSourceMarshaller::checkIntegrityConst(triggerSource_))   return false;
  delete triggerSource;
  triggerSource=0;

  triggerSource=new TriggerSource(triggerSource_);
  return true;
}

void PerformInteraction_response::reset_triggerSource(void)
{
  if(triggerSource)
    delete triggerSource;
  triggerSource=0;
}




bool PerformInteraction_response::get_success(void) const
{
  return success;
}

const Result& PerformInteraction_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* PerformInteraction_response::get_info(void) const 
{
  return info;
}

const unsigned int* PerformInteraction_response::get_choiceID(void) const 
{
  return choiceID;
}

const TriggerSource* PerformInteraction_response::get_triggerSource(void) const 
{
  return triggerSource;
}

