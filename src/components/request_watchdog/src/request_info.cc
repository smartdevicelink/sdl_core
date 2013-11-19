/**
* \file request_info.h
* \brief request information structure source file.
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

#include "request_watchdog/request_info.h"

namespace request_watchdog {

RequestInfo::RequestInfo(int FunctionId, int ConnectionID
                         , int CorrelationId, int CustomTimeout)
  : functionID_(FunctionId),
    connectionID_(ConnectionID),
    correlationID_(CorrelationId),
    customTimeout_(CustomTimeout),
    delayed_delete_(false)
{}

bool operator==(const RequestInfo& left, const RequestInfo& right) {
  return left.connectionID_ == right.connectionID_ &&
         left.correlationID_ == right.correlationID_ ;
}

bool operator<(const RequestInfo& left, const RequestInfo& right) {
  if (left.connectionID_ == right.connectionID_) {
    return left.correlationID_ < right.correlationID_;
  } else {
    return left.connectionID_ < right.connectionID_;
  }
}

}  // namespace request_watchdog
