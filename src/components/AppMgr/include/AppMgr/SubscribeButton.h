#ifndef SUBSCRIBEBUTTON_H
#define SUBSCRIBEBUTTON_H

#include "AppMgr/IAppCommand.h"
#include "AppMgr/AppMgrCore.h"

#include <string>

class ButtonName;

namespace NsAppManager
{

struct ButtonParams
{
    Message mMessage;
};

class SubscribeButtonCmd : public IAppCommand
{
public:
    SubscribeButtonCmd(const RegistryItem* receiver, const void* params);
	virtual void execute();
};

};

#endif // SUBSCRIBEBUTTON_H
