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


#include "rpc/HMIMessageHandlerTester.h"


namespace test{
	namespace HMI_Message_Handler_Tester{
		
		
		HMIMessageHandlerTester::HMIMessageHandlerTester()
		{
		}
		
		
		HMIMessageHandlerTester::~HMIMessageHandlerTester()
		{
		}
		

		HMIMessageHandlerTester::HMIMessageHandlerTester(const HMIMessageHandlerTester &)
		{
		}


		HMIMessageHandlerTester *HMIMessageHandlerTester::operator=(const HMIMessageHandlerTester &)
		{
			return this;
		}


		
		void HMIMessageHandlerTester::SendMessageToHMI(utils::SharedPtr<application_manager::Message> message)
		{
			printf("\n\n\n\n  HMIMessageHandlerTester::SendMessageToHMI      \n\n\n\n");
			
			
			
			printf("correlation_id : %d\n",(*message).correlation_id());
			printf("function_id : %d\n",(*message).function_id());
			printf("connection_key : %d\n",(*message).connection_key());
			printf("type : %d\n",(*message).type());
			printf("protocol_version : %d\n",(*message).protocol_version());
			printf("json_message : %s\n\n",(*message).json_message().data());
			
		}
		
		
		void HMIMessageHandlerTester::OnMessageReceived(utils::SharedPtr<application_manager::Message> message)
		{
			printf("\n\n\n\n  HMIMessageHandlerTester::OnMessageReceived      \n\n\n\n");
		}
		
		
		void HMIMessageHandlerTester::OnErrorSending(utils::SharedPtr<application_manager::Message> message)
		{
			printf("\n\n\n\n  HMIMessageHandlerTester::OnErrorSending      \n\n\n\n");
		}
		
		
		void HMIMessageHandlerTester::AddHMIMessageAdapter(HMIMessageAdapter* adapter)
		{
			printf("\n\n\n\n  HMIMessageHandlerTester::AddHMIMessageAdapter      \n\n\n\n");
		}
		
		
		void HMIMessageHandlerTester::RemoveHMIMessageAdapter(HMIMessageAdapter* adapter)
		{
			printf("\n\n\n\n  HMIMessageHandlerTester::RemoveHMIMessageAdapter      \n\n\n\n");
		}
			

	
	
		}//	namespace HMI_Message_Handler_Tester
}//namespace test


