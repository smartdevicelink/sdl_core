/**
 * \file CommandMapping.h
 * \brief Command mapping
 * \author vsalo
 */

#ifndef COMMANDMAPPING_H
#define COMMANDMAPPING_H

#include <map>
#include <tuple>
#include <vector>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    class RegistryItem;
    class Application;

    /**
     * \brief CommandType acts as command type enum representation that allows iterating over types in cycle and using types in comparison
     */
    class CommandType
    {
    public:

        /**
         * \brief command types
         */
        typedef enum
        {
            UNDEFINED = -1,
            UI        = 0,
            FIRST     = UI,
            VR        = 1,
            TTS       = 2,
            LAST      = TTS
        } Type;

        /**
         * \brief Default constructor
         */
        CommandType();

        /**
         * \brief Copy constructor
         */
        CommandType(const CommandType& src);

        /**
         * \brief Class constructor
         * \param type command type to create a class with
         */
        CommandType(const Type& type);

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator==(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator==(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator<(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator<(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator>(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator>(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator!=(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator!=(const CommandType& type) const;

        /**
         * \brief pre-increment operator
         * \return incremented value
         */
        CommandType& operator++ ();

        /**
         * \brief post-increment operator
         * \return incremented value
         */
        CommandType operator++ (int);

        /**
         * \brief get command type
         * \return command type
         */
        const Type& getType() const;

    private:
        Type mType;
    };

    /**
     * \brief mapping of command id to specific command type
     */
    typedef std::tuple<unsigned int, CommandType> CommandKey;

    /**
     * \brief command-to-registered_app map
     */
    typedef std::map<CommandKey, RegistryItem*> CommandMap;

    /**
     * \brief command-to-registered_app map item
     */
    typedef std::pair<CommandKey, RegistryItem*> CommandMapItem;

    /**
     * \brief command types associated with command
     */
    typedef std::vector<CommandType> CommandTypes;

    /**
     * \brief commands vector
     */
    typedef std::vector<CommandKey> Commands;

    /**
     * \brief command_id-to-request_number map (command id is a key);
     */
    typedef std::map<unsigned int, unsigned int> RequestsAwaitingResponse;

    /**
     * \brief command_id-to-request_number map item (command id is a key);
     */
    typedef std::pair<unsigned int, unsigned int> RequestAwaitingResponse;

    /**
     * \brief CommandMapping acts as a mapping of command to registsred application that subscribed to them
     */
    class CommandMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        CommandMapping();

        /**
         * \brief Default class destructor
         */
        ~CommandMapping();

        /**
         * \brief add a command to a mapping
         * \param commandId command id
         * \param type command type
         * \param app application to map a command to
         */
        void addCommand( unsigned int commandId, CommandType type, RegistryItem* app );

        /**
         * \brief remove a command from a mapping
         * \param commandId command id
         * \param type a type of a command
         */
        void removeCommand(unsigned int commandId, CommandType type);

        /**
         * \brief remove an application from a mapping
         * \param app application to remove all associated commands from mapping
         */
        void removeItem( RegistryItem* app );

        /**
         * \brief retrieve types associated with command id in current mapping
         * \param commandId command id to search for types
         * \param types input container of command types to be filled with result
         */
        void getTypes(unsigned int commandId, CommandTypes& types ) const;

        /**
         * \brief find a registry item subscribed to command
         * \param commandId command id
         * \param type command type
         * \return RegistryItem instance
         */
        RegistryItem *findRegistryItemAssignedToCommand(unsigned int commandId, CommandType type) const;

        /**
         * \brief find commands the registry item is subscribed to
         * \param item a registry item in question
         * \return CommandKey vector
         */
        Commands findCommandsAssignedToRegistryItem(const RegistryItem* item) const;

        /**
         * \brief find commands the application is subscribed to
         * \param item a registry item in question
         * \return CommandKey vector
         */
        Commands findCommandsAssignedToApplication(const Application *item) const;

        /**
         * \brief get count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to count unresponded requests for
         * \return unresponded requests count
         */
        unsigned int getUnrespondedRequestCount(const unsigned int& cmdId) const;

        /**
         * \brief increment count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to increment unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int incrementUnrespondedRequestCount(const unsigned int& cmdId);

        /**
         * \brief decrement count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to decrement unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int decrementUnrespondedRequestCount(const unsigned int& cmdId);

        /**
         * \brief cleans all the items
         */
        void clear( );

        /**
         * \brief cleans all the requests awaiting response
         */
        void clearUnrespondedRequests( );

    private:

        /**
         * \brief Copy constructor
         */
        CommandMapping(const CommandMapping&);

        CommandType   mCommandType;
        CommandMap    mCommandMapping;
        RequestsAwaitingResponse mRequestsPerCommand;
        static log4cplus::Logger mLogger;
    };

}

#endif // COMMANDMAPPING_H
