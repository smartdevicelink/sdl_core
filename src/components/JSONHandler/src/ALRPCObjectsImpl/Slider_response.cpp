#include "../../include/JSONHandler/ALRPCObjects/Slider_response.h"
#include "Slider_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

