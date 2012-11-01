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

typedef std::map<unsigned int, RegistryItem*> CommandMap;
typedef std::pair<unsigned int, RegistryItem*> CommandMapItem;

class CommandMapping
{
public:
    CommandMapping();
    void addCommand( unsigned int commandId, RegistryItem* app );
    void removeCommand(unsigned int commandId);
    void removeItem( RegistryItem* app );
    RegistryItem *findRegistryItemAssignedToCommand(unsigned int commandId) const;

private:
    CommandMapping(const CommandMapping&);

    CommandMap    mCommandMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // COMMANDMAPPING_H
