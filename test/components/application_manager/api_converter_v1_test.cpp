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
#include <string>
#include <set>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "application_manager/api_version_converter_v1.h"
#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "JSONHandler/CSmartFactory.hpp"

namespace test {
    namespace components {
        namespace application_mngr {
            namespace APIVersionConverterV1Test {

                class CAPIVersionConverterV1Test: public application_manager::APIVersionConverterV1 {
                public:
                   /**
                    * Class constructor
                    */
                   CAPIVersionConverterV1Test()
                   : application_manager::APIVersionConverterV1() {}

                   /**
                    * Class destructor
                    */
                   virtual ~CAPIVersionConverterV1Test() { destructor();}

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

TEST(CAPIVersionConverterV1Test, Constructor) {
   using namespace test::components::application_mngr::APIVersionConverterV1Test;

   CAPIVersionConverterV1Test* pVersionConverter = new CAPIVersionConverterV1Test();

   EXPECT_CALL(*pVersionConverter, destructor()).Times(0);
   EXPECT_CALL(*pVersionConverter, destructor()).Times(1);
   delete pVersionConverter;
}

TEST(CAPIVersionConverterV1Test, convertToV2) {
   using namespace test::components::application_mngr::APIVersionConverterV1Test;
   using namespace application_manager;
   using namespace NsSmartDeviceLink::NsJSONHandler::strings;
   using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;
   using namespace NsSmartDeviceLink::NsSmartObjects;

   // create CApplicationCoreTest instance
        std::string inputJsonString = "\
        {\
            \"request\": {\
                \"name\" : \"RegisterAppInterface\",\
                \"correlationID\": 11,\
                \"parameters\": {\
                    \"syncMsgVersion\": \"version\",\
                    \"appName\": \"some app name\",\
                    \"ttsName\": [{\
                        \"text\": \"ABC\",\
                        \"type\": \"TEXT\"\
                    }],\
                   \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                }\
            }\
        }";

    // create new CSmartObject from json string
    CSmartObject* pObj = new CSmartObject();
    CFormatterJsonALRPCv1::fromString(inputJsonString, *pObj);

    CSmartObject params = pObj->getElement(S_PARAMS);
    std::set<std::string> tmp2 = params.enumerate();
    for (std::set<std::string>::iterator it = tmp2.begin(); it != tmp2.end(); ++it) {
        printf("%s\n", (*it).c_str());
    }

    printf("\nFUNCTION_ID - %s\n", params[S_FUNCTION_ID].asString().c_str());

    // call version control interface
    CAPIVersionConverterV1Test versionConverter;
    application_manager::tSharedPtr smartPtr =
        versionConverter.convertToV2(*pObj);
    CSmartObject tmpParams = smartPtr->getElement(S_PARAMS);
    printf("FUNCTION_ID - %d\n", tmpParams[S_FUNCTION_ID].asInt());

    int functionId =
        NsSmartDeviceLinkRPC::V2::FunctionID::RegisterAppInterfaceID;

    ASSERT_EQ(functionId, tmpParams[S_FUNCTION_ID].asInt());
}

TEST(CAPIVersionConverterV1Test, convertFromV2) {
    using namespace test::components::application_mngr::APIVersionConverterV1Test;
    using namespace NsSmartDeviceLink::NsSmartObjects;
    using namespace NsSmartDeviceLink::NsJSONHandler::strings;
    using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

    // create SmartObject for API V2
    CSmartObject obj;
    obj[S_PARAMS][S_FUNCTION_ID] =
        NsSmartDeviceLinkRPC::V2::FunctionID::RegisterAppInterfaceID;

    // call version control interface
    CAPIVersionConverterV1Test versionConverter;
    application_manager::tSharedPtr smartPtr =
        versionConverter.convertFromV2(obj);
    CSmartObject tmpParams = smartPtr->getElement(S_PARAMS);

    const std::string functionId = "RegisterAppInterface";
    ASSERT_EQ(functionId, tmpParams[S_FUNCTION_ID].asString());
    printf("FUNCTION_ID - %s\n", tmpParams[S_FUNCTION_ID].asString().c_str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
