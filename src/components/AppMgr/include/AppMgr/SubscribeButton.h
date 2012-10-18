#ifndef SUBSCRIBEBUTTON_H
#define SUBSCRIBEBUTTON_H

#include "AppMgr/IAppCommand.h"

namespace NsAppManager
{
	
class SubscribeButton : public IAppCommand
{
public:
	virtual void execute();
};

}

#endif // SUBSCRIBEBUTTON_H
