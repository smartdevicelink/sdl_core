#ifndef IAPPCOMMAND_H
#define IAPPCOMMAND_H

#include "RegistryItem.h"

namespace NsAppManager
{
	
class IAppCommand
{
public:
	IAppCommand(const RegistryItem& receiver);
	virtual ~IAppCommand();

	virtual void execute()=0;
	
protected:
	
	const RegistryItem& mReceiver;
};

}

#endif // IAPPCOMMAND_H
