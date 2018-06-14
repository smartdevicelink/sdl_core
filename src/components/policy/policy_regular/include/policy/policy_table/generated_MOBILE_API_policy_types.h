/**
 * @file generated_MOBILE_API_policy_types.h
 * @brief Generated class generated_MOBILE_API_policy_types header file.
 *
* Copyright (c) 2017, Ford Motor Company
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

#ifndef _GENERATED_MOBILE_API_POLICY_TYPES_31feacaeebd611e7bc4508606ee6b839_H__
#define _GENERATED_MOBILE_API_POLICY_TYPES_31feacaeebd611e7bc4508606ee6b839_H__
#include <string>

namespace rpc {
namespace policy_table_interface_base {
/**
 * @brief Enumeration AppHMIType.
 *
 * Enumeration listing possible app types.
 */
enum AppHMIType {
  /**
   * @brief INVALID_ENUM.
   */
  INVALID_ENUM = -1,

  /**
   * @brief DEFAULT.
   */
  DEFAULT,

  /**
   * @brief COMMUNICATION.
   */
  COMMUNICATION,

  /**
   * @brief MEDIA.
   */
  MEDIA,

  /**
   * @brief MESSAGING.
   */
  MESSAGING,

  /**
   * @brief NAVIGATION.
   */
  NAVIGATION,

  /**
   * @brief INFORMATION.
   */
  INFORMATION,

  /**
   * @brief SOCIAL.
   */
  SOCIAL,

  /**
   * @brief BACKGROUND_PROCESS.
   */
  BACKGROUND_PROCESS,

  /**
   * @brief TESTING.
   */
  TESTING,

  /**
   * @brief SYSTEM.
   */
  SYSTEM,

  /**
   * @brief PROJECTION.
   */
  PROJECTION,

  /**
   * @brief REMOTE_CONTROL.
   */
  REMOTE_CONTROL
};
bool IsValidEnum(AppHMIType val);
const char* EnumToJsonString(AppHMIType val);
bool EnumFromJsonString(const std::string& literal, AppHMIType* result);
}  // rpc
}  // policy_table_interface_base
#endif  //_GENERATED_MOBILE_API_POLICY_TYPES_31feacaeebd611e7bc4508606ee6b839_H__
