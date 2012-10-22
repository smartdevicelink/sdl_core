#ifndef BUTTON_EVENT_MODE_CLASS
#define BUTTON_EVENT_MODE_CLASS 

class ButtonEventMode
{
public:
    enum ButtonEventModeInternal
    {
        INVALID_ENUM = -1,
        BUTTONUP = 0,
        BUTTONDOWN = 1
    };

    ButtonEventMode() : mInternal( INVALID_ENUM ) {}
    ButtonEventMode(ButtonEventModeInternal e) : mInternal(e) {}
    ~ButtonEventMode() {}

    ButtonEventModeInternal get(void) const { return mInternal; }
    void set(ButtonEventModeInternal e) { mInternal = e; }

private:
    ButtonEventModeInternal mInternal;
    friend class ButtonEventModeMarshaller;
    
};

#endif