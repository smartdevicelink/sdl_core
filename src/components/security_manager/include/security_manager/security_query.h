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

#ifndef SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_QUERY_H_
#define SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_QUERY_H_

#include <stdint.h>
#include <cstddef>
#include <vector>
#include <string>
#include "utils/shared_ptr.h"

namespace security_manager {
/**
 * \brief SecurityQuery is wrapper for handling Mobile messages
 * as security queries
 */
class SecurityQuery {
 public:
  /**
   * \brief QueryType is 1 byte type of income query
   * Equal RPC Type (Ford Binary Header Definition)
   */
  enum QueryType {
    REQUEST      = 0x00,
    RESPONSE     = 0x10,
    NOTIFICATION = 0x20,
    INVALID_QUERY_TYPE = 0xFF
    };
  /**
   * \brief QueryId is 3 byte identifier of income query
   * Equal RPC Function ID (Ford Binary Header Definition)
   */
  enum QueryId {
    SEND_HANDSHAKE_DATA      = 0x1,
    SEND_INTERNAL_ERROR      = 0x2,
    INVALID_QUERY_ID         = 0xFFFFFF
  };
  /**
   * \brief QueryHeader is 12 byte header of security query
   * Equal Ford Binary Header Definition
   */
  struct QueryHeader {
    QueryHeader();
    QueryHeader(uint8_t queryType, uint32_t queryId,
                uint32_t seqNumber = 0, uint32_t jsonSize= 0);
    // TODO(EZamakhov): check bitfield correctness on other endianness platform
    uint32_t query_type:8;
    uint32_t query_id:24;  // API function identifier
    uint32_t seq_number;   // request sequential number
    uint32_t json_size;
  };

  /**
   * \brief Constructor
   */
  SecurityQuery();
  /**
   * \brief Constructor with header and connection_key
   * \param connection_key Unique key used by other components as session identifier
   * \param header QueryHeader
   */
  SecurityQuery(const QueryHeader &header, const uint32_t connection_key);
  /**
   * \brief Constructor with header, connection_key and query binary data
   * \param connection_key Unique key used by other components as session identifier
   * \param raw_data pointer to binary data array
   * \param raw_data_size size of binary data array
   * \param header QueryHeader
   */
  SecurityQuery(const QueryHeader &header, const uint32_t connection_key,
                const uint8_t *const raw_data, const size_t raw_data_size);
  /**
   * \brief Serialize income from Mobile Application data
   * as query with header and binary data or json message
   * \param raw_data pointer to binary data array
   * \param raw_data_size size of binary data array
   * \return \c true on correct parse and \c false on wrong size of data
   */
  bool SerializeQuery(const uint8_t *const raw_data, const size_t raw_data_size);
  /**
   * \brief Deserialize query for sending to Mobile Application
   * \return \c vector of uint8_t data (serialized header data and send_data))
   */
  const std::vector<uint8_t> DeserializeQuery() const;
  /**
   * \brief Set binary data. (No header modification)
   * \param binary_data pointer to binary data array
   * \param bin_data_size size of binary data array
   */
  void set_data(const uint8_t *const binary_data, const size_t bin_data_size);
  /**
   * \brief Set json data. (No header modification)
   * \param json_message string with json error
   */
  void set_json_message(const std::string &json_message);
  /**
   * \brief Set connection key
   * \param connection_key Unique key used by other components as session identifier
   */
  void set_connection_key(const uint32_t connection_key);
  /**
   * \brief Set query header
   * \param header of query
   */
  void set_header(const QueryHeader &header);
  /**
   * \brief Get query header
   * \return header of query
   */
  const QueryHeader &get_header() const;
  /**
   * \brief Get query binary data (without header data)
   * \return const pointer to const binary data
   */
  const uint8_t *get_data() const;
  /**
   * \brief Get query binary data size
   * \return size of binary data
   */
  size_t get_data_size() const;
  /**
   * \brief Get json string data (without header data)
   * \return const pointer to const binary data
   */
  const std::string &get_json_message() const;
  /**
   * \brief Get connection key
   * \return Unique key used by other components as session identifier
   */
  uint32_t get_connection_key() const;

 private:
  /**
   *\brief 12 byte header of security query
   * Equal Ford Binary Header Definition
   */
  QueryHeader header_;
  /**
   *\brief nique key used by other components as session identifier
   */
  uint32_t connection_key_;
  /**
   *\brief Binary data of query (without header info)
   */
  std::vector<uint8_t> data_;
  /**
   *\brief JSON (string) value of query
   */
  std::string json_message_;
};
/**
*\brief SmartPointer wrapper
*/
typedef utils::SharedPtr<SecurityQuery> SecurityQueryPtr;
}  // namespace security_manager
#endif  // SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_QUERY_H_
