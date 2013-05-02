/**
* Copyright (c) 2013, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_MB_MESSAGE_ADAPTER
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_MB_MESSAGE_ADAPTER

#include "mb_controller.hpp"
#include "hmi_message_handler/hmi_message_adapter.h"

namespace hmi_message_handler {
	class MessageBrokerAdapter : public HMIMessageAdapter,
								 public NsMessageBroker::CMessageBrokerController
	 {
	public:
		explicit MessageBrokerAdapter(HMIMessageHandler* handler);
		~MessageBrokerAdapter();
		void sendMessageToHMI(application_manager::Message * message);

		/*Methods from CMessageBrokerController*/
	    /**
	     * \brief Called on receiving response message from RPCBus.
	     * \param method Name of corresponding request method that was sent previously to RPCBus.
	     * \param root Received Json object.
	     */
	    void processResponse(std::string method, Json::Value& root);

	    /**
	     * \brief Called on receiving request message from RPCBus.
	     * \param root Received Json object.
	     */
	    void processRequest(Json::Value& root);

	    /**
	     * \brief Called on receiving notification message from RPCBus.
	     * \param root Received Json object.
	     */
	    void processNotification(Json::Value& root);

	protected:
		void subscribeTo();
	    void processRecievedfromMB(Json::Value& root);

	private:
		static const std::string ADDRESS;
		static const uint16_t PORT;
	};
}

#endif // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_MB_MESSAGE_ADAPTER	