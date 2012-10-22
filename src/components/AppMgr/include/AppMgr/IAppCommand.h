#ifndef IAPPCOMMAND_H
#define IAPPCOMMAND_H

#include "RegistryItem.h"

namespace NsAppManager
{
	
class IAppCommand
{
public:
	IAppCommand(const RegistryItem& receiver, const void* params);
	virtual ~IAppCommand();

	virtual void execute()=0;
	
protected:
	IAppCommand(const RegistryItem& receiver);
	const RegistryItem& mReceiver;
	const void* mParams;
};

}

#endif // IAPPCOMMAND_H
