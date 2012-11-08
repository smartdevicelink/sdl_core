#include "AppMgr/CommandMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger CommandMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CommandMapping"));

CommandMapping::CommandMapping()
{
}

void CommandMapping::addCommand(unsigned int commandId, CommandType type, RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Adding a command to a null registry item");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to a command " << commandId << " type " << type.getType() << " in app " << app->getApplication()->getName() );
    mCommandMapping.insert(CommandMapItem(commandId, Command(type, app)));
}

void CommandMapping::removeCommand(unsigned int commandId)
{
    mCommandMapping.erase(commandId);
}

void CommandMapping::removeItem(RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
        return;
    }
    if(!app->getApplication())
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove an item without an application");
        return;
    }
    for(CommandMap::iterator it = mCommandMapping.begin(); it != mCommandMapping.end(); it++)
    {
        Command& command = it->second;
        RegistryItem* registryItem = command.second;
        if(registryItem->getApplication())
        {
            if(registryItem->getApplication()->getSessionID() == app->getApplication()->getSessionID())
            {
                mCommandMapping.erase(it->first);
            }
        }
    }
}

CommandType CommandMapping::getType(unsigned int commandId) const
{
    CommandMap::const_iterator it = mCommandMapping.find( commandId );
    if ( it != mCommandMapping.end() )
    {
        const Command& command = it->second;
        return command.first;
    }
    return CommandType::UNDEFINED;
}

RegistryItem *CommandMapping::findRegistryItemAssignedToCommand(unsigned int commandId) const
{
    CommandMap::const_iterator it = mCommandMapping.find( commandId );
    if ( it != mCommandMapping.end() )
    {
        const Command& command = it->second;
        RegistryItem* registryItem = command.second;
        if( registryItem )
        {
            if ( registryItem->getApplication() )
            {
                LOG4CPLUS_INFO_EXT(mLogger, "An application "<< registryItem->getApplication()->getName() <<" is subscribed to a command " << commandId );
                return registryItem;
            }
            LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!" );
            return 0;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found!" );
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Command " << commandId << " not found in subscribed." );
    return 0;
}

CommandMapping::CommandMapping(const CommandMapping &)
{
}

CommandType::CommandType()
    :mType(CommandType::UNDEFINED)
{
}

CommandType::CommandType(const CommandType& src)
    :mType(src.getType())
{
}

CommandType::CommandType(const CommandType::Type& type)
    :mType(type)
{
}

bool CommandType::operator ==(const CommandType::Type &type) const
{
    return mType == type;
}

bool CommandType::operator ==(const CommandType &type) const
{
    return mType == type.getType();
}

const CommandType::Type& CommandType::getType() const
{
    return mType;
}

}
