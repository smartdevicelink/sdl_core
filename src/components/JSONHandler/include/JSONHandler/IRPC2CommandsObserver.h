#ifndef IRPC2COMMANDS_OBSERVER_CLASS
#define IRPC2COMMANDS_OBSERVER_CLASS

#include "JSONHandler/RPC2Command.h"

class IRPC2CommandsObserver
{
public:
    virtual void onCommandReceivedCallback( RPC2Communication::RPC2Command * command ) = 0;

protected:
    virtual ~IRPC2CommandsObserver(){};

};

#endif
