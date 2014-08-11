/*
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_

#include <aoa/aoa.h>

namespace transport_manager {
namespace transport_adapter {

enum AOAVersion { AOA_Unknown, AOA_1_0, AOA_2_0 };

class AOAWrapper {
 public:
  typedef aoa_hdl_t* AOAHandle;

  static bool Init() const;
  static bool Shutdown() const;
  static inline bool IsError(int ret) const;
  static inline void PrintError(int ret) const;
  static bool IsValidHandle(AOAHandle hdl) const;
  static AOAVersion GetProtocol(AOAHandle hdl) const;

  AOAWrapper(AOAHandle hdl);

  bool IsValidHandle() const;
  AOAVersion GetProtocol() const;
  uint32_t GetMaximumSizeBuffer() const;
  bool Subscribe() const;
  bool SendToControlEndpoint(uint16_t request, uint16_t value, uint16_t index,
                             RawMessagePtr message) const;
  bool SendToBulkEndpoint(RawMessagePtr message) const;

 private:
  AOAHandle hdl_;
  uint32_t endpoint_;
  uint32_t timeout_;

  static AOAVersion Version(uint16_t version);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_AOA_WRAPPER_H_
