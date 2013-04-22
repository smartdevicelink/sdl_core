// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdlib.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "AppMgr/ApplicationCore.h"
#include "JSONHandler/SDLRPCObjects/V1/Marshaller.h"

namespace test {
    namespace components {
        namespace AppMgr {
            namespace ApplicationCoreTest {

                class CApplicationCoreTest
                : public NsAppManager::ApplicationCore {
                public:
                   /**
                    * Default constructor
                    */
                    CApplicationCoreTest(): NsAppManager::ApplicationCore() {}

                   /**
                    * Default destructor
                    */
                    virtual ~CApplicationCoreTest(){ destructor();}

                    /**
                     * @brief Converts incoming message from NsAppManager::Message to the TSharedPtr<CSmartObject>
                     * @param message Incoming NsAppManager::Message message to be converted
                     *
                     * @return TSharedPtr<CSmartObject> that points to CSmartObject
                     */
                    NsAppManager::tSharedPtr processInMessage(const NsAppManager::Message& message) {
                      // create inner smart object
                      NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
                      obj[0] = (int)message.first->getProtocolVersion();
                      obj[1] = false;
                      obj[2] = NsSmartDeviceLink::NsSmartObjects::CSmartObject();
                      obj[2][0] = message.first->getMessageType();
                      obj[2][1]= NsSmartDeviceLink::NsSmartObjects::CSmartObject();
                      obj[2][1][0] = (int)message.first->getProtocolVersion();
                      NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
                      //test
                      binaryData.push_back(0x74);
                      binaryData.push_back(0x65);
                      binaryData.push_back(0x73);
                      binaryData.push_back(0x74);
                      obj[3] = binaryData;
                      NsSmartDeviceLink::NsSmartObjects::CSmartObject* newobj =
                          new NsSmartDeviceLink::NsSmartObjects::CSmartObject(obj);

                      return NsAppManager::tSharedPtr(newobj);
                    }

                    /**
                     * @brief Converts incoming message from TSharedPtr<CSmartObject> type to the NsAppManager::Message type
                     * @param message Incoming TSharedPtr<CSmartObject>
                     *
                     * @return AppMngrCore::Message for transfered message
                     */
                    NsAppManager::Message processOutMessage(const NsAppManager::tSharedPtr message) {
                      NsSmartDeviceLink::NsSmartObjects::CSmartObject sessionKey = message->getElement(0);
                      NsSmartDeviceLink::NsSmartObjects::CSmartObject msg = message->getElement(1);
                      NsSmartDeviceLinkRPC::SDLRPCMessage::MessageType msgType =
                          static_cast<NsSmartDeviceLinkRPC::SDLRPCMessage::MessageType>(msg[0].asInt());

                      unsigned int protocoloV = static_cast<unsigned int>(msg[1].asInt());
                      return NsAppManager::Message(new NsSmartDeviceLinkRPC::SDLRPCMessage(protocoloV, msgType), sessionKey.asInt());
                    }

                    /**
                     *
                     */
                    MOCK_METHOD0(destructor, void());

                private:
                };

            }  // namespace ApplicationCoreTest
        }  // namespace AppMgr
    }  // namespace components
}  // namespace test

TEST(ApplicationCoreTest, Constructor) {
    test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest * object1 =
        new test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest();

    EXPECT_CALL(*object1, destructor()).Times(0);
    EXPECT_CALL(*object1, destructor()).Times(1);
    delete object1;
}

TEST(ApplicationCoreTest, GetApplicationCoreMessage) {
    // create CApplicationCoreTest instance
    test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest * object1 =
        new test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest();

    // create SDLRPCMessage with protocol = 1
    NsSmartDeviceLinkRPC::SDLRPCMessage* messageObject =
        new NsSmartDeviceLinkRPC::SDLRPCMessage(4);

    NsAppManager::Message msg(messageObject, 1);

    NsAppManager::tSharedPtr smartPtr = object1->processInMessage(msg);
    NsSmartDeviceLink::NsSmartObjects::CSmartObject resObject = smartPtr->getElement(0);

    ASSERT_EQ(resObject.asInt(), messageObject->getProtocolVersion());
    delete object1;
    delete messageObject;
}

TEST(ApplicationCoreTest, GetApplicationCoreSmartPointer) {
    // create CApplicationCoreTest instance
    test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest * object1 =
        new test::components::AppMgr::ApplicationCoreTest::CApplicationCoreTest();

    NsSmartDeviceLink::NsSmartObjects::CSmartObject tmpObj;
    tmpObj[0] = 1;     /// session key
    // Message body
    tmpObj[1] = NsSmartDeviceLink::NsSmartObjects::CSmartObject();
    tmpObj[1][0] = NsSmartDeviceLinkRPC::SDLRPCMessage::REQUEST;
    tmpObj[1][1]= 1;   /// protocol version
    NsSmartDeviceLink::NsSmartObjects::CSmartObject* obj =
        new NsSmartDeviceLink::NsSmartObjects::CSmartObject(tmpObj);

    NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CSmartObject> smartObject(obj);

    NsAppManager::Message msg  = object1->processOutMessage(smartObject);
    NsSmartDeviceLink::NsSmartObjects::CSmartObject sessionKey = smartObject->getElement(0);
    NsSmartDeviceLink::NsSmartObjects::CSmartObject SDLRPCmsg = smartObject->getElement(1);

    ASSERT_EQ(msg.second, sessionKey.asInt());
    ASSERT_EQ(msg.first->getProtocolVersion(), SDLRPCmsg[1].asInt());
    ASSERT_EQ(msg.first->getMessageType(), SDLRPCmsg[0].asInt());

    delete object1;
    delete msg.first;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
