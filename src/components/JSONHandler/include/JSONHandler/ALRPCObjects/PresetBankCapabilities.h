#ifndef NSAPPLINKRPC_PRESETBANKCAPABILITIES_INCLUDE
#define NSAPPLINKRPC_PRESETBANKCAPABILITIES_INCLUDE




/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/


namespace NsAppLinkRPC
{

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


///  Onscreen custom presets are available.
      bool onScreenPresetsAvailable;
  };

}

#endif
