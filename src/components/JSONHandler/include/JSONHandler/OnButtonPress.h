#ifndef ONBUTTON_PRESS_CLASS
#define ONBUTTON_PRESS_CLASS

#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ButtonPressMode.hh"

namespace RPC2Communication
{
    class OnButtonPress : public RPC2Notification
    {
    public:
        OnButtonPress();
        ~OnButtonPress();

        std::string getName(void) const;
        ButtonPressMode getMode(void) const;

        bool setName(const std::string & s);
        bool setMode(const ButtonPressMode& mode);

    private:
        std::string mName;
        ButtonPressMode mMode;
    
        friend class OnButtonPressMarshaller;
    };
}

#endif