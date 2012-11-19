/**
 * \file RequestMapping.cpp
 * \brief Request mapping
 * \author vsalo
 */

#include "AppMgr/RequestMapping.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger RequestMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("RequestMapping"));

    /**
     * \brief Default class constructor
     */
    RequestMapping::RequestMapping()
    {
    }

    /**
     * \brief add a message to a mapping
     * \param msgId message id
     * \param cmdId a request to map a message to
     */
    void RequestMapping::addMessage(int msgId, unsigned int cmdId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Mapping a message " << msgId << " to a commandId " << cmdId );
        mRequestMapping.insert(RequestMapItem(msgId, cmdId));
    }

    /**
     * \brief remove a message from a mapping
     * \param msgId message id
     */
    void RequestMapping::removeMessage(int msgId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Unmapping a message " << msgId );
        mRequestMapping.erase(msgId);
    }

    /**
     * \brief remove an application from a mapping
     * \param cmdId a request to remove all associated messages from mapping
     */
    void RequestMapping::removeRequest(unsigned int cmdId)
    {
        for(RequestMap::iterator it = mRequestMapping.begin(); it != mRequestMapping.end(); it++)
        {
            if(it->second == cmdId)
            {
                mRequestMapping.erase(it->first);
            }
        }
    }

    /**
     * \brief find a registry item subscribed to message
     * \param msgId message id
     * \return id of the request assigned to a message
     */
    unsigned int RequestMapping::findRequestIdAssignedToMessage(int msgId) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching a message " << msgId << " for mapped command id " );
        RequestMap::const_iterator it = mRequestMapping.find( msgId );
        if ( it != mRequestMapping.end() )
        {
            LOG4CPLUS_INFO_EXT(mLogger, "A message " << msgId << " is mapped to a command id " << it->second );
            return it->second;
        }

        LOG4CPLUS_ERROR_EXT(mLogger, " No commands assigned to a message " << msgId);
        return -1;
    }

    /**
     * \brief Copy constructor
     */
    RequestMapping::RequestMapping(const RequestMapping &)
    {
    }

}
