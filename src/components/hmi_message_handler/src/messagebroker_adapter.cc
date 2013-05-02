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

#include "hmi_message_handler/messagebroker_adapter.h"

namespace hmi_message_handler {
MessageBrokerAdapter::MessageBrokerAdapter(HMIMessageHandler* handler)
	: NsMessageBroker::CMessageBrokerController(std::string("127.0.0.1"), 8087, "SDL")
	, HMIMessageAdapter(handler) {

}

MessageBrokerAdapter::~MessageBrokerAdapter() {

}

void MessageBrokerAdapter::sendMessageToHMI(
		application_manager::Message * message) {
	if (!message) {
		//LOG
		return;
	}

	Json::Reader reader;
    Json::Value json_value;
    if (!reader.parse(message->json_message(), json_value, false))
    {
        //LOG4CPLUS_ERROR(mLogger, "Received invalid json string.");
        return;
    }

	sendJsonMessage( json_value );
}

void MessageBrokerAdapter::processResponse(std::string method, Json::Value& root) {
	processRecievedfromMB(root);
}

void MessageBrokerAdapter::processRequest(Json::Value& root) {
	processRecievedfromMB(root);
}

void MessageBrokerAdapter::processNotification(Json::Value& root) {
	processRecievedfromMB(root);
}

void MessageBrokerAdapter::subscribeTo() {
	NsMessageBroker::CMessageBrokerController::subscribeTo("Buttons.OnButtonEvent");
    NsMessageBroker::CMessageBrokerController::subscribeTo("Buttons.OnButtonPress");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnCommand");
    NsMessageBroker::CMessageBrokerController::subscribeTo("VR.OnCommand");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnReady");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnDriverDistraction");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnSystemContext");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnAppActivated");
    NsMessageBroker::CMessageBrokerController::subscribeTo("BasicCommunication.OnAppDeactivated");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnDeviceChosen");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnLanguageChange");
    NsMessageBroker::CMessageBrokerController::subscribeTo("VR.OnLanguageChange");
    NsMessageBroker::CMessageBrokerController::subscribeTo("TTS.OnLanguageChange");
    NsMessageBroker::CMessageBrokerController::subscribeTo("VehicleInfo.OnVehicleData");
    NsMessageBroker::CMessageBrokerController::subscribeTo("UI.OnTBTClientState");
}

void MessageBrokerAdapter::processRecievedfromMB(Json::Value& root) {
	if (root.isNull()) {
		//LOG
		return;
	}

	Json::FastWriter writer;
	std::string message_string = writer.write(root);

	if (message_string.empty()) {
		//LOG
		return;
	}

	if (!handler()) {
		//WARNING
		return;
	}

	application_manager::Message * message = 
		new application_manager::Message;
	//message->set_message_type()
	message->set_json_message(message_string);
	message->set_protocol_version(
		application_manager::ProtocolVersion::kHMI);

	handler()->onMessageReceived(message);
}

}