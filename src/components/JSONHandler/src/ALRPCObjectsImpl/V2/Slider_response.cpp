#include "../include/JSONHandler/ALRPCObjects/V2/Slider_response.h"
#include "Slider_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

Slider_response::~Slider_response(void)
{
}


Slider_response::Slider_response(const Slider_response& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool Slider_response::checkIntegrity(void)
{
  return Slider_responseMarshaller::checkIntegrity(*this);
}


Slider_response::Slider_response(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool Slider_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool Slider_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool Slider_response::set_sliderPosition(unsigned int sliderPosition_)
{
  if(sliderPosition_>26)  return false;
  if(sliderPosition_<1)  return false;
  sliderPosition=sliderPosition_;
  return true;
}




bool Slider_response::get_success(void) const
{
  return success;
}

const Result& Slider_response::get_resultCode(void) const 
{
  return resultCode;
}

unsigned int Slider_response::get_sliderPosition(void) const
{
  return sliderPosition;
}

