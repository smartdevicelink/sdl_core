#include "../include/JSONHandler/SDLRPCObjects/V2/PresetBankCapabilities.h"
#include "PresetBankCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/



using namespace NsSmartDeviceLinkRPCV2;

PresetBankCapabilities::PresetBankCapabilities(const PresetBankCapabilities& c)
{
  *this=c;
}


bool PresetBankCapabilities::checkIntegrity(void)
{
  return PresetBankCapabilitiesMarshaller::checkIntegrity(*this);
}


PresetBankCapabilities::PresetBankCapabilities(void)
{
}



bool PresetBankCapabilities::set_onScreenPresetsAvailable(bool onScreenPresetsAvailable_)
{
  onScreenPresetsAvailable=onScreenPresetsAvailable_;
  return true;
}




bool PresetBankCapabilities::get_onScreenPresetsAvailable(void) const
{
  return onScreenPresetsAvailable;
}


