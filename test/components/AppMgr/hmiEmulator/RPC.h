#ifndef RPC_H
#define RPC_H

#include <iostream>

#include "json/json.h"
#include <json/value.h>
#include "mb_controller.hpp"
#include "CMessageBroker.hpp"
#include "LoggerHelper.hpp"
#include "ResourceContainer.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"
#include "JSONHandler/SDLRPCObjects/Result.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{
    /**
    * \class RPC
    * \brief MessageBroker Controller.
    */
    class RPC : public NsMessageBroker::CMessageBrokerController
    {

    public:
        /**
        * \brief Constructor.
        * \param address remote network address or FQDN
        * \param port remote local port
        */
        RPC(const std::string& address, unsigned short port, const std::string& name);

        /**
        * \brief Destructor.
        */
        virtual ~RPC();

        /**
        * \brief process request.
        * \param root JSON message.
        */
        virtual void processRequest(Json::Value& root);

        /**
        * \brief process notification.
        * \param root JSON message.
        */
        virtual void processNotification(Json::Value& root);

        /**
        * \brief process response.
        * \param method method name which has been called.
        * \param root JSON message.
        */
        virtual void processResponse(std::string method, Json::Value& root);

        /**
         * \brief Callback function which is called upon a new message from mobile side arrival
         * \param command RPC2Bus Json message
         */
        virtual void messageReceivedFromDeviceCallback( NsRPC2Communication::RPC2Command * command ) = 0;

        /**
         * \brief send a message to a mobile side via applinkcore
         * \param command Pointer to base class of AppLink Json object
         */
        virtual void sendRPC2MessageToMobileSide(NsRPC2Communication::RPC2Command * command);

    protected:
        /**
         * \struct ResponseContainer
         * \brief Used for associating request method name to corresponding response.
         * TODO - not needed with latest version of code generator?
        */
        struct ResponseContainer
        {
            /**
              *\brief Request method name.
            */
            std::string methodName;

            /**
              *\brief Response.
            */
            Json::Value response;
        };

        static log4cplus::Logger mLogger;
    };
}

#endif // RPC_H
