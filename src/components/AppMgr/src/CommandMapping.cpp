#include "AppMgr/CommandMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger CommandMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CommandMapping"));

void CommandMapping::addCommand(unsigned int commandId, RegistryItem *app)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribe to a command " << commandId << " in app " << app->getApplication()->getName() );
    mCommandMapping.insert(CommandMapItem(commandId, app));
}

void CommandMapping::removeCommand(unsigned int commandId)
{
    mCommandMapping.erase(commandId);
}

void CommandMapping::removeItem(RegistryItem *app)
{
    for(CommandMap::iterator it = mCommandMapping.begin(); it != mCommandMapping.end(); it++)
    {
        if(it->second->getApplication()->getSessionID() == app->getApplication()->getSessionID())
        {
            mCommandMapping.erase(it->first);
        }
    }
}

RegistryItem *CommandMapping::findRegistryItemAssignedToCommand(unsigned int commandId) const
{
    CommandMap::const_iterator it = mCommandMapping.find( commandId );
    if ( it != mCommandMapping.end() )
    {
        if ( !it->second )
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found" );
            return 0;
        }
        if ( it->second->getApplication() )
        {
            return it->second;
        }
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Command " << commandId << " not found in subscribed." );
    return 0;
}

CommandMapping::CommandMapping(const CommandMapping &)
{
}

}
