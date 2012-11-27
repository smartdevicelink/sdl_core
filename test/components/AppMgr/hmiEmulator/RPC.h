#ifndef RPC_H
#define RPC_H

#include <iostream>

#include "json/json.h"
#include <json/value.h>
#include "mb_controller.hpp"
#include "CMessageBroker.hpp"
#include "LoggerHelper.hpp"
#include "JSONHandler/IRPC2CommandsObserver.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"

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
    class RPC : public NsMessageBroker::CMessageBrokerController, public IRPC2CommandsObserver
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
        ~RPC();

        /**
        * \brief process request.
        * \param root JSON message.
        */
        void processRequest(Json::Value& root);

        /**
        * \brief process notification.
        * \param root JSON message.
        */
        void processNotification(Json::Value& root);

        /**
        * \brief process response.
        * \param method method name which has been called.
        * \param root JSON message.
        */
        void processResponse(std::string method, Json::Value& root);

        /**
         * \brief Callback function which is called by JSONRPC2Handler
         *  when new RPC2Bus Json message is received from HMI.
         * \param command RPC2Bus Json message
         */
        virtual void onCommandReceivedCallback( NsRPC2Communication::RPC2Command * command ) = 0;

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
