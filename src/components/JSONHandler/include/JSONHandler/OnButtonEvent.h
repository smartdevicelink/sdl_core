#ifndef ON_BUTTON_EVENT_CLASS
#define ON_BUTTON_EVENT_CLASS

#include "JSONHandler/ALRPCObjects/ButtonEventMode.h"
#include "JSONHandler/RPC2Notification.h"

namespace RPC2Communication
{
    class OnButtonEvent : public RPC2Notification
    {
    public:
        OnButtonEvent();
        virtual ~OnButtonEvent();

        const std::string & getName(void) const;
        const ButtonEventMode & getMode(void) const;

        bool setName(const std::string & s);
        bool setMode(const ButtonEventMode& s);
    
    private:
        std::string mName;
        ButtonEventMode mMode;

        friend class OnButtonEventMarshaller;
    };
}

#endif