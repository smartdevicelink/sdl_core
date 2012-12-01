#include "../include/JSONHandler/ALRPCObjects/PresetBankCapabilities.h"
#include "PresetBankCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

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


