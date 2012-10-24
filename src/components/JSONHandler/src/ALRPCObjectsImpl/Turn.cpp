#include "../../include/JSONHandler/ALRPCObjects/Turn.h"
#include "TurnMarshaller.h"
#include "ImageMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



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


