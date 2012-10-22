#ifndef SUBSCRIBEBUTTON_H
#define SUBSCRIBEBUTTON_H

#include "AppMgr/IAppCommand.h"

#include <string>

namespace NsAppManager
{

struct ButtonParams
{
	std::string mName;
};

class SubscribeButtonCmd : public IAppCommand
{
public:
	virtual void execute();
};

};

#endif // SUBSCRIBEBUTTON_H
