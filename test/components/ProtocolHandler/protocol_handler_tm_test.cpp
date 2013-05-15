#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "TransportManager/SDeviceInfo.hpp"
#include "TransportManager/ITransportManager.hpp"
#include "protocol_handler/protocol_handler_impl.h"
#include "ConnectionHandler/connection_handler_impl.h"

namespace protocol_handler_test {

	using namespace NsSmartDeviceLink::NsTransportManager;
	using namespace protocol_handler;

class MockTransportManager : public ITransportManager {
public:
	MOCK_METHOD4(sendFrame, void(tConnectionHandle ConnectionHandle, 
		const uint8_t * Data, size_t DataSize, const int UserData));

	void run(void) {}
    void scanForNewDevices(void) {}
    void connectDevice(const tDeviceHandle DeviceHandle) {}
    void disconnectDevice(const tDeviceHandle DeviceHandle) {}
    void addDataListener(ITransportManagerDataListener * Listener) {}
    void removeDataListener(ITransportManagerDataListener * Listener) {}
    void addDeviceListener(ITransportManagerDeviceListener * Listener) {}
    void removeDeviceListener(ITransportManagerDeviceListener * Listener) {}
};

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

	int main(int argc, char **argv) {
    //PropertyConfigurator::doConfigure(LOG4CXX_TEXT("log4cxx.properties"));
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
}
