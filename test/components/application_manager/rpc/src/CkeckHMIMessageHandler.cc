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


#include "rpc/CheckMobileMessageHandler.h"

namespace test{
	namespace Mobile_Message_Handler_Tester{

		CheckMobileMessageHandler::CheckMobileMessageHandler(const std::string &inUnitTestName,
						const std::string &inTestName, 
						utils::SharedPtr<application_manager::Message> &inResponse)
			:ICheckClass(inUnitTestName, inTestName)
			//,flagRun_(false)
			,got_response_(new application_manager::Message())
			,expected_response_(new application_manager::Message())
		{
			*expected_response_ = *inResponse;
		}
		
	
		CheckMobileMessageHandler::~CheckMobileMessageHandler()
		{
		}
			
		
		CheckMobileMessageHandler::CheckMobileMessageHandler(const CheckMobileMessageHandler &inCheck)
				:ICheckClass(inCheck.unitTestName, inCheck.testName)
		{
		}
		
		
		CheckMobileMessageHandler *CheckMobileMessageHandler::operator=(const CheckMobileMessageHandler &)
		{
			return this;
		}
		
		
		bool CheckMobileMessageHandler::check()
		{
			bool returnValues = true;
			
			printf("\n\n\n----------------------------------------\n\n");
			printf("EXPECTED_RESPONSE:\n\n");
			printf("correlation_id : %d\n",(*expected_response_).correlation_id());
			printf("function_id : %d\n",(*expected_response_).function_id());
			printf("connection_key : %d\n",(*expected_response_).connection_key());
			printf("type : %d\n",(*expected_response_).type());
			printf("protocol_version : %d\n",(*expected_response_).protocol_version());
			printf("json_message : %s\n\n",(*expected_response_).json_message().data());
			
			printf("GOT_RESPONSE:\n\n");
			printf("correlation_id : %d\n",(*got_response_).correlation_id());
			printf("function_id : %d\n",(*got_response_).function_id());
			printf("connection_key : %d\n",(*got_response_).connection_key());
			printf("type : %d\n",(*got_response_).type());
			printf("protocol_version : %d\n",(*got_response_).protocol_version());
			printf("json_message : %s\n\n",(*got_response_).json_message().data());
			
			printf("----------------------------------------\n\n");
			
			
			returnValues = returnValues && ((*expected_response_).correlation_id()==(*got_response_).correlation_id());
			//printf("%d\n",returnValues);
			returnValues = returnValues && ((*expected_response_).function_id()==(*got_response_).function_id());
			//printf("%d\n",returnValues);
			returnValues = returnValues && ((*expected_response_).connection_key()==(*got_response_).connection_key());
			//printf("%d\n",returnValues);
			returnValues = returnValues && ((*expected_response_).protocol_version()==(*got_response_).protocol_version());
			//printf("%d\n",returnValues);
			returnValues = returnValues && ((*expected_response_).json_message()==(*got_response_).json_message());
			//printf("%d\n",returnValues);
			
			return returnValues;
		}
		
		
		void CheckMobileMessageHandler::addMessage(const MobileMessage &inMessage)
		{
			*got_response_ = *inMessage;		

			//flagRun_ = true;
		}
			
	}//namespace Mobile_Message_Handler_Tester
}//namespace test


