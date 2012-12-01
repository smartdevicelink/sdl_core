#include "../include/JSONHandler/ALRPCObjects/Slider_response.h"
#include "Slider_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Result_v2Marshaller.h"

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

Slider_response::~Slider_response(void)
{
}


Slider_response::Slider_response(const Slider_response& c)
{
  *this=c;
}


bool Slider_response::checkIntegrity(void)
{
  return Slider_responseMarshaller::checkIntegrity(*this);
}


Slider_response::Slider_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_SLIDER_RESPONSE)
{
}



bool Slider_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool Slider_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
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

const Result_v2& Slider_response::get_resultCode(void) const 
{
  return resultCode;
}

unsigned int Slider_response::get_sliderPosition(void) const
{
  return sliderPosition;
}

