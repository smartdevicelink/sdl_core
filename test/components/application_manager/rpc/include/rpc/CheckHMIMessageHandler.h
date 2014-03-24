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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_CHECK_HMI_MESSAGE_HANDLER_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_CHECK_HMI_MESSAGE_HANDLER_H_

#include "rpc/ICheckClass.h"

#include "hmi_message_handler/hmi_message_handler.h"
#include "protocol_handler/raw_message.h"

#include "utils/shared_ptr.h"
#include "application_manager/application_manager_impl.h"

namespace test{
	namespace HMI_Message_Handler_Tester{

		typedef  utils::SharedPtr<application_manager::Message> SP_AM_M;
		/**
		* @brief Test class for test request/response Mobile
		*/
		class CheckHMIMessageHandler: public ICheckClass
		{
		public:
			/**
			* @brief In argument name name_test_case,next name_test and got_response_
			*
			* @param first - name test case, second - name test, third - SharedPrt message
			*
			* @return
			*/
			CheckHMIMessageHandler(const std::string& inUnitTestName, const std::string& inTestName, SP_AM_M &);
			
			/**
			* @brief Destructor
			*/
			~CheckHMIMessageHandler();
			
		public:
			/**
			* @brief Calls Gtest func and check got response and expected response
			*
			* @param
			*
			* @return bool
			*/
			virtual bool check();
			
			/**
			* @brief set got_response_
			*
			* @param first ref MobileMessage
			*
			* @return void
			*/
			void addMessage(const MobileMessage &);
			
		protected:
		
		private:
			/**
			* @brief Copy constructor
			*
			* @param first ref CheckMobileMessageHandler
			*/
			CheckHMIMessageHandler(const CheckMobileMessageHandler &);
		
		private:
			/**
			* @brief Operator assignment
			*
			* @param first ref CheckMobileMessageHandler
			*
			* @return point to CheckHMIMessageHandler
			*/
			CheckHMIMessageHandler *operator=(const CheckMobileMessageHandler &);
			
		private:
			/**
			* @brief Got responce from app. manager
			*/
			utils::SharedPtr<application_manager::Message> got_response_;
			/**
			* @brief Expected response from XML
			*/
			utils::SharedPtr<application_manager::Message> expected_response_;
		};
		
	}//namespace HMI_Message_Handler_Tester
}//namespace test



#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_CHECK_HMI_MESSAGE_HANDLER_H_


