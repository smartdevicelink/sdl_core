#ifndef MESSAGEMAPPING_H
#define MESSAGEMAPPING_H

#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

class RegistryItem;

typedef std::map<int, RegistryItem*> MessageMap;
typedef std::pair<int, RegistryItem*> MessageMapItem;

class MessageMapping
{
public:
    MessageMapping();
    void addMessage( int msgId, RegistryItem* app );
    void addMessage( int msgId, unsigned char sessionID );
    void removeMessage(int msgId);
    void removeItem( RegistryItem* app );
    RegistryItem *findRegistryItemAssignedToCommand(int msgId) const;

private:
    MessageMapping(const MessageMapping&);

    MessageMap    mMessageMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // MESSAGEMAPPING_H
