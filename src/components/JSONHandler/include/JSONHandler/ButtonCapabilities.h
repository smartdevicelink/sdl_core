#ifndef BUTTON_CAPABILITIES_CLASS
#define BUTTON_CAPABILITIES_CLASS

#include "JSONHandler/ButtonName.hh"

class ButtonCapabilities
{
public:
    ButtonCapabilities() {}
    ~ButtonCapabilities() {}

    void setName(const ButtonName& name) { mName = name; }
    void setShortPressAvailable(bool s) { mShortPressAvailable = s; }
    void setLongPressAvailable(bool s) { mLongPressAvailable = s; }
    void setUpDownAvailable(bool s) { mUpDownAvailable = s; }

    ButtonName getButtonName() const { return mName; }
    bool getShortPressAvailable() const { return mShortPressAvailable; }
    bool getLongPressAvailable() const { return mLongPressAvailable; }
    bool getUpDownAvailable() const { return mUpDownAvailable; }

private:
    ButtonName mName;
    bool mShortPressAvailable;
    bool mLongPressAvailable;
    bool mUpDownAvailable;
    friend class ButtonCapabilitiesMarshaller;
};


#endif