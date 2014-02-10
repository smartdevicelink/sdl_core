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

#include "rpc/initStartData.h"
#include "../../../../../src/appMain/life_cycle.h"

void initStartData()
{
	transport_manager::TransportManager* transport_manager_;
	protocol_handler::ProtocolHandlerImpl* protocol_handler_;
	connection_handler::ConnectionHandlerImpl* connection_handler_;
	application_manager::ApplicationManagerImpl* app_manager_;
#ifdef WEB_HMI
	hmi_message_handler::MessageBrokerAdapter* mb_adapter_;
#endif
#ifdef QT_HMI
	hmi_message_handler::DBusMessageAdapter* dbus_adapter_;
#endif

	//policies::PolicyManagerImpl* policy_manager_;

	media_manager::MediaManagerImpl* media_manager_;
			
			
	//mobile_message_handler::MobileMessageHandlerImpl* mmh_;
	hmi_message_handler::HMIMessageHandlerImpl* hmi_handler_;			
			
			
	log4cxx::PropertyConfigurator::configure("./../src/appMain/log4cxx.properties");



			
	// --------------------------------------------------------------------------
	// Third-Party components initialization.
	//if (!main_namespace::LifeCycle::instance()->InitMessageBroker()) {

	//    exit(EXIT_FAILURE);
	//}

	
	// --------------------------------------------------------------------------
	// Components initialization
//  transport_manager::TransportManagerAttr tma;
//	tma.disconnectTimeout = 1000;
	transport_manager_ =
	//				new transport_manager::TransportManagerImpl(tma);
			transport_manager::TransportManagerDefault::Instance();
	DCHECK(transport_manager_);

	protocol_handler_ =
			new protocol_handler::ProtocolHandlerImpl(transport_manager_);
	DCHECK(protocol_handler_);

	//mmh_ =
	//		mobile_message_handler::MobileMessageHandlerImpl::instance();
	//DCHECK(mmh_);

	connection_handler_ =
			connection_handler::ConnectionHandlerImpl::instance();
	DCHECK(connection_handler_);

	app_manager_ =
			application_manager::ApplicationManagerImpl::instance();
	DCHECK(app_manager_);				

	hmi_handler_ =
			hmi_message_handler::HMIMessageHandlerImpl::instance();
	DCHECK(hmi_handler_);		
	
	//policy_manager_ = policies::PolicyManagerImpl::instance();
	//DCHECK(policy_manager_);
	
	media_manager_ = media_manager::MediaManagerImpl::instance();

	////////////////////////////////////////
	/*
	//Add 25 december
	printf("\n\n\n add device \n\n\n");
	transport_manager::DeviceHandle device_handler_test = 1;
	std::string mac_address_test = "255.255.255.0";
	std::string name_test = "test_DeviceInfo";

	transport_manager::ConnectionUID connection_id_test = 1;
			
	transport_manager::DeviceInfo device_info_test(device_handler_test, mac_address_test, name_test);

	std::vector<transport_manager::DeviceInfo> device_list_test;
	device_list_test.push_back(device_info_test);
	connection_handler_->OnDeviceListUpdated(device_list_test);
			
			
	connection_handler_->OnConnectionEstablished(device_info_test, connection_id_test);
	*/
	////////////////////////////////////
	
	
			
	//transport_manager_->SetProtocolHandler(protocol_handler_);
	transport_manager_->AddEventListener(protocol_handler_);
	transport_manager_->AddEventListener(connection_handler_);
	//printf("\n\n\n after init transport_manager \n\n\n");
	

	hmi_handler_->set_message_observer(app_manager_);
	//printf("\n\n\n after init hmi \n\n\n");		

	media_manager_->SetProtocolHandler(protocol_handler_);
	//printf("\n\n\n after init media_manager \n\n\n");
	protocol_handler_->set_session_observer(connection_handler_);						
	//protocol_handler_->AddProtocolObserver(mmh_);			
	protocol_handler_->AddProtocolObserver(media_manager_);
	protocol_handler_->AddProtocolObserver(app_manager_);
	//printf("\n\n\n after init protocol_handler \n\n\n");
	connection_handler_->set_transport_manager(transport_manager_);
	connection_handler_->set_connection_handler_observer(app_manager_);
	//printf("\n\n\n after init connection_handler \n\n\n");
	//mmh_->set_protocol_handler(protocol_handler_);
	//mmh_->AddMobileMessageListener(app_manager_);
	//printf("\n\n\n after init mmh \n\n\n");
	// It's important to initialise TM after setting up listener chain
	// [TM -> CH -> AM], otherwise some events from TM could arrive at nowhere
	transport_manager_->Init();
		
	//app_manager_->set_mobile_message_handler(mmh_);
	app_manager_->set_connection_handler(connection_handler_);
	app_manager_->set_hmi_message_handler(hmi_handler_);
	app_manager_->set_protocol_handler(protocol_handler_);
	
	//app_manager_->set_policy_manager(policy_manager_);

	//printf("\n\n\n after init app \n\n\n");			
	
	if (profile::Profile::instance()->server_address() ==
			std::string(InitializeHMI::kLocalHostAddress)) {
	
		if (!InitializeHMI::InitHmi()) {
			exit(EXIT_FAILURE);
		}

	}
	
	printf("\n\n\n after first init \n\n\n");
	

	//transport_manager::transport_adapter::TransportAdapterController* TAC = 
	//					new transport_manager::transport_adapter::TransportAdapterController;
						
	//transport_manager::transport_adapter::TcpTransportAdapter* ta =
	//					new transport_manager::transport_adapter::TcpTransportAdapter();
						
	/*
	transport_manager::transport_adapter::DnssdServiceBrowser* DSB = 
						new transport_manager::transport_adapter::DnssdServiceBrowser(ta);
	
	transport_manager::transport_adapter::ServerConnectionFactory* SCF = 
						new transport_manager::transport_adapter::TcpConnectionFactory(ta);
	
	transport_manager::transport_adapter::ClientConnectionListener* CCL = 
						new transport_manager::transport_adapter::TcpClientListener(ta,80);
	*/					

	//ta = new transport_manager::transport_adapter::TransportAdapter(DSB,SCF,CCL);
	
	//utils::SharedPtr<TransportAdapter> taSprt(ta);
	
	//transport_manager_->AddTransportAdapter(taSprt);
	/*	
	
	//transport_manager::transport_adapter::

	utils::SharedPtr<protocol_handler::RawMessage> rawMessageSptr;//typedef utils::SharedPtr<protocol_handler::RawMessage> RawMessageSptr;
	int type = 0;//TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE;
			
	transport_manager::DeviceUID DUID;//typedef std::string DeviceUID;
	int app_handle = 0;//typedef int ApplicationHandle; 
			
	transport_manager::BaseError *br = new transport_manager::BaseError;
	transport_manager::TransportAdapterEvent *tae = 
		new transport_manager::TransportAdapterEvent(type, taSprt, DUID, app_handle, rawMessageSptr, br);
			
	transport_manager_->ReceiveEventFromDevice(*tae);
	*/	
			
}















