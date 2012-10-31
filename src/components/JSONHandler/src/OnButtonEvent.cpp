#include "JSONHandler/OnButtonEvent.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnButtonEvent::OnButtonEvent()
:RPC2Notification( RPC2Marshaller::METHOD_ONBUTTONEVENT )
{}
  
OnButtonEvent::~OnButtonEvent()
{}

const ButtonName & OnButtonEvent::getName(void) const
{
    return mName;
}

const ButtonEventMode & OnButtonEvent::getMode(void) const
{
    return mMode;
}

bool OnButtonEvent::setName(const ButtonName & s)
{
    mName = s;
}
      
bool OnButtonEvent::setMode(const ButtonEventMode& s)
{
    mMode = s;
}