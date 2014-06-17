/**
* \file request_info.h
* \brief request information structure header file.
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

#ifndef SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_INFO_H_
#define SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_INFO_H_

#include <stdint.h>

namespace request_watchdog {

struct RequestInfo {
  RequestInfo(const int32_t& function_id, const int32_t& connection_id,
              const int32_t& correlation_id, const int32_t& custom_timeout,
              const int32_t& app_hmi_level);

  int32_t functionID_;
  uint32_t connectionID_;
  uint32_t correlationID_;
  uint32_t customTimeout_;
  int32_t app_hmi_level_;
  bool delayed_delete_;

  friend bool operator==(const RequestInfo& left, const RequestInfo& right);

  friend bool operator<(const RequestInfo& left, const RequestInfo& right);
};

}  //  namespace request_watchdog

#endif  // SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_INFO_H_
