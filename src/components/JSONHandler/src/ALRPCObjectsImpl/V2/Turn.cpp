#include "../include/JSONHandler/ALRPCObjects/V2/Turn.h"
#include "TurnMarshaller.h"
#include "ImageMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

Turn& Turn::operator =(const Turn& c)
{
  navigationText=c.navigationText ? new std::string(c.navigationText[0]) : 0;
  turnIcon=c.turnIcon;

  return *this;
}


Turn::~Turn(void)
{
  if(navigationText)
    delete navigationText;
}


Turn::Turn(const Turn& c)
{
  *this=c;
}


bool Turn::checkIntegrity(void)
{
  return TurnMarshaller::checkIntegrity(*this);
}


Turn::Turn(void) :
    navigationText(0)
{
}



bool Turn::set_navigationText(const std::string& navigationText_)
{
  if(navigationText_.length()>500)  return false;
  delete navigationText;
  navigationText=0;

  navigationText=new std::string(navigationText_);
  return true;
}

void Turn::reset_navigationText(void)
{
  if(navigationText)
    delete navigationText;
  navigationText=0;
}

bool Turn::set_turnIcon(const Image& turnIcon_)
{
  if(!ImageMarshaller::checkIntegrityConst(turnIcon_))   return false;
  turnIcon=turnIcon_;
  return true;
}




const std::string* Turn::get_navigationText(void) const 
{
  return navigationText;
}


const Image& Turn::get_turnIcon(void) const 
{
  return turnIcon;
}


