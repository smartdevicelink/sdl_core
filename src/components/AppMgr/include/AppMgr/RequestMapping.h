/**
 * \file RequestMapping.h
 * \brief Request mapping
 * \author vsalo
 */

#ifndef REQUESTMAPPING_H
#define REQUESTMAPPING_H

#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief message-to-command map (message id as a key)
     */
    typedef std::map<int, unsigned int> RequestMap;

    /**
     * \brief message-to-command map item (message id as a key)
     */
    typedef std::pair<int, unsigned int> RequestMapItem;

    /**
     * \brief MessageMapping acts as a mapping of messages to registered application which subscribes to them
     */
    class RequestMapping
    {
    public:
        /**
         * \brief Default class constructor
         */
        RequestMapping();

        /**
         * \brief Default class destructor
         */
        ~RequestMapping();

        /**
         * \brief add a message to a mapping
         * \param msgId message id
         * \param cmdId a request to map a message to
         */
        void addMessage( int msgId, unsigned int cmdId );

        /**
         * \brief remove a message from a mapping
         * \param msgId message id
         */
        void removeMessage(int msgId);

        /**
         * \brief remove an application from a mapping
         * \param cmdId a request to remove all associated messages from mapping
         */
        void removeRequest( unsigned int cmdId );

        /**
         * \brief find a registry item subscribed to message
         * \param msgId message id
         * \return id of the request assigned to a message
         */
        unsigned int findRequestIdAssignedToMessage(int msgId) const;

        /**
         * \brief cleans request mapping
         */
        void clear( );

    private:

        /**
         * \brief Copy constructor
         */
        RequestMapping(const RequestMapping&);

        RequestMap    mRequestMapping;
        static log4cplus::Logger mLogger;
    };

}

#endif // REQUESTMAPPING_H
