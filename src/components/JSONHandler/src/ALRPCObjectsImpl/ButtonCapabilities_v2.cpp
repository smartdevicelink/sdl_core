#include "../include/JSONHandler/ALRPCObjects/ButtonCapabilities_v2.h"
#include "ButtonCapabilities_v2Marshaller.h"
#include "ButtonName_v2Marshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

ButtonCapabilities_v2::ButtonCapabilities_v2(const ButtonCapabilities_v2& c)
{
  *this=c;
}


bool ButtonCapabilities_v2::checkIntegrity(void)
{
  return ButtonCapabilities_v2Marshaller::checkIntegrity(*this);
}


ButtonCapabilities_v2::ButtonCapabilities_v2(void)
{
}



bool ButtonCapabilities_v2::set_longPressAvailable(bool longPressAvailable_)
{
  longPressAvailable=longPressAvailable_;
  return true;
}

bool ButtonCapabilities_v2::set_name(const ButtonName_v2& name_)
{
  if(!ButtonName_v2Marshaller::checkIntegrityConst(name_))   return false;
  name=name_;
  return true;
}

bool ButtonCapabilities_v2::set_shortPressAvailable(bool shortPressAvailable_)
{
  shortPressAvailable=shortPressAvailable_;
  return true;
}

bool ButtonCapabilities_v2::set_upDownAvailable(bool upDownAvailable_)
{
  upDownAvailable=upDownAvailable_;
  return true;
}




bool ButtonCapabilities_v2::get_longPressAvailable(void) const
{
  return longPressAvailable;
}


const ButtonName_v2& ButtonCapabilities_v2::get_name(void) const 
{
  return name;
}


bool ButtonCapabilities_v2::get_shortPressAvailable(void) const
{
  return shortPressAvailable;
}


bool ButtonCapabilities_v2::get_upDownAvailable(void) const
{
  return upDownAvailable;
}


