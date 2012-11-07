#include "AppMgr/MessageMapping.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger MessageMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MessageMapping"));

MessageMapping::MessageMapping()
{
}

void MessageMapping::addMessage(int msgId, RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Adding a message to a null registry item");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to a message " << msgId << " in app " << app->getApplication()->getName() );
    mMessageMapping.insert(MessageMapItem(msgId, app));
}

void MessageMapping::addMessage(int msgId, unsigned char sessionID)
{
    RegistryItem* app = AppMgrRegistry::getInstance().getItem(sessionID);
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found by sessionId "<<sessionID );
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribe to a message " << msgId << " in app " << app->getApplication()->getName() );
    addMessage(msgId, app);
}

void MessageMapping::removeMessage(int msgId)
{
    mMessageMapping.erase(msgId);
}

void MessageMapping::removeItem(RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
        return;
    }
    for(MessageMap::iterator it = mMessageMapping.begin(); it != mMessageMapping.end(); it++)
    {
        if(it->second->getApplication()->getSessionID() == app->getApplication()->getSessionID())
        {
            mMessageMapping.erase(it->first);
        }
    }
}

RegistryItem *MessageMapping::findRegistryItemAssignedToCommand(int msgId) const
{
    MessageMap::const_iterator it = mMessageMapping.find( msgId );
    if ( it != mMessageMapping.end() )
    {
        if ( !it->second )
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found" );
            return 0;
        }
        if ( it->second->getApplication() )
        {
            LOG4CPLUS_INFO_EXT(mLogger, "An application "<< it->second->getApplication()->getName() <<" is subscribed to a message " << msgId );
            return it->second;
        }
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Message " << msgId << " not found in subscribed." );
    return 0;
}

MessageMapping::MessageMapping(const MessageMapping &)
{
}

}
