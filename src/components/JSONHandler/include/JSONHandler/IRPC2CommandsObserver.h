#ifndef IRPC2COMMANDS_OBSERVER_CLASS
#define IRPC2COMMANDS_OBSERVER_CLASS

#include "JSONHandler/RPC2Command.h"

/**
 * \class IRPC2CommandsObserver
 * \brief Abstract class for communication with RPC2Bus.
 * Descendant class has to implement onCommandReceivedCallback
 * \sa onCommandReceivedCallback
*/
class IRPC2CommandsObserver
{
public:
    /**
     * \brief Callback function which is called by JSONRPC2Handler
     *  when new RPC2Bus Json message is received from HMI.
     * \param command RPC2Bus Json message
     */
    virtual void onCommandReceivedCallback( RPC2Communication::RPC2Command * command ) = 0;

protected:
    /**
     * \brief Destructor
    */
    virtual ~IRPC2CommandsObserver(){};

};

#endif
