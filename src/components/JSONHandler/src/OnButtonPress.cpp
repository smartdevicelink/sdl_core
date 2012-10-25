#include "JSONHandler/OnButtonPress.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

OnButtonPress::OnButtonPress()
:RPC2Notification( RPC2Marshaller::METHOD_ONBUTTONPRESS )
{}
  
OnButtonPress::~OnButtonPress()
{}

ButtonName OnButtonPress::getName(void) const
{
    return mName;
}

ButtonPressMode OnButtonPress::getMode(void) const
{
    return mMode;
}

bool OnButtonPress::setName(const ButtonName & s)
{
    mName = s;
}
      
bool OnButtonPress::setMode(const ButtonPressMode& s)
{
    mMode = s;
}