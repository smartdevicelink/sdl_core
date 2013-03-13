//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef HMIHANDLER_H
#define HMIHANDLER_H

class JSONRPC2Handler;

namespace NsRPC2Communication
{
    class RPC2Notification;
    class RPC2Response;
    class RPC2Request;
}

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief HMIHandler acts as wrapper for JSON RPC2 handler
     */
    class HMIHandler
    {
    public:

        /**
         * \brief Returning class instance
         * \return class instance
         */
        static HMIHandler &getInstance();

        /**
         * \brief set handler ready state
         * \param ready ready state
         */
        void setReadyState(bool ready);

        /**
         * \brief send notification via associated handler
         * \param command notification to send
         */
        void sendNotification( const NsRPC2Communication::RPC2Notification * command );

        /**
         * \brief send response via associated handler
         * \param command response to send
         */
        void sendResponse( const NsRPC2Communication::RPC2Response * command );

        /**
         * \brief send request via associated handler
         * \param command request to send
         */
        void sendRequest( const NsRPC2Communication::RPC2Request * command );

        /**
         * \brief set Json RPC2 handler
         * \param handler Json RPC2 handler instance
         */
        void setJsonRPC2Handler(JSONRPC2Handler* handler);

        /**
         * \brief get Json RPC2 handler
         * \return Json RPC2 handler instance
         */
        JSONRPC2Handler* getJsonRPC2Handler( ) const;

    private:

        /**
         * \brief Default class constructor
         */
        HMIHandler();

        /**
         * \brief Copy constructor
         */
        HMIHandler(const HMIHandler&);

        bool             m_bHMIReady;
        JSONRPC2Handler* mJSONRPC2Handler;

        static log4cplus::Logger mLogger;
    };

}

#endif // HMIHANDLER_H
