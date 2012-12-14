#ifndef NSAPPLINKRPCV2_PRESETBANKCAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_PRESETBANKCAPABILITIES_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
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
