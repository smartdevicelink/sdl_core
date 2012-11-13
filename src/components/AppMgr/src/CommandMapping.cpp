#include "AppMgr/CommandMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger CommandMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("CommandMapping"));

/**
 * \brief Default class constructor
 */
CommandMapping::CommandMapping()
{
}

/**
 * \brief add a command to a mapping
 * \param commandId command id
 * \param type command type
 * \param app application to map a command to
 */
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

/**
 * \brief remove a command from a mapping
 * \param commandId command id
 * \param type a type of a command
 */
void CommandMapping::removeCommand(unsigned int commandId, CommandType type)
{
    mCommandMapping.erase(CommandKey(commandId, type));
}

/**
 * \brief remove an application from a mapping
 * \param app application to remove all associated commands from mapping
 */
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

/**
 * \brief retrieve types associated with command id in current mapping
 * \param commandId command id to search for types
 * \param types input container of command types to be filled with result
 */
void CommandMapping::getTypes( unsigned int commandId, CommandTypes& types ) const
{
    types.clear();
    for(CommandType type = CommandType::FIRST; type != CommandType::LAST; type++)
    {
        CommandMap::const_iterator it = mCommandMapping.find( CommandKey(commandId, type) );
        if ( it != mCommandMapping.end() )
        {
            types.push_back(type);
        }
    }
}

/**
 * \brief find a registry item subscribed to command
 * \param commandId command id
 * \param type command type
 * \return RegistryItem instance
 */
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

/**
 * \brief Copy constructor
 */
CommandMapping::CommandMapping(const CommandMapping &)
{
}

/**
 * \brief Default constructor
 */
CommandType::CommandType()
    :mType(CommandType::UNDEFINED)
{
}

/**
 * \brief Copy constructor
 */
CommandType::CommandType(const CommandType& src)
    :mType(src.getType())
{
}

/**
 * \brief Class constructor
 * \param type command type to create a class with
 */
CommandType::CommandType(const CommandType::Type& type)
    :mType(type)
{
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator ==(const CommandType::Type &type) const
{
    return mType == type;
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator ==(const CommandType &type) const
{
    return mType == type.getType();
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator <(const CommandType::Type &type) const
{
    return mType < type;
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator <(const CommandType &type) const
{
    return mType < type.getType();
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator >(const CommandType::Type &type) const
{
    return mType > type;
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator >(const CommandType &type) const
{
    return mType > type.getType();
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator !=(const CommandType::Type &type) const
{
    return mType != type;
}

/**
 * \brief comparison operator
 * \param type of a command to compare with
 * \return comparison result
 */
bool CommandType::operator !=(const CommandType &type) const
{
    return mType != type.getType();
}

/**
 * \brief pre-increment operator
 * \return incremented value
 */
CommandType& CommandType::operator ++()
{
    if(mType != CommandType::LAST)
    {
        int type = mType + 1;
        mType = (CommandType::Type)type;
    }
    return *this;
}

/**
 * \brief post-increment operator
 * \return incremented value
 */
CommandType CommandType::operator++ (int)
{
    CommandType result(*this);
    ++(*this);
    return result;
}

/**
 * \brief get command type
 * \return command type
 */
const CommandType::Type& CommandType::getType() const
{
    return mType;
}

}
