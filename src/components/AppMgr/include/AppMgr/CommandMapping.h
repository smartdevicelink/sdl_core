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
     * \brief add a command to a mapping
     * \param commandId command id
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

private:

    /**
     * \brief Copy constructor
     */
    CommandMapping(const CommandMapping&);

    CommandType   mCommandType;
    CommandMap    mCommandMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // COMMANDMAPPING_H
