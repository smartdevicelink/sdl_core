#include "../include/JSONHandler/ALRPCObjects/Slider_request.h"
#include "Slider_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

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
Slider_request& Slider_request::operator =(const Slider_request& c)
{
  numTicks= c.numTicks ? new unsigned int(c.numTicks[0]) : 0;
  position= c.position;
  sliderHeader= c.sliderHeader ? new std::string(c.sliderHeader[0]) : 0;
  sliderFooter= c.sliderFooter ? new std::vector<std::string>(c.sliderFooter[0]) : 0;
  timeout= c.timeout;

  return *this;}


Slider_request::~Slider_request(void)
{
  if(numTicks)
    delete numTicks;
  if(sliderHeader)
    delete sliderHeader;
  if(sliderFooter)
    delete sliderFooter;
}


Slider_request::Slider_request(const Slider_request& c)
{
  *this=c;
}


bool Slider_request::checkIntegrity(void)
{
  return Slider_requestMarshaller::checkIntegrity(*this);
}


Slider_request::Slider_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SLIDER_REQUEST),
      numTicks(0),
    sliderHeader(0),
    sliderFooter(0)
{
}



bool Slider_request::set_numTicks(unsigned int numTicks_)
{
  if(numTicks_>26)  return false;
  if(numTicks_<2)  return false;
  delete numTicks;
  numTicks=0;

  numTicks=new unsigned int(numTicks_);
  return true;
}

void Slider_request::reset_numTicks(void)
{
  if(numTicks)
    delete numTicks;
  numTicks=0;
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
  delete sliderHeader;
  sliderHeader=0;

  sliderHeader=new std::string(sliderHeader_);
  return true;
}

void Slider_request::reset_sliderHeader(void)
{
  if(sliderHeader)
    delete sliderHeader;
  sliderHeader=0;
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




const unsigned int* Slider_request::get_numTicks(void) const 
{
  return numTicks;
}

unsigned int Slider_request::get_position(void) const
{
  return position;
}

const std::string* Slider_request::get_sliderHeader(void) const 
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

