/**
* \file request_watchdog_test.cc
* \brief RequetWatchdog test source file.
*
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

#ifndef TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_TM_TEST_H_
#define TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_TM_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#include "transport_manager/transport_manager.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "connection_handler/connection_handler_impl.h"

namespace test  {
namespace components  {
namespace protocol_handler_test {

//	using namespace transport_manager::TransportManager;
	using namespace protocol_handler;

//class MockTransportManager : public TransportManagerImpl {
//public:
//	MOCK_METHOD4(sendFrame, void(tConnectionHandle ConnectionHandle,
//		const uint8_t * Data, size_t DataSize, const int UserData));
//
//	void run(void) {}
//    void scanForNewDevices(void) {}
//    void connectDevice(const tDeviceHandle DeviceHandle) {}
//    void disconnectDevice(const tDeviceHandle DeviceHandle) {}
//    void addDataListener(ITransportManagerDataListener * Listener) {}
//    void removeDataListener(ITransportManagerDataListener * Listener) {}
//    void addDeviceListener(ITransportManagerDeviceListener * Listener) {}
//    void removeDeviceListener(ITransportManagerDeviceListener * Listener) {}
//};

	TEST(interface_test, test_result_success){
		//Segfault
		/*MockTransportManager tm;
		ProtocolHandlerImpl * handler = new ProtocolHandlerImpl(&tm);
		SessionObserver * connectionHandler  = connection_handler::ConnectionHandlerImpl::getInstance();
		handler->set_session_observer(connectionHandler);

		std::string messageString = "";

		unsigned char* rawMessage = new unsigned char[messageString.length() + 1];
    	memcpy(rawMessage, messageString.c_str(), messageString.length() + 1);

		RawMessage* msgToProtocolHandler = new protocol_handler::RawMessage(
	        1,
	        1,
	        rawMessage,
	        messageString.length() + 1);

		handler->sendMessageToMobileApp(msgToProtocolHandler);

		EXPECT_CALL(tm, sendFrame(1, 0, 0, 0))
            .Times(1);*/

	}
}
}
}

#endif  //TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_TM_TEST_H_
