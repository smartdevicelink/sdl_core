/**
 * @file meta_formatter_test_helper.h
 * @brief file describes class CMetaFormatterTestHelper which is designed to 
 *        create test environemnt to test class CMetaFormatter
 */
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

#ifndef __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTERTESTHELPER_H__
#define __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTERTESTHELPER_H__

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "test/components/JSONHandler/formatters/test_JSONHandler_alrpcv2.hpp"

#include "SmartObjects/CSmartObject.hpp"

namespace generated_ns = Gen::test::components::JSONHandler2;

namespace test {
namespace components {
namespace JSONHandler {
namespace formatters {

namespace FunctionID {
  /**
    * @brief Enumeration FunctionID.
    *
    * Enumeration linking function names with function IDs in WiPro protocol.
    * Assumes enumeration starts at value 0.
    */
  enum eType {
    /**
      * @brief INVALID_ENUM.
      */
    INVALID_ENUM = -1,

    /**
    * @brief RegisterAppInterface.
    */
    kRegisterAppInterfaceID,

    /**
      * @brief UnregisterAppInterface.
      */
    kUnregisterAppInterfaceID,

    /**
      * @brief SetGlobalProperties.
      */
    kSetGlobalPropertiesID,
  };
}

namespace messageType {
  /**
    * @brief Enumeration messageType.
    *
    * Enumeration linking message types with function types in WiPro protocol.
    * Assumes enumeration starts at value 0.
    */
  enum eType {
    /**
      * @brief INVALID_ENUM.
      */
    INVALID_ENUM = -1,

    /**
      * @brief request.
      */
    kRequest = 0,

    /**
      * @brief response.
      */
    kResponse = 1,

    /**
      * @brief notification.
      */
    kNotification = 2
  };
}
  
  class CMetaFormatterTestHelper :public ::testing::Test {
   protected:

    virtual void SetUp();

    virtual void TearDown();

    void AnyObjectToJsonString(
       const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj,
       std::string& result_string);

    // for messageType::request, FunctionID::RegisterAppInterfaceID
    void FillObjectIdenticalToSchema(
      NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

    // for messageType::request, FunctionID::RegisterAppInterfaceID
    void FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

    void CompareObjects(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& first,
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& second);

    // for messageType::request, FunctionID::RegisterAppInterfaceID
    void FillObjectWithDefaultValues(
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

    void FillObjectWithoutSomeMandatoryFields(
      NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

    // members
    generated_ns::test_JSONHandler_alrpcv2 factory_;
    std::set<FunctionID::eType> function_id_items_;
    std::set<messageType::eType> message_type_items_;

    static const bool kIsPrintOut = false;    
  };

}
}
}
}

#endif // __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTERTESTHELPER_H__