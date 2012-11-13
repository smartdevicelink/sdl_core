#ifndef IAPPCOMMAND_H
#define IAPPCOMMAND_H

#include "RegistryItem.h"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
	
/**
 * \brief IAppCommand acts as interface for concrete commands
 */
class IAppCommand
{
    friend class AppMgrCore;
public:

    /**
     * \brief Class constructor
     * \param receiver a registry item associated with application that will execute command
     * \param params command params
     */
    IAppCommand(const RegistryItem* receiver, const void* params);

    /**
     * \brief Default destructor
     */
	virtual ~IAppCommand();

    /**
     * \brief virtual method to being called upon command execution, must be overriden
     */
	virtual void execute()=0;
	
protected:

    /**
     * \brief Class constructor
     * \param receiver a registry item associated with application that will execute command
     */
    IAppCommand(const RegistryItem* receiver);

    const RegistryItem* mReceiver;
    const void* mParams;
    static log4cplus::Logger mLogger;

private:

    /**
     * \brief Copy constructor
     */
    IAppCommand(const IAppCommand&);
};

}

#endif // IAPPCOMMAND_H
