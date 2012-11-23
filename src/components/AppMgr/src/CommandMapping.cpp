/**
 * \file CommandMapping.cpp
 * \brief Command mapping
 * \author vsalo
 */

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
     * \brief Default class destructor
     */
    CommandMapping::~CommandMapping()
    {
        clear();
    }

    /**
     * \brief add a command to a mapping
     * \param commandId command id
     * \param type command type
     */
    void CommandMapping::addCommand(unsigned int commandId, CommandType type)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Adding a command to a null registry item");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to a command " << commandId << " type " << type.getType() << " in app " << app->getApplication()->getName() );
        mCommands.insert(Commands(commandId, type));
    }

    /**
     * \brief remove a command from a mapping
     * \param commandId command id
     * \param type a type of a command
     */
    void CommandMapping::removeCommand(unsigned int commandId, CommandType type)
    {
        mCommands.erase(Command(commandId, type));
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
            Commands::const_iterator it = mCommands.find( Command(commandId, type) );
            if ( it != mCommands.end() )
            {
                types.push_back(type);
            }
        }
    }

    /**
     * \brief get count of commands
     * \return commands count
     */
    size_type CommandMapping::size() const
    {
        mCommands.size();
    }

    /**
     * \brief find commands the application is subscribed to
     * \param item a registry item in question
     * \return CommandKey vector
     */
    Commands CommandMapping::findCommandsAssignedToApplication(const Application *item) const
    {
        Commands commands;
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Cannot search using null param!");
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Searching for commands assigned to application " << item->getName() << " connection id " << item->getConnectionID() << " session id " << (uint)item->getSessionID());
            for(CommandMap::const_iterator it = mCommandMapping.begin(); it != mCommandMapping.end(); it++)
            {
                const RegistryItem* found = it->second;
                if( !found )
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "null-registr item found in the mapping!");
                    continue;
                }
                if( item == found->getApplication() )
                {
                    const CommandKey& key = it->first;
                    commands.push_back(key);
                    LOG4CPLUS_INFO_EXT(mLogger, "Found a command " << std::get<0>(key) << " of a type " << std::get<1>(key).getType() );
                }
            }
        }
        return commands;
    }

    /**
     * \brief get count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to count unresponded requests for
     * \return unresponded requests count
     */
    unsigned int CommandMapping::getUnrespondedRequestCount(const unsigned int &cmdId) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for unresponded requests for command " << cmdId );
        RequestsAwaitingResponse::const_iterator it = mRequestsPerCommand.find(cmdId);
        if(it != mRequestsPerCommand.end())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Unresponded requests for command " << cmdId << " is " << it->second );
            return it->second;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "No unresponded requests for command " << cmdId << " found! " );
        return 0;
    }

    /**
     * \brief increment count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to increment unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int CommandMapping::incrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Incrementing for unresponded requests for command " << cmdId );
        RequestsAwaitingResponse::iterator it = mRequestsPerCommand.find(cmdId);
        unsigned int reqsCount = it != mRequestsPerCommand.end() ? it->second : 0;
        LOG4CPLUS_INFO_EXT(mLogger, "Unresponded requests for command " << cmdId << " was " << reqsCount );
        reqsCount++;
        if(it != mRequestsPerCommand.end())
        {
            mRequestsPerCommand.erase(it);
        }
        mRequestsPerCommand.insert(RequestAwaitingResponse(cmdId, reqsCount));
        LOG4CPLUS_INFO_EXT(mLogger, "Unresponded requests for command " << cmdId << " became " << reqsCount );
        return reqsCount;
    }

    /**
     * \brief decrement count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to decrement unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int CommandMapping::decrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Decrementing for unresponded requests for command " << cmdId );
        RequestsAwaitingResponse::iterator it = mRequestsPerCommand.find(cmdId);
        if(it != mRequestsPerCommand.end())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Unresponded requests for command " << cmdId << " was " << it->second );
            if(it->second <= 0)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Trying to decrement already null value!" );
                return 0;
            }
            unsigned int reqsCount = it->second - 1;
            mRequestsPerCommand.erase(it);
            mRequestsPerCommand.insert(RequestAwaitingResponse(cmdId, reqsCount));
            LOG4CPLUS_INFO_EXT(mLogger, "Unresponded requests for command " << cmdId << " became " << reqsCount );
            return reqsCount;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "No unresponded requests for command " << cmdId << " found! " );
        return 0;
    }

    /**
     * \brief cleans all the items
     */
    void CommandMapping::clear()
    {
        mCommands.clear();
    }

    /**
     * \brief cleans all the requests awaiting response
     */
    void CommandMapping::clearUnrespondedRequests()
    {
        mRequestsPerCommand.clear();
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
