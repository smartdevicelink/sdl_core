#ifndef IAPPCOMMAND_H
#define IAPPCOMMAND_H

#include "RegistryItem.h"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
	
class IAppCommand
{
    friend class AppMgrCore;
public:
    IAppCommand(const RegistryItem* receiver, const void* params);
	virtual ~IAppCommand();

	virtual void execute()=0;
	
protected:
    IAppCommand(const RegistryItem* receiver);
    const RegistryItem* mReceiver;
	const void* mParams;
    static log4cplus::Logger mLogger;

private:
    IAppCommand(const IAppCommand&);
};

}

#endif // IAPPCOMMAND_H
