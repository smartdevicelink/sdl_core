#include "AppMgr/RequestMapping.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/RegistryItem.h"

namespace NsAppManager
{

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
    mRequestMapping.insert(RequestMapItem(msgId, cmdId));
}

/**
 * \brief remove a message from a mapping
 * \param msgId message id
 */
void RequestMapping::removeMessage(int msgId)
{
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
    RequestMap::const_iterator it = mRequestMapping.find( msgId );
    if ( it != mRequestMapping.end() )
    {
        return it->second;
    }

    return -1;
}

/**
 * \brief Copy constructor
 */
RequestMapping::RequestMapping(const RequestMapping &)
{
}

}
