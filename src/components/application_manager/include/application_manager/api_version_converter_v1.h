//
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_V1_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_V1_H_

#include <map>
#include <string>

#include "Utils/macro.h"
#include "SmartObjects/CSmartObject.hpp"
#include "application_manager/api_version_converter.h"
#include "v4_protocol_v2_0_revT.hpp"
#include "v4_protocol_v1_2_no_extra.hpp"

namespace application_manager {

/**
 * @brief typedef for API V2 enum string representation
 */
typedef std::map<NsSmartDeviceLinkRPC::V2::FunctionID::eType, std::string> FunctionMap;

/**
 * @brief typedef for API V2 enum string representation
 */
typedef std::map<NsSmartDeviceLinkRPC::V2::Result::eType,
    NsSmartDeviceLinkRPC::V1::Result::eType> ResultCodeMap;

/**
 * @brief class APIVersionConverterV1 is intended to convert application API
 * from V1 to to internal representation which is API V2.
 */
class APIVersionConverterV1 : public APIVersionConverter {
 public:
  /**
   * @brief Class constructor
   */
  APIVersionConverterV1();

  /**
   * @brief Class destructor
   */
  virtual ~APIVersionConverterV1();

  /**
   * @brief Converts incoming message from API V1 to API V2
   * @param smartObj Incoming SmartObject to be converted
   *
   * @return SharedPtr<CSmartObject> for transfered message converted to API V2
   */
  virtual tSharedPtr convertToV2(
      const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj);

  /**
   * @brief Converts incoming message from API V2 to API V1
   * @param smartObj Incoming SmartObject to be converted
   *
   * @return SharedPtr<CSmartObject> for transfered message converted to API V1
   */
  virtual tSharedPtr convertFromV2(
      const NsSmartDeviceLink::NsSmartObjects::CSmartObject& smartObj);

  /**
   * @brief Provides information if message is valid after conversion
   *
   * @return TRUE if message is valid, otherwise FALSE.
   */
  virtual bool isMessageValid() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(APIVersionConverterV1);

  /**
   * @brief Search for function ID in API V2 for given function name
   * @param functionName Incoming function name string
   *
   * @return On success function ID in API V2, otherwise default value -1.
   */
  int findV2FunctionId(const std::string& functionName) const;

  /**
   * @brief Search for function name in API V1 for given API V2 function ID
   * @param functionId Incoming function ID in API V2
   *
   * @return On success function name in API V1, otherwise empty string.
   */
  std::string findV1FunctionName(int functionId) const;

  /**
   * @brief Search for corresponding API V1 result code from API V2 result code
   * @param resultCode Incoming API V2 result code
   *
   * @return On success corresponding API V1 result code, otherwise INVALID_DATA result.
   */
  int findV1ResultCode(int resultCode);

  // SmartObject key for result code
  static const std::string RESULT_CODE;

  // API V1 functions name constants
  static const std::string REG_APP_INTERFACE;
  static const std::string UNREG_APP_INTERFACE;
  static const std::string SET_GLOBAL_PROPERTIES;
  static const std::string RESET_GLOBAL_PROPERTIES;
  static const std::string ADD_COMMAND;
  static const std::string DELETE_COMMAND;
  static const std::string ADD_SUBMENU;
  static const std::string DELETE_SUBMENU;
  static const std::string CREATE_INTERACTION_CHOICE_SET;
  static const std::string PERFORM_INTERACTION;
  static const std::string DELETE_INTERACTION_CHOICE_SET;
  static const std::string ALERT;
  static const std::string SHOW;
  static const std::string SPEAK;
  static const std::string SET_MEDIA_CLOCK_TIMER;
  static const std::string ENCODED_SYNC_PDATA;
  static const std::string SUBSRIBE_BUTTON;
  static const std::string UNSUBSRIBE_BUTTON;
  static const std::string GENERIC_RESPONSE;
  static const std::string ON_HMI_STATUS;
  static const std::string ON_APP_INTERFACE_UNREG;
  static const std::string ON_BUTTON_EVENT;
  static const std::string ON_BUTTON_PRESS;
  static const std::string ON_COMMAND;
  static const std::string ON_ENCODED_SYNC_PDATA;
  static const std::string ON_TBT_CLIENT_STATE;
  static const std::string ON_DRIVE_DISTRACTION;

  // API V1 enum string representation
  static FunctionMap       functionMap_;
  static ResultCodeMap     resultCodeMap_;

  // indicates if message is correct
  bool                     messageValid_;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_API_VERSION_CONVERTER_V1_H_
