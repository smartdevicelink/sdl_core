#include "../../include/JSONHandler/ALRPCObjects/PresetBankCapabilities.h"
#include "PresetBankCapabilitiesMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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


