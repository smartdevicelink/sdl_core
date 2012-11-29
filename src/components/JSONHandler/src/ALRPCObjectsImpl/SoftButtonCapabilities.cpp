#include "../include/JSONHandler/ALRPCObjects/SoftButtonCapabilities.h"
#include "SoftButtonCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

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


