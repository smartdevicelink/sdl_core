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


#include "rpc/AdminAppTest.h"
#include "../../../../../src/appMain/life_cycle.h"


namespace test {
	namespace app_manager_test {

		AdminAppTest::AdminAppTest()
		        //xmlPathToDir()
		{		
		}


		AdminAppTest::~AdminAppTest()
		{
		}


		void AdminAppTest::threadMain()
		{
			this->run();
		}


		void AdminAppTest::run()
		{
			application_manager::ApplicationManagerImpl* app_manager_ = 
						application_manager::ApplicationManagerImpl::instance();
			
			//Start init dependence appManagerImpl,HMIImpl,transport...
			initStartData();
			
			sleep(20);
			printf("\n\n\n after init in Admin \n\n\n");
			//////////////////////////////////////////////
			//Test AddCommand
			
			testHMI();
			app_manager_->UnregisterAllApplications();
			//////////////////////////////////////////////
			//Test Mobile
			
			//testMobile();

			//////////////////////////////////////////////
			//check add Test
			
			//addTest("www", "aaa", foo, 65);
			
			
			//////////////////////////////////////////////			

			/*
			too_many_request_test_case *requestTestCase = 
										new too_many_request_test_case("too_many_request_test_case", "first");
			
			GMockClassContainer *container = GMockClassContainer::instance();
			
			container->addTest(requestTestCase);
			*/
			
			//////////////////////////////////////////////
			/*
			too_many_pending_requests_test_case *requestPendingTestCase = new too_many_pending_requests_test_case("too_many_pending_requests_test_case", "first");
			
			GMockClassContainer *container = GMockClassContainer::instance();
			
			container->addTest(requestPendingTestCase);
			*/
			//////////////////////////////////////////////
			
			
		}
	}//namespace app_manager_test
}//namespace test



