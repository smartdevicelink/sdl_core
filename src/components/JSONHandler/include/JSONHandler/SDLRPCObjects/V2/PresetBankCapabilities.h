#ifndef NSAPPLINKRPCV2_PRESETBANKCAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_PRESETBANKCAPABILITIES_INCLUDE




/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsSmartDeviceLinkRPCV2
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
