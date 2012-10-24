#include "../../include/JSONHandler/ALRPCObjects/SoftButtonCapabilities.h"
#include "SoftButtonCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



SoftButtonCapabilities::SoftButtonCapabilities(const SoftButtonCapabilities& c)
{
  *this=c;
}


bool SoftButtonCapabilities::checkIntegrity(void)
{
  return SoftButtonCapabilitiesMarshaller::checkIntegrity(*this);
}


SoftButtonCapabilities::SoftButtonCapabilities(void)
{
}



bool SoftButtonCapabilities::set_imageSupported(bool imageSupported_)
{
  imageSupported=imageSupported_;
  return true;
}

bool SoftButtonCapabilities::set_longPressAvailable(bool longPressAvailable_)
{
  longPressAvailable=longPressAvailable_;
  return true;
}

bool SoftButtonCapabilities::set_shortPressAvailable(bool shortPressAvailable_)
{
  shortPressAvailable=shortPressAvailable_;
  return true;
}

bool SoftButtonCapabilities::set_upDownAvailable(bool upDownAvailable_)
{
  upDownAvailable=upDownAvailable_;
  return true;
}




bool SoftButtonCapabilities::get_imageSupported(void) const
{
  return imageSupported;
}


bool SoftButtonCapabilities::get_longPressAvailable(void) const
{
  return longPressAvailable;
}


bool SoftButtonCapabilities::get_shortPressAvailable(void) const
{
  return shortPressAvailable;
}


bool SoftButtonCapabilities::get_upDownAvailable(void) const
{
  return upDownAvailable;
}


