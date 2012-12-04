#include "../include/JSONHandler/ALRPCObjects/V2/PresetBankCapabilities.h"
#include "PresetBankCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

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


