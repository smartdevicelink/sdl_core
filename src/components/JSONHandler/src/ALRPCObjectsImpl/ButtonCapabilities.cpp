#include "../../include/JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "ButtonCapabilitiesMarshaller.h"
#include "ButtonNameMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



ButtonCapabilities::ButtonCapabilities(const ButtonCapabilities& c)
{
  *this=c;
}


bool ButtonCapabilities::checkIntegrity(void)
{
  return ButtonCapabilitiesMarshaller::checkIntegrity(*this);
}


ButtonCapabilities::ButtonCapabilities(void)
{
}



bool ButtonCapabilities::set_longPressAvailable(bool longPressAvailable_)
{
  longPressAvailable=longPressAvailable_;
  return true;
}

bool ButtonCapabilities::set_name(const ButtonName& name_)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(name_))   return false;
  name=name_;
  return true;
}

bool ButtonCapabilities::set_shortPressAvailable(bool shortPressAvailable_)
{
  shortPressAvailable=shortPressAvailable_;
  return true;
}

bool ButtonCapabilities::set_upDownAvailable(bool upDownAvailable_)
{
  upDownAvailable=upDownAvailable_;
  return true;
}




bool ButtonCapabilities::get_longPressAvailable(void) const
{
  return longPressAvailable;
}


const ButtonName& ButtonCapabilities::get_name(void) const 
{
  return name;
}


bool ButtonCapabilities::get_shortPressAvailable(void) const
{
  return shortPressAvailable;
}


bool ButtonCapabilities::get_upDownAvailable(void) const
{
  return upDownAvailable;
}


