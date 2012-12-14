#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Slider.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


Slider& Slider::operator =(const Slider& c)
{
  numTicks=c.numTicks;
  position=c.position;
  sliderHeader=c.sliderHeader;
  if(sliderFooter)  delete sliderFooter;
  sliderFooter= c.sliderFooter ? new std::vector<std::string>(c.sliderFooter[0]) : 0;
  timeout=c.timeout;
  appId=c.appId;
  return *this;
}


Slider::~Slider(void)
{
  if(sliderFooter)  delete sliderFooter;
}


Slider::Slider(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SLIDER),
  sliderFooter(0)
{
}


Slider::Slider(const Slider& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SLIDER,c.getId())
{
  *this=c;
}


unsigned int Slider::get_numTicks(void)
{
  return numTicks;
}

bool Slider::set_numTicks(unsigned int numTicks_)
{
  numTicks=numTicks_;
  return true;
}

unsigned int Slider::get_position(void)
{
  return position;
}

bool Slider::set_position(unsigned int position_)
{
  position=position_;
  return true;
}

const std::string& Slider::get_sliderHeader(void)
{
  return sliderHeader;
}

bool Slider::set_sliderHeader(const std::string& sliderHeader_)
{
  sliderHeader=sliderHeader_;
  return true;
}

const std::vector< std::string>* Slider::get_sliderFooter(void)
{
  return sliderFooter;
}

bool Slider::set_sliderFooter(const std::vector< std::string>& sliderFooter_)
{
  if(sliderFooter)  delete sliderFooter;
  sliderFooter=new std::vector< std::string>(sliderFooter_);
  return true;
}

void Slider::reset_sliderFooter(void)
{
  if(sliderFooter)  delete sliderFooter;
  sliderFooter=0;
}

unsigned int Slider::get_timeout(void)
{
  return timeout;
}

bool Slider::set_timeout(unsigned int timeout_)
{
  timeout=timeout_;
  return true;
}

int Slider::get_appId(void)
{
  return appId;
}

bool Slider::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool Slider::checkIntegrity(void)
{
  return SliderMarshaller::checkIntegrity(*this);
}
