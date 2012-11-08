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
    mCommandMapping.insert(CommandMapItem(CommandKey(commandId, type), app));
}

void CommandMapping::removeCommand(unsigned int commandId, CommandType type)
{
    mCommandMapping.erase(CommandKey(commandId, type));
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
        RegistryItem* registryItem = it->second;
        if(registryItem->getApplication())
        {
            if(registryItem->getApplication()->getSessionID() == app->getApplication()->getSessionID())
            {
                mCommandMapping.erase(it->first);
            }
        }
    }
}

void CommandMapping::getTypes( unsigned int commandId, CommandTypes& types ) const
{
    for(CommandType type = CommandType::FIRST; type != CommandType::LAST; type++)
    {
        CommandMap::const_iterator it = mCommandMapping.find( CommandKey(commandId, type) );
        if ( it != mCommandMapping.end() )
        {
            types.push_back(type);
        }
    }
}

RegistryItem *CommandMapping::findRegistryItemAssignedToCommand(unsigned int commandId, CommandType type) const
{
    CommandMap::const_iterator it = mCommandMapping.find( CommandKey(commandId, type) );
    if ( it != mCommandMapping.end() )
    {
        RegistryItem* registryItem = it->second;
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
    LOG4CPLUS_INFO_EXT(mLogger, "Command " << commandId << " of type " <<type.getType()<< " not found in subscribed." );
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

bool CommandType::operator <(const CommandType::Type &type) const
{
    return mType < type;
}

bool CommandType::operator <(const CommandType &type) const
{
    return mType < type.getType();
}

bool CommandType::operator >(const CommandType::Type &type) const
{
    return mType > type;
}

bool CommandType::operator >(const CommandType &type) const
{
    return mType > type.getType();
}

bool CommandType::operator !=(const CommandType::Type &type) const
{
    return mType != type;
}

bool CommandType::operator !=(const CommandType &type) const
{
    return mType != type.getType();
}

CommandType& CommandType::operator ++()
{
    if(mType != CommandType::LAST)
    {
        int type = mType + 1;
        mType = (CommandType::Type)type;
    }
    return *this;
}

CommandType CommandType::operator++ (int)
{
    CommandType result(*this);
    ++(*this);
    return result;
}

const CommandType::Type& CommandType::getType() const
{
    return mType;
}

}
