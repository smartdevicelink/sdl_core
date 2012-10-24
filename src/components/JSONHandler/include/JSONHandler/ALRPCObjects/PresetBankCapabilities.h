#ifndef PRESETBANKCAPABILITIES_INCLUDE
#define PRESETBANKCAPABILITIES_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



///  Contains information about on-screen preset capabilities.

class PresetBankCapabilities
{
public:

  PresetBankCapabilities(const PresetBankCapabilities& c);
  PresetBankCapabilities(void);

  bool checkIntegrity(void);
// getters

  bool get_onScreenPresetsAvailable(void) const;

// setters

  bool set_onScreenPresetsAvailable(bool onScreenPresetsAvailable_);

private:

  friend class PresetBankCapabilitiesMarshaller;

  bool onScreenPresetsAvailable;
};

#endif
