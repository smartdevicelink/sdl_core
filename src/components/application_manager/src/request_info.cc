/**
* \file request_info.h
* \brief request information structure source file.
*
* Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/request_info.h"
namespace application_manager {

namespace request_controller {

HMIRequestInfo::HMIRequestInfo(
    RequestPtr request,
    const uint64_t timeout_sec):
  RequestInfo(timeout_sec),
  request_(request) {
    correlation_id_ = request_->correlation_id();
}

HMIRequestInfo::HMIRequestInfo(
    RequestPtr request,
    const TimevalStruct &start_time,
    const uint64_t timeout_sec):
  RequestInfo(start_time, timeout_sec),
  request_(request) {
    correlation_id_ = request_->correlation_id();
}

MobileRequestInfo::MobileRequestInfo(
    RequestPtr request,
    const uint64_t timeout_sec):
  RequestInfo(timeout_sec),
  request_(request) {
    mobile_correlation_id_ = request_.get()->correlation_id();
    app_id_ = request_.get()->connection_key();
}

MobileRequestInfo::MobileRequestInfo(
    RequestPtr request,
    const TimevalStruct &start_time,
    const uint64_t timeout_sec):
  RequestInfo(start_time, timeout_sec),
  request_(request) {
    mobile_correlation_id_ = request_.get()->correlation_id();
    app_id_ = request_.get()->connection_key();
}

} // namespace request_controller

} // namespace application_manager
