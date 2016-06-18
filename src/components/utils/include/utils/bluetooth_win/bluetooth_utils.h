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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_UTILS_WIN_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_UTILS_WIN_H_
#include "utils/winhdr.h"
#include "utils/macro.h"

#include <vector>
#include <string>
#include <cstdint>
#include <initguid.h>
#include <ws2bth.h>
#include <BluetoothAPIs.h>

namespace utils {
class BluetoothUUID;
const TCHAR* const kCommonUuidPostfix = "-0000-1000-8000-00805f9b34fb";
const TCHAR* const kCommonUuidPrefix = "0000";
const uint16_t kProtocolDescriptorListId = 4;
const uint16_t kRfcommUuid = 3;
const uint16_t kUuidId = 1;

enum UUIDFormat { kFormatInvalid, kFormat16Bit, kFormat32Bit, kFormat128Bit };

template <int BASE, typename T>
bool CharToDigit(T c, uint8_t& digit) {
  if (c >= '0' && c <= '9') {
    digit = c - '0';
  } else if (c >= 'a' && c < 'a' + BASE - 10) {
    digit = c - 'a' + 10;
  } else if (c >= 'A' && c < 'A' + BASE - 10) {
    digit = c - 'A' + 10;
  } else {
    return false;
  }
  return true;
}

template <typename T>
T ToLowerASCII(T c) {
  return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

template <typename T>
T ToUpperASCII(T c) {
  return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
}

template <typename T>
bool IsHexDigit(T c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
         (c >= 'a' && c <= 'f');
}

bool HexStringToBytes(const std::string& input, std::vector<uint8_t>& output);

std::string GetLastErrorMessage(const DWORD last_error);

void ConvertBytesToUUID(const BYTE* bytes, GUID& uuid);

/**
 * @brief Returns the canonical, 128-bit canonical, and the format of the UUID
 *        in |canonical|, |canonical_128|, and |format| based on |uuid|.
 */
void GetCanonicalUuid(std::string uuid,
                      std::string& canonical,
                      std::string& canonical_128,
                      utils::UUIDFormat& format);

bool AdvanceToSdpType(const SDP_ELEMENT_DATA& sequence_data,
                      SDP_TYPE type,
                      HBLUETOOTH_CONTAINER_ELEMENT& element,
                      SDP_ELEMENT_DATA& sdp_data);

void ExtractChannels(const SDP_ELEMENT_DATA& protocol_descriptor_list_data,
                     bool& supports_rfcomm,
                     uint8_t& rfcomm_channel);

void ExtractUuid(const SDP_ELEMENT_DATA& uuid_data, BluetoothUUID& uuid);

BTH_ADDR StringToBthAddr(const std::string& address);

SOCKADDR_BTH StringToSockBthAddr(const std::string& address);

std::string BthAddrTostring(BTH_ADDR& address);

std::string BthDeviceAddrToStr(const BLUETOOTH_DEVICE_INFO_STRUCT& bt_dev_info);

std::string GuidToStr(const GUID& guid);

std::string GetDeviceAddrStr(LPCSADDR_INFO& addr_info,
                             WSAPROTOCOL_INFO& protocolInfo);

std::vector<uint8_t> ByteArrayToVector(const BLOB& p_blob);
}  // namespace utils
#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BLUETOOTH_UTILS_WIN_H_
