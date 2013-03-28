//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
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
