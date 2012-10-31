#ifndef ONBUTTON_PRESS_CLASS
#define ONBUTTON_PRESS_CLASS

#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ALRPCObjects/ButtonPressMode.h"
#include "JSONHandler/ALRPCObjects/ButtonName.h"

namespace RPC2Communication
{
    class OnButtonPress : public RPC2Notification
    {
    public:
        OnButtonPress();
        ~OnButtonPress();

        ButtonName getName(void) const;
        ButtonPressMode getMode(void) const;

        bool setName(const ButtonName & s);
        bool setMode(const ButtonPressMode& mode);

    private:
        ButtonName mName;
        ButtonPressMode mMode;
    
        friend class OnButtonPressMarshaller;
    };
}

#endif