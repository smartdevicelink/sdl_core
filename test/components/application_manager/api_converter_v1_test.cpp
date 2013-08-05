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
//#include "application_manager/api_version_converter_v1.h"

#include "formatters/CFormatterJsonSDLRPCv1.hpp"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "MOBILE_API.h"
#include "v4_protocol_v1_2_no_extra.h"
#include "formatters/CSmartFactory.hpp"

namespace test {
namespace components {
namespace application_mngr {
namespace APIVersionConverterV1Test {

class CAPIVersionConverterV1Test {
  public:
    /**
     * Class constructor
     */
    CAPIVersionConverterV1Test() {}

    /**
     * Class destructor
     */
    virtual ~CAPIVersionConverterV1Test() {
      destructor();
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

TEST(CAPIVersionConverterV1Test, Constructor) {
  using namespace test::components::application_mngr::APIVersionConverterV1Test;

  CAPIVersionConverterV1Test* pVersionConverter = new CAPIVersionConverterV1Test();

  EXPECT_CALL(*pVersionConverter, destructor()).Times(0);
  EXPECT_CALL(*pVersionConverter, destructor()).Times(1);
  delete pVersionConverter;
}

TEST(CAPIVersionConverterV1Test, convertJSON) {
  using namespace NsSmartDeviceLink::NsJSONHandler::strings;
  using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;
  using namespace NsSmartDeviceLink::NsSmartObjects;
  using namespace NsSmartDeviceLinkRPC::V1;

  // create CApplicationCoreTest instance
  std::string inputJsonString = "\
        {\
            \"request\": {\
                \"name\" : \"RegisterAppInterace\",\
                \"correlationID\": 0,\
                \"parameters\": {\
                    \"syncMsgVersion\" : {\
                        \"majorVersion\" : 2,\
                        \"minorVersion\" : 10\
                    },\
                    \"appName\": \"some app name\",\
                    \"ttsName\": [{\
                        \"text\": \"ABC\",\
                        \"type\": \"TEXT\"\
                    }],\
                   \"vrSynonyms\": [\"Synonym 1\", \"Synonym 2\"]\
                }\
            }\
        }";

  SmartObject obj;
  bool result;
  result = CFormatterJsonSDLRPCv1::fromString<FunctionID::eType, messageType::eType>(inputJsonString, obj);

  if (false == result) {
    printf("Formatter result is FALSE\n");
  } else {
    printf("Formatter result is TRUE\n");
  }

  printf("#### %d \n", obj[S_PARAMS][S_FUNCTION_ID].asInt());

  /*
  std::set<std::string> tmp2 = newobj.enumerate();
  for (std::set<std::string>::iterator it = tmp2.begin(); it != tmp2.end(); ++it) {
      printf("%s\n", (*it).c_str());
  }
  */
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
