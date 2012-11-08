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

class CommandType
{
public:
    typedef enum
    {
        UNDEFINED = -1,
        UI        = 0,
        FIRST     = UI,
        VR        = 1,
        TTS       = 2,
        LAST      = TTS
    } Type;

    CommandType();
    CommandType(const CommandType& src);
    CommandType(const Type& type);
    bool operator==(const Type& type) const;
    bool operator==(const CommandType& type) const;
    bool operator<(const Type& type) const;
    bool operator<(const CommandType& type) const;
    bool operator>(const Type& type) const;
    bool operator>(const CommandType& type) const;
    bool operator!=(const Type& type) const;
    bool operator!=(const CommandType& type) const;
    CommandType& operator++ ();
    CommandType operator++ (int);
    const Type& getType() const;

private:
    Type mType;
};

typedef std::tuple<unsigned int, CommandType> CommandKey;
typedef std::map<CommandKey, RegistryItem*> CommandMap;
typedef std::pair<CommandKey, RegistryItem*> CommandMapItem;
typedef std::vector<CommandType> CommandTypes;

class CommandMapping
{
public:
    CommandMapping();
    void addCommand( unsigned int commandId, CommandType type, RegistryItem* app );
    void removeCommand(unsigned int commandId, CommandType type);
    void removeItem( RegistryItem* app );
    void getTypes(unsigned int commandId, CommandTypes& types ) const;
    RegistryItem *findRegistryItemAssignedToCommand(unsigned int commandId, CommandType type) const;

private:
    CommandMapping(const CommandMapping&);

    CommandType   mCommandType;
    CommandMap    mCommandMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // COMMANDMAPPING_H
