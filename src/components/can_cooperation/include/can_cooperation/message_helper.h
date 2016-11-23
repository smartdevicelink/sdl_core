/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_MESSAGE_HELPER_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_MESSAGE_HELPER_H_

#include <stdint.h>
#include <string>
#include <map>

#include "utils/macro.h"
#include "json/json.h"
#include "functional_module/function_ids.h"

namespace can_cooperation {

/**
 * @brief MessageHelper class
 **/
class MessageHelper {
 public:
  /**
   * @brief Returns unique correlation ID for next CAN request
   *
   * @return Unique correlation ID
   */
  static uint32_t GetNextCANCorrelationID();
  static const std::string GetMobileAPIName(
      functional_modules::MobileFunctionID func_id);

  /**
   * @brief Convert Json::Value to std::string
   *
   * @param value Value with json
   *
   * @return string with json
   */
  static std::string ValueToString(const Json::Value& value);

  /**
   * @brief Convert std::string to Json::Value
   *
   * @param string string with json
   *
   * @return Value created from string with json
   */
  static Json::Value StringToValue(const std::string& string);

  /**
   * Validates structure DeviceInfo
   * @param value json of DeviceInfo
   * @return true if json is valid
   */
  static bool ValidateDeviceInfo(const Json::Value& value);

  /**
   * Validates structure InteriorZone
   * @param value json of InteriorZone
   * @return true if json is valid
   */
  static bool ValidateInteriorZone(const Json::Value& value);

 private:
  DISALLOW_COPY_AND_ASSIGN(MessageHelper);

  MessageHelper();

  static uint32_t next_correlation_id_;
  static const std::map<functional_modules::MobileFunctionID, std::string>
      kMobileAPINames;
};

/**
 * @brief Check for existence of specified key in Json::Value
 *
 * @param value Value with json
 * @param key string with key name
 *
 * @return true if key exist
 */
bool IsMember(const Json::Value& value, const std::string& key);

}  // namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_MESSAGE_HELPER_H_
