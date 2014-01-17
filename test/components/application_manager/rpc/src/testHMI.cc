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

#include "rpc/testHMI.h"

namespace test{
	namespace app_manager_test{
		void testHMI()
		{
				
			printf("\n\n\n init param to HMI AddCommand \n\n\n");
			
			
			connection_handler::ConnectionHandlerImpl* connection_handler_ = 
										connection_handler::ConnectionHandlerImpl::instance();


			application_manager::ApplicationManagerImpl* app_manager_ = 
										application_manager::ApplicationManagerImpl::instance();
			
			transport_manager::DeviceHandle device_handler_test = 1;
			std::string mac_address_test = "255.255.255.0";
			std::string name_test = "test_DeviceInfo";

			transport_manager::ConnectionUID connection_id_test = 1;
			
			transport_manager::DeviceInfo device_info_test(device_handler_test, mac_address_test, name_test);

			std::vector<transport_manager::DeviceInfo> device_list_test;
			device_list_test.push_back(device_info_test);
			connection_handler_->OnDeviceListUpdated(device_list_test);
			
			
			connection_handler_->OnConnectionEstablished(device_info_test, connection_id_test);
			
			
			//////////////////////////////////////////
			//HMICapabilities 14.1
			//HMICapabilities& hmi_capabilities =
			//	ApplicationManagerImpl::instance()->hmi_capabilities();
			
			//hmi_capabilities.
			//app_manager_->OnHMIStartedCooperation();
			HMICapabilities &hmi_capabilities = app_manager_->hmi_capabilities();
			hmi_capabilities.set_is_vr_cooperating(true);
			hmi_capabilities.set_is_tts_cooperating(true);
			hmi_capabilities.set_is_ui_cooperating(true);
			hmi_capabilities.set_is_navi_cooperating(true);
			hmi_capabilities.set_is_ivi_cooperating(true);
			
			hmi_capabilities.set_active_ui_language(hmi_apis::Common_Language::EN_US);
			hmi_capabilities.set_active_vr_language(hmi_apis::Common_Language::EN_US);
			hmi_capabilities.set_active_tts_language(hmi_apis::Common_Language::EN_US);

			SmartObject hmi_capa;
			hmi_capabilities.set_ui_supported_languages(hmi_capa);
			hmi_capabilities.set_tts_supported_languages(hmi_capa);
			hmi_capabilities.set_vr_supported_languages(hmi_capa);
			hmi_capabilities.set_vehicle_type(hmi_capa);
			
			
			////////////////////////////////////////
			sleep(2);
			printf("\n\n\n Registration Application \n\n\n");
			///////////////////////////////////////////////////
			/*
			Reg app 
					
			utils::SharedPtr<SmartObject> AppRegRequest(new SmartObject);

			(*AppRegRequest)[S_PARAMS][strings::connection_key] = 65537;
			//(*AppRegRequest)[S_MSG_PARAMS][strings::menu_params][strings::menu_name] = "MenuName";
			(*AppRegRequest)[S_MSG_PARAMS][strings::app_name] = "SyncProxyTester";
			
			
			
			(*AppRegRequest)[S_PARAMS][strings::function_id] = mobile_apis::FunctionID::RegisterAppInterfaceID;
			(*AppRegRequest)[S_PARAMS][strings::message_type] = mobile_apis::messageType::request;
			(*AppRegRequest)[S_PARAMS][strings::correlation_id] = 1;
									
			(*AppRegRequest)[S_MSG_PARAMS][strings::available] = true;
			Application* appl = app_manager_->RegisterApplication(AppRegRequest); 
			*/		
			///////////////////////////////////
			//add to 23 december
			
			utils::SharedPtr<SmartObject> AppRegRequest(new SmartObject);
			//test::Mobile_Message_Handler_Tester::MobileMessageHandlerTester* mobile_handler_test;
		    //mobile_handler_test = 
        	//				new test::Mobile_Message_Handler_Tester::MobileMessageHandlerTester;
        					
			//app_manager_->set_mobile_message_handler(mobile_handler_test);
			////////////////////////////////////
			//add 25 december
			//app_manager_->set_active_vr_language(hmi_apis::Common_Language::EN_US);
			//app_manager_->set_active_ui_language(hmi_apis::Common_Language::EN_US);
			/////////////////////////////////////
			(*AppRegRequest)[S_PARAMS][strings::function_id] = mobile_apis::FunctionID::RegisterAppInterfaceID;
			(*AppRegRequest)[S_PARAMS][strings::message_type] = mobile_apis::messageType::request;
			(*AppRegRequest)[S_PARAMS][strings::correlation_id] = 1;
			(*AppRegRequest)[S_PARAMS][strings::connection_key] = 65537;
			
			
			(*AppRegRequest)[S_MSG_PARAMS][strings::app_name] = "SyncProxyTester";					
			(*AppRegRequest)[S_MSG_PARAMS][strings::available] = true;
			
			//(*AppRegRequest)[S_MSG_PARAMS][strings::app_id] = "12345";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::is_media_application] = true;
			//(*AppRegRequest)[S_MSG_PARAMS][strings::vr_synonyms][0] = "VR SyncProxyTester";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::ngn_media_screen_app_name] = "SPT";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::tts_name][0]["text"] = "SyncProxyTester";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::tts_name][0]["type"] = "TEXT";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::app_hmi_type] = mobile_apis::AppHMIType::NAVIGATION;
			//(*AppRegRequest)[S_MSG_PARAMS][strings::cmd_id] = 321;
			//(*AppRegRequest)[S_MSG_PARAMS][strings::menu_params][strings::menu_name] = "MenuName";
			//(*AppRegRequest)[S_MSG_PARAMS][strings::language_desired] = hmi_apis::Common_Language::EN_US;
			//(*AppRegRequest)[S_MSG_PARAMS][strings::hmi_display_language_desired] = hmi_apis::Common_Language::EN_US;
			
			
			//MessageChaining* MesChain = app_manager_->AddMessageChain(connectKey, 1, 16, NULL, &(*AppRegRequest) );
			//printf("\n\n\n after MesChain \n\n\n");
			//app_manager_->AddMessageChain(connectKey,1,16,MesChain, &(*AppRegRequest) );
			//printf("\n\n\n after MesChain  after\n\n\n");
			app_manager_->ManageMobileCommand(AppRegRequest);
			///////////////////////////////////			
			
			
			Application* appl = app_manager_->application(65537);
			
			////////////////////////////////////////
			sleep(20);
			
			
			
			printf("\n\n\n AddCommand Request \n\n\n");

			test::HMI_Message_Handler_Tester::HMIMessageHandlerTester* hmi_handler_test;
			
			hmi_handler_test = new test::HMI_Message_Handler_Tester::HMIMessageHandlerTester;

			utils::SharedPtr<SmartObject> HMIRequest(new SmartObject);
			

			
			//NsSmartDeviceLink::NsJSONHandler::strings::
			//mobile_apis::FunctionID::RegisterAppInterfaceID;

			
			(*HMIRequest)[strings::params][strings::function_id] 		= mobile_apis::FunctionID::AddCommandID;
			(*HMIRequest)[S_PARAMS][strings::correlation_id] 	= 1;
			(*HMIRequest)[S_PARAMS][strings::message_type] 		= mobile_apis::messageType::request;
			(*HMIRequest)[S_PARAMS][strings::protocol_type] 	= 0;
			(*HMIRequest)[S_PARAMS][strings::protocol_version]	= 2;
			(*HMIRequest)[S_PARAMS][strings::connection_key] 	= appl->app_id();
			
			(*HMIRequest)[S_MSG_PARAMS][strings::app_id] = "12345";
			(*HMIRequest)[S_MSG_PARAMS][strings::app_name] = "SyncProxyTester";

			(*HMIRequest)[S_MSG_PARAMS][strings::cmd_id] = 321;
			(*HMIRequest)[S_MSG_PARAMS][strings::success] = true;
			(*HMIRequest)[S_MSG_PARAMS][strings::hmi_level] = 5;
			
			(*HMIRequest)[S_MSG_PARAMS][strings::language_desired] = hmi_apis::Common_Language::EN_US;
			(*HMIRequest)[S_MSG_PARAMS][strings::hmi_display_language_desired] = hmi_apis::Common_Language::EN_US;

			(*HMIRequest)[S_MSG_PARAMS][strings::is_media_application] = true;
			(*HMIRequest)[S_MSG_PARAMS][strings::device_name] = device_handler_test;

			//(*HMIRequest)[S_MSG_PARAMS][strings::menu_params][strings::vr_commands] = mobile_apis::Result::RETRY;
			(*HMIRequest)[S_MSG_PARAMS][strings::menu_params][strings::menu_name] = "MenuName";


			/*
			(*spRequest)[S_MSG_PARAMS][strings::sync_msg_version]["majorVersion"] = 2;
			(*spRequest)[S_MSG_PARAMS][strings::sync_msg_version]["minorVersion"] = 2;
			(*spRequest)[S_MSG_PARAMS][strings::tts_name][0]["text"] = "SyncProxyTester";
			(*spRequest)[S_MSG_PARAMS][strings::tts_name][0]["type"] = "TEXT";
			(*spRequest)[S_MSG_PARAMS][strings::ngn_media_screen_app_name] = "SPT";
			(*spRequest)[S_MSG_PARAMS][strings::vr_synonyms][0] = "VR SyncProxyTester";
			(*spRequest)[S_MSG_PARAMS][strings::is_media_application] = true;
			(*spRequest)[S_MSG_PARAMS][strings::app_type][0] = "DEFAULT";
			
			*/

			
			app_manager_->set_hmi_message_handler(hmi_handler_test);
			/*
			//////////////////////////////////////
			//change 25 december
		  	app_manager_->set_active_vr_language(hmi_apis::Common_Language::EN_US);
			app_manager_->set_active_ui_language(hmi_apis::Common_Language::EN_US);
			
			/////////////////////////////////
			*/
			//Application* appRegistr = app_manager_->application(appl->app_id());
			//if (appRegistr)
			//	printf("\n\n\n blablabla \n\n\n");

			appl->set_hmi_level(mobile_apis::HMILevel::HMI_BACKGROUND);

			app_manager_->ManageMobileCommand(HMIRequest);

			
			sleep(20);
		}
	}//namespace app_manager_test
}//namespace test





