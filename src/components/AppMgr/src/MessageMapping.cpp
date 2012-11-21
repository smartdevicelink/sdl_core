/**
 * \file MessageMapping.cpp
 * \brief Message mapping
 * \author vsalo
 */

#include "AppMgr/MessageMapping.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger MessageMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MessageMapping"));

    /**
     * \brief Default class constructor
     */
    MessageMapping::MessageMapping()
    {
    }

    /**
     * \brief Default class destructor
     */
    MessageMapping::~MessageMapping()
    {
        clear();
    }

    /**
     * \brief add a message to a mapping
     * \param msgId message id
     * \param app application to map a message to
     */
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

    /**
     * \brief add a message to a mapping
     * \param msgId message id
     * \param sessionID session to map appropriate application with message
     */
    void MessageMapping::addMessage(int msgId, unsigned char sessionID)
    {
        RegistryItem* app = AppMgrRegistry::getInstance().getItem(0, sessionID);//0-temp! Specify unsigned int connectionID instead!!!!
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found by sessionId " << sessionID );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Subscribe to a message " << msgId << " in app " << app->getApplication()->getName() );
        addMessage(msgId, app);
    }

    /**
     * \brief remove a message from a mapping
     * \param msgId message id
     */
    void MessageMapping::removeMessage(int msgId)
    {
        mMessageMapping.erase(msgId);
    }

    /**
     * \brief remove an application from a mapping
     * \param app application to remove all associated messages from mapping
     */
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

    /**
     * \brief find a registry item subscribed to message
     * \param msgId message id
     * \return RegistryItem instance
     */
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
                LOG4CPLUS_INFO_EXT(mLogger, "An application " << it->second->getApplication()->getName() << " is subscribed to a message " << msgId );
                return it->second;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Message " << msgId << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief cleans menu mapping
     */
    void MessageMapping::clear()
    {
        mMessageMapping.clear();
    }

    /**
     * \brief Copy constructor
     */
    MessageMapping::MessageMapping(const MessageMapping &)
    {
    }

}
