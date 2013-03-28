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

#ifndef MOBILEHANDLER_H
#define MOBILEHANDLER_H

class JSONHandler;

namespace log4cplus
{
    class Logger;
}

namespace NsSmartDeviceLinkRPC
{
    class SDLRPCMessage;
}

namespace NsAppManager
{

    /**
     * \brief MobileHandler acts as wrapper for mobile JSON handler
     */
    class MobileHandler
    {
    public:

        /**
         * \brief Returning class instance
         * \return class instance
         */
        static MobileHandler &getInstance();

        /**
         * \brief send a message via associated handler
         * \param message message to send
         */
        void sendRPCMessage(const NsSmartDeviceLinkRPC::SDLRPCMessage * message, int appId );

        /**
         * \brief set mobile Json handler
         * \param handler mobile Json handler instance
         */
        void setJsonHandler(JSONHandler* handler);

        /**
         * \brief get mobile Json handler
         * \return mobile Json handler instance
         */
        JSONHandler* getJsonHandler( ) const;

    private:

        /**
         * \brief Default class constructor
         */
        MobileHandler();

        /**
         * \brief Copy constructor
         */
        MobileHandler(const MobileHandler&);

        JSONHandler* mJSONHandler;

        static log4cplus::Logger mLogger;
    };

}

#endif // MOBILEHANDLER_H
