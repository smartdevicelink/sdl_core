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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TEST_H_

#include "utils/macro.h"
#include "hmi_message_handler/hmi_message_handler.h"


using namespace hmi_message_handler;

namespace test{
	namespace HMI_Message_Handler_Tester{
	
		/**
		* @brief Redefinition HMIMessageHandler for test
		*/
		class HMIMessageHandlerTester: public HMIMessageHandler
		{
		public:
		
			/**
			* @brief Constructor
			*/
			HMIMessageHandlerTester();
			
			/**
			* @brief Destructor
			*/
			~HMIMessageHandlerTester();
			
		public:
			virtual void SendMessageToHMI(utils::SharedPtr<application_manager::Message> message);
			virtual void OnMessageReceived(utils::SharedPtr<application_manager::Message> message);
			virtual void OnErrorSending(utils::SharedPtr<application_manager::Message> message);				
			virtual void AddHMIMessageAdapter(HMIMessageAdapter* adapter);
			virtual void RemoveHMIMessageAdapter(HMIMessageAdapter* adapter);
			
		protected:
		
		private:
		
			/**
			* @brief Copy constructor
			*
			* @param first - ref HMIMessageHandlerTester
			*/
			HMIMessageHandlerTester(const HMIMessageHandlerTester &);			

		private:
		
			/**
			* @brief Operator assignment
			*
			* @param first - ref HMIMessageHandlerTester
			*
			* @return point to HMIMessageHandlerTester
			*/
			HMIMessageHandlerTester *operator=(const HMIMessageHandlerTester &);
			
		};
		
	}//	namespace HMI_Message_Handler_Tester
}//namespace test


#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_HMI_MESSAGE_HANDLER_TEST_H_



