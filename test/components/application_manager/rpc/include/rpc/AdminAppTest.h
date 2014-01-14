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

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADMIN_APP_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADMIN_APP_TEST_H_

//Class replace TEST and Call's Gtest/Gmock of Google
#include "rpc/GTestFord.h"

//Container singltone, have all class what called in GTestFord
#include "rpc/GMockClassContainer.h"

//Interface class what called in GTestFord
#include "rpc/addTest.h"
#include "rpc/testHMI.h"
#include "rpc/xmlParser.h"
#include "rpc/ICheckClass.h"
#include "rpc/initStartData.h"
#include "rpc/HMIMessageHandlerTester.h"

#include "transport_manager/error.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"				 
#include "transport_manager/transport_adapter/transport_adapter_listener_impl.h"

#include "utils/threads/thread_delegate.h"


using namespace application_manager;
using namespace std;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace test::app_manager_test::xml_parser;
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

namespace test {
	namespace app_manager_test{
	
		/**
		* @brief Main class for test 
		*/
		class AdminAppTest : public threads::ThreadDelegate
		{
		public:
			
			//metods
			
			/**
			* @brief Constructor
			*/
			AdminAppTest();
			
			/**
			* @brief Destructor
			*/
			~AdminAppTest();

			/**
			* @brief Run method what creat XMLParser, read XML file
			* creat necessary to us ICheckClass, add him to
			* GMockClassContainer
			*
			* @param
			* 
			* @return void
			*/
			void run();
			
			/**
			* @brief Set path to XML file
			*
			* @param string patch to XML file
			*
			* @return void
			*/
			inline void SetXmlPathToDir(const std::string& path);
			
			/**
			* @brief Thread procedure.
			*/
			virtual void threadMain();

		protected:

		private:
			
			//metods
		
			/**
			* @brief Copy constructor
			*/
			AdminAppTest(const AdminAppTest &);


			/**
			* @brief Operator assignment
			*/
			AdminAppTest *operator=(const AdminAppTest &);
			
			//members

			/**
			* @brief path to XML file
			*/
			std::string xmlPathToDir;

		};
		
		
		inline void AdminAppTest::SetXmlPathToDir(const std::string& path)
		{
		    xmlPathToDir.clear();
		    xmlPathToDir.append(path);
		}
	}
}


#endif //TEST_COMPONENTS_APPLICATION_MANAGER_RPC_INCLUDE_RPC_ADMIN_APP_TEST_H_



