/**
 * \file MessageMapping.h
 * \brief Message mapping
 * \author vsalo
 */

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

    /**
     * \brief message-to-registered_app map
     */
    typedef std::map<int, RegistryItem*> MessageMap;

    /**
     * \brief message-to-registered_app map item
     */
    typedef std::pair<int, RegistryItem*> MessageMapItem;

    /**
     * \brief MessageMapping acts as a mapping of messages to registered application which subscribes to them
     */
    class MessageMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        MessageMapping();

        /**
         * \brief add a message to a mapping
         * \param msgId message id
         * \param app application to map a message to
         */
        void addMessage( int msgId, RegistryItem* app );

        /**
         * \brief add a message to a mapping
         * \param msgId message id
         * \param sessionID session to map appropriate application with message
         */
        void addMessage( int msgId, unsigned char sessionID );

        /**
         * \brief remove a message from a mapping
         * \param msgId message id
         */
        void removeMessage(int msgId);

        /**
         * \brief remove an application from a mapping
         * \param app application to remove all associated messages from mapping
         */
        void removeItem( RegistryItem* app );

        /**
         * \brief find a registry item subscribed to message
         * \param msgId message id
         * \return RegistryItem instance
         */
        RegistryItem *findRegistryItemAssignedToCommand(int msgId) const;

        /**
         * \brief cleans message mapping
         */
        void clear( );

    private:

        /**
         * \brief Copy constructor
         */
        MessageMapping(const MessageMapping&);

        MessageMap    mMessageMapping;
        static log4cplus::Logger mLogger;
    };

}

#endif // MESSAGEMAPPING_H
