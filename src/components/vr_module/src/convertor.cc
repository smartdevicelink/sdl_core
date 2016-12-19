/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "vr_module/convertor.h"

namespace vr_module {

vr_hmi_api::ResultCode Convertor(vr_mobile_api::ResultCode value) {
  switch (value) {
    case vr_mobile_api::SUCCESS:
      return vr_hmi_api::SUCCESS;
    case vr_mobile_api::INVALID_DATA:
      return vr_hmi_api::INVALID_DATA;
    case vr_mobile_api::GENERIC_ERROR:
      return vr_hmi_api::GENERIC_ERROR;
    case vr_mobile_api::REJECTED:
      return vr_hmi_api::REJECTED;
    case vr_mobile_api::WARNINNG:
      return vr_hmi_api::WARNINGS;
    case vr_mobile_api::TIMEOUT:
      return vr_hmi_api::TIMEOUT;
    default:
      return vr_hmi_api::GENERIC_ERROR;
  }
}

vr_mobile_api::ResultCode Convertor(vr_hmi_api::ResultCode value) {
  switch (value) {
    case vr_hmi_api::SUCCESS:
      return vr_mobile_api::SUCCESS;
    case vr_hmi_api::INVALID_DATA:
      return vr_mobile_api::INVALID_DATA;
    case vr_hmi_api::GENERIC_ERROR:
      return vr_mobile_api::GENERIC_ERROR;
    case vr_hmi_api::WARNINGS:
      return vr_mobile_api::WARNINNG;
    case vr_hmi_api::REJECTED:
      return vr_mobile_api::REJECTED;
    case vr_hmi_api::TIMEOUT:
      return vr_mobile_api::TIMEOUT;
    case vr_hmi_api::UNSUPPORTED_RESOURCE:
    default:
      return vr_mobile_api::GENERIC_ERROR;
  }
}

}  // namespace vr_module
