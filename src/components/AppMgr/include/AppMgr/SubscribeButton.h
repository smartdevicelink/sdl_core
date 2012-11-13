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

/**
 * \brief SubscribeButtonCmd acts as a concrete command implementation for button subscription
 */
class SubscribeButtonCmd : public IAppCommand
{
public:

    /**
     * \brief Class constructor
     * \param receiver a registry item associated with application that will execute command
     * \param params parameters to executor
     */
    SubscribeButtonCmd(const RegistryItem* receiver, const void* params);

    /**
     * \brief method to being called upon command execution, derived from IAppCommand
     */
	virtual void execute();
};

}

#endif // SUBSCRIBEBUTTON_H
