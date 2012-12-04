#include "../include/JSONHandler/ALRPCObjects/V2/Slider_request.h"
#include "Slider_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
Slider_request& Slider_request::operator =(const Slider_request& c)
{
  numTicks= c.numTicks;
  position= c.position;
  sliderHeader= c.sliderHeader;
  sliderFooter= c.sliderFooter ? new std::vector<std::string>(c.sliderFooter[0]) : 0;
  timeout= c.timeout;

  return *this;
}


Slider_request::~Slider_request(void)
{
  if(sliderFooter)
    delete sliderFooter;
}


Slider_request::Slider_request(const Slider_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool Slider_request::checkIntegrity(void)
{
  return Slider_requestMarshaller::checkIntegrity(*this);
}


Slider_request::Slider_request(void) : ALRPC2Message(PROTOCOL_VERSION),
      sliderFooter(0)
{
}



bool Slider_request::set_numTicks(unsigned int numTicks_)
{
  if(numTicks_>26)  return false;
  if(numTicks_<2)  return false;
  numTicks=numTicks_;
  return true;
}

bool Slider_request::set_position(unsigned int position_)
{
  if(position_>26)  return false;
  if(position_<1)  return false;
  position=position_;
  return true;
}

bool Slider_request::set_sliderHeader(const std::string& sliderHeader_)
{
  if(sliderHeader_.length()>500)  return false;
  sliderHeader=sliderHeader_;
  return true;
}

bool Slider_request::set_sliderFooter(const std::vector<std::string>& sliderFooter_)
{
  unsigned int i=sliderFooter_.size();
  if(i>26 || i<1)  return false;
  while(i--)
  {
    if(sliderFooter_[i].length()>500)  return false;
  }
  delete sliderFooter;
  sliderFooter=0;

  sliderFooter=new std::vector<std::string>(sliderFooter_);
  return true;
}

void Slider_request::reset_sliderFooter(void)
{
  if(sliderFooter)
    delete sliderFooter;
  sliderFooter=0;
}

bool Slider_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>65535)  return false;
  timeout=timeout_;
  return true;
}




unsigned int Slider_request::get_numTicks(void) const
{
  return numTicks;
}

unsigned int Slider_request::get_position(void) const
{
  return position;
}

const std::string& Slider_request::get_sliderHeader(void) const 
{
  return sliderHeader;
}

const std::vector<std::string>* Slider_request::get_sliderFooter(void) const 
{
  return sliderFooter;
}

unsigned int Slider_request::get_timeout(void) const
{
  return timeout;
}

