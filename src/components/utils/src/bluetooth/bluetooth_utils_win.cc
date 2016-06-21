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

#include <sstream>
#include "utils/bluetooth/bluetooth_utils_win.h"
#include "utils/bluetooth/bluetooth_uuid_win.h"

std::string utils::GetLastErrorMessage(const DWORD last_error) {
  TCHAR errmsg[512];
  if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                     0,
                     last_error,
                     0,
                     errmsg,
                     sizeof(errmsg) - 1,
                     NULL)) {
    return std::string(utils::GetLastErrorMessage(GetLastError()));
  }
  return std::string(errmsg);
}

void utils::ConvertBytesToUUID(const BYTE* bytes, GUID& uuid) {
  DCHECK(bytes);
  uuid.Data1 = bytes[0] << 24 & 0xff000000 | bytes[1] << 16 & 0x00ff0000 |
               bytes[2] << 8 & 0x0000ff00 | bytes[3] & 0x000000ff;
  uuid.Data2 = bytes[4] << 8 & 0xff00 | bytes[5] & 0x00ff;
  uuid.Data3 = bytes[6] << 8 & 0xff00 | bytes[7] & 0x00ff;

  for (int i = 0; i < 8; i++) {
    uuid.Data4[i] = bytes[i + 8];
  }
}

bool utils::HexStringToBytes(const std::string& input,
                             std::vector<uint8_t>& output) {
  const size_t count = input.size();
  if (count == 0u || (count % 2) != 0) {
    return false;
  }
  for (uintptr_t i = 0u; i < count / 2; ++i) {
    uint8_t msb = 0u;  // most significant 4 bits
    uint8_t lsb = 0u;  // least significant 4 bits
    if (!CharToDigit<16>(input[i * 2], msb) ||
        !CharToDigit<16>(input[i * 2 + 1], lsb)) {
      return false;
    }
    output.push_back((msb << 4) | lsb);
  }
  return true;
}

void utils::GetCanonicalUuid(std::string uuid,
                             std::string& canonical,
                             std::string& canonical_128,
                             utils::UUIDFormat& format) {
  canonical.clear();
  canonical_128.clear();
  format = utils::UUIDFormat::kFormatInvalid;

  if (uuid.empty()) {
    return;
  }

  if (uuid.size() < 11u && uuid.find("0x") == 0u) {
    uuid = uuid.substr(2);
  }
  if (!(uuid.size() == 4u || uuid.size() == 8u || uuid.size() == 36u)) {
    return;
  }
  for (size_t i = 0u; i < uuid.size(); ++i) {
    if (i == 8u || i == 13u || i == 18u || i == 23u) {
      if (uuid[i] != '-') {
        return;
      }
    } else {
      if (!IsHexDigit(uuid[i])) {
        return;
      }
      uuid[i] = utils::ToLowerASCII(uuid[i]);
    }
  }
  canonical.assign(uuid);
  if (uuid.size() == 4u) {
    canonical_128.assign(kCommonUuidPrefix + uuid + kCommonUuidPostfix);
    format = utils::UUIDFormat::kFormat16Bit;
  } else if (uuid.size() == 8u) {
    canonical_128.assign(uuid + kCommonUuidPostfix);
    format = utils::UUIDFormat::kFormat32Bit;
  } else {
    canonical_128.assign(uuid);
    format = utils::UUIDFormat::kFormat128Bit;
  }
}

bool utils::AdvanceToSdpType(const SDP_ELEMENT_DATA& sequence_data,
                             SDP_TYPE type,
                             HBLUETOOTH_CONTAINER_ELEMENT& element,
                             SDP_ELEMENT_DATA& sdp_data) {
  while (ERROR_SUCCESS ==
         BluetoothSdpGetContainerElementData(sequence_data.data.sequence.value,
                                             sequence_data.data.sequence.length,
                                             &element,
                                             &sdp_data)) {
    if (sdp_data.type == type) {
      return true;
    }
  }
  return false;
}

void utils::ExtractChannels(
    const SDP_ELEMENT_DATA& protocol_descriptor_list_data,
    bool& supports_rfcomm,
    uint8_t& rfcomm_channel) {
  HBLUETOOTH_CONTAINER_ELEMENT sequence_element = NULL;
  SDP_ELEMENT_DATA sequence_data;
  while (utils::AdvanceToSdpType(protocol_descriptor_list_data,
                                 SDP_TYPE_SEQUENCE,
                                 sequence_element,
                                 sequence_data)) {
    HBLUETOOTH_CONTAINER_ELEMENT inner_sequence_element = NULL;
    SDP_ELEMENT_DATA inner_sequence_data;
    if (utils::AdvanceToSdpType(sequence_data,
                                SDP_TYPE_UUID,
                                inner_sequence_element,
                                inner_sequence_data) &&
        inner_sequence_data.data.uuid32 == utils::kRfcommUuid &&
        utils::AdvanceToSdpType(sequence_data,
                                SDP_TYPE_UINT,
                                inner_sequence_element,
                                inner_sequence_data) &&
        inner_sequence_data.specificType == SDP_ST_UINT8) {
      rfcomm_channel = inner_sequence_data.data.uint8;
      supports_rfcomm = true;
    }
  }
}

void utils::ExtractUuid(const SDP_ELEMENT_DATA& uuid_data,
                        utils::BluetoothUUID& uuid) {
  HBLUETOOTH_CONTAINER_ELEMENT inner_uuid_element = NULL;
  SDP_ELEMENT_DATA inner_uuid_data;
  if (utils::AdvanceToSdpType(
          uuid_data, SDP_TYPE_UUID, inner_uuid_element, inner_uuid_data)) {
    if (inner_uuid_data.specificType == SDP_ST_UUID16) {
      TCHAR temp_uuid_str[sizeof(inner_uuid_data)];
      sprintf_s(temp_uuid_str,
                sizeof(temp_uuid_str),
                "%04x",
                inner_uuid_data.data.uuid16);
      uuid = utils::BluetoothUUID(std::string(temp_uuid_str));
    } else if (inner_uuid_data.specificType == SDP_ST_UUID32) {
      TCHAR temp_uuid_str[sizeof(inner_uuid_data)];
      sprintf_s(temp_uuid_str,
                sizeof(temp_uuid_str),
                "%08x",
                inner_uuid_data.data.uuid32);
      uuid = utils::BluetoothUUID(std::string(temp_uuid_str));
    } else if (inner_uuid_data.specificType == SDP_ST_UUID128) {
      TCHAR temp_uuid_str[sizeof(inner_uuid_data) +
                          sizeof(inner_uuid_data.data.uuid128)];
      sprintf_s(temp_uuid_str,
                sizeof(temp_uuid_str),
                "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                inner_uuid_data.data.uuid128.Data1,
                inner_uuid_data.data.uuid128.Data2,
                inner_uuid_data.data.uuid128.Data3,
                inner_uuid_data.data.uuid128.Data4[0],
                inner_uuid_data.data.uuid128.Data4[1],
                inner_uuid_data.data.uuid128.Data4[2],
                inner_uuid_data.data.uuid128.Data4[3],
                inner_uuid_data.data.uuid128.Data4[4],
                inner_uuid_data.data.uuid128.Data4[5],
                inner_uuid_data.data.uuid128.Data4[6],
                inner_uuid_data.data.uuid128.Data4[7]);
      uuid = utils::BluetoothUUID(std::string(temp_uuid_str));
    } else {
      uuid = utils::BluetoothUUID();
    }
  }
}

BTH_ADDR utils::StringToBthAddr(const std::string& address) {
  BTH_ADDR bth_addr = 0;
  std::string numbers_only;
  for (int i = 0; i < 6; ++i) {
    numbers_only += address.substr(i * 3, 2);
  }

  std::vector<uint8_t> address_bytes;
  HexStringToBytes(numbers_only, address_bytes);
  int byte_position = 0;
  for (std::vector<uint8_t>::reverse_iterator iter = address_bytes.rbegin();
       iter != address_bytes.rend();
       ++iter) {
    bth_addr += static_cast<BTH_ADDR>(*iter * pow(256.0, byte_position));
    byte_position++;
  }
  return bth_addr;
}

SOCKADDR_BTH utils::StringToSockBthAddr(const std::string& address) {
  SOCKADDR_BTH socket_address;
  std::stringstream address_record;
  address_record << address.c_str();
  int address_size = sizeof(struct sockaddr_storage);
  std::string addr_str_tmp = address_record.str();
  LPSTR addr_str = static_cast<LPSTR>(&addr_str_tmp[0]);
  WSAStringToAddress(addr_str,
                     AF_BTH,
                     NULL,
                     reinterpret_cast<LPSOCKADDR>(&socket_address),
                     &address_size);
  return socket_address;
}

std::string utils::BthAddrTostring(BTH_ADDR& address) {
  TCHAR* AddrString = 0;
  DWORD dwSizeOfStr = sizeof(AddrString);
  if (WSAAddressToStringA(reinterpret_cast<LPSOCKADDR>(&address),
                          sizeof(address),
                          NULL,
                          AddrString,
                          &dwSizeOfStr)) {
    return std::string();
  }
  return std::string(AddrString);
}

std::string utils::BthDeviceAddrToStr(
    const BLUETOOTH_DEVICE_INFO_STRUCT& bt_dev_info) {
  TCHAR address[sizeof(bt_dev_info.Address.rgBytes) * 6];
  sprintf_s(address,
            sizeof(bt_dev_info.Address.rgBytes) * 6,
            "(%02x:%02x:%02x:%02x:%02x:%02x)",
            bt_dev_info.Address.rgBytes[5],
            bt_dev_info.Address.rgBytes[4],
            bt_dev_info.Address.rgBytes[3],
            bt_dev_info.Address.rgBytes[2],
            bt_dev_info.Address.rgBytes[1],
            bt_dev_info.Address.rgBytes[0]);
  std::string result(address);
  for (std::string::iterator it = result.begin(); it != result.end(); ++it) {
    *it = static_cast<char>(toupper(*it));
  }
  return result;
}

std::string utils::GuidToStr(const GUID& guid) {
  TCHAR data[sizeof(guid) * 4];
  sprintf_s(
      data,
      sizeof(guid) * 4,
      "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
      utils::ToLowerASCII(guid.Data1),
      utils::ToLowerASCII(guid.Data2),
      utils::ToLowerASCII(guid.Data3),
      utils::ToLowerASCII(guid.Data4[0]),
      utils::ToLowerASCII(guid.Data4[1]),
      utils::ToLowerASCII(guid.Data4[2]),
      utils::ToLowerASCII(guid.Data4[3]),
      utils::ToLowerASCII(guid.Data4[4]),
      utils::ToLowerASCII(guid.Data4[5]),
      utils::ToLowerASCII(guid.Data4[6]),
      utils::ToLowerASCII(guid.Data4[7]));
  return std::string(data);
}

std::string utils::GetDeviceAddrStr(LPCSADDR_INFO& addr_info,
                                    WSAPROTOCOL_INFO& protocolInfo) {
  CSADDR_INFO* pCSAddr = static_cast<CSADDR_INFO*>(addr_info);
  TCHAR addressAsString[1000];
  DWORD addressSize = sizeof(addressAsString);
  addressSize = sizeof(addressAsString);
  WSAAddressToString(pCSAddr->RemoteAddr.lpSockaddr,
                     pCSAddr->RemoteAddr.iSockaddrLength,
                     &protocolInfo,
                     addressAsString,
                     &addressSize);
  return std::string(addressAsString);
}

std::vector<uint8_t> utils::ByteArrayToVector(const BLOB& p_blob) {
  std::vector<uint8_t> sdp_data;
  for (uint64_t i = 0; i < p_blob.cbSize; i++) {
    sdp_data.push_back(p_blob.pBlobData[i]);
  }
  return sdp_data;
}
