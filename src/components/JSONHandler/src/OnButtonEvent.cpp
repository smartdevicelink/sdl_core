#include "JSONHandler/OnButtonEvent.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnButtonEvent::OnButtonEvent()
:RPC2Notification( RPC2Marshaller::METHOD_ONBUTTONEVENT )
{}
  
OnButtonEvent::~OnButtonEvent()
{}

const std::string & OnButtonEvent::getName(void) const
{
    return mName;
}

const ButtonEventMode & OnButtonEvent::getMode(void) const
{
    return mMode;
}

bool OnButtonEvent::setName(const std::string & s)
{
    mName = s;
}
      
bool OnButtonEvent::setMode(const ButtonEventMode& s)
{
    mMode = s;
}