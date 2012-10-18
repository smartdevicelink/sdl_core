#ifndef IAPPCOMMAND_H
#define IAPPCOMMAND_H

#include "Application.h"

namespace NsAppManager
{
	
class IAppCommand
{
public:
	IAppCommand(Application* receiver);
	virtual ~IAppCommand();

	virtual void execute()=0;
	
protected:
	IAppCommand();
	
	Application* mReceiver;
};

}

#endif // IAPPCOMMAND_H
