#ifndef COMMANDMAPPING_H
#define COMMANDMAPPING_H

#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

class RegistryItem;

class CommandType
{
public:
    typedef enum
    {
        UNDEFINED = -1,
        UI        = 0,
        VR        = 1,
        TTS       = 2
    } Type;

    CommandType();
    CommandType(const CommandType& src);
    CommandType(const Type& type);
    bool operator==(const Type& type) const;
    bool operator==(const CommandType& type) const;
    const Type& getType() const;

private:
    Type mType;
};

typedef std::pair<CommandType, RegistryItem*> Command;
typedef std::map<unsigned int, Command> CommandMap;
typedef std::pair<unsigned int, Command> CommandMapItem;

class CommandMapping
{
public:
    CommandMapping();
    void addCommand( unsigned int commandId, CommandType type, RegistryItem* app );
    void removeCommand(unsigned int commandId);
    void removeItem( RegistryItem* app );
    CommandType getType( unsigned int commandId ) const;
    RegistryItem *findRegistryItemAssignedToCommand(unsigned int commandId) const;

private:
    CommandMapping(const CommandMapping&);

    CommandType   mCommandType;
    CommandMap    mCommandMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // COMMANDMAPPING_H
