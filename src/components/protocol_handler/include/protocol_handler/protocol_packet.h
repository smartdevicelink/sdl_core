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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_

#include <list>
#include "utils/macro.h"
#include "protocol/common.h"
#include "transport_manager/common.h"

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {

typedef transport_manager::ConnectionUID ConnectionID;
/**
 * \class ProtocolPacket
 * \brief Class for forming/parsing protocol headers of the message and
 * handling multiple frames of the message.
 */
class ProtocolPacket {
 public:
  /**
   * \struct ProtocolData
   * \brief Used for storing message and its size.
   */
  struct ProtocolData {
    ProtocolData();
    ~ProtocolData();
    uint8_t *data;
    uint32_t totalDataBytes;
  };

  /**
   * \class ProtocolHeader
   * \brief Used for storing protocol header of a message.
   */
  class ProtocolHeader {
   public:
    ProtocolHeader();
    ProtocolHeader(uint8_t version, bool protection,
                   uint8_t frameType,
                   uint8_t serviceType,
                   uint8_t frameData, uint8_t sessionID,
                   uint32_t dataSize, uint32_t messageID);
    uint8_t version;
    bool protection_flag;
    uint8_t frameType;
    uint8_t serviceType;
    uint8_t frameData;
    uint8_t sessionId;
    uint32_t dataSize;
    uint32_t messageId;
    void deserialize(const uint8_t *message, const size_t messageSize);
  };
  /**
   * \class ProtocolHeaderValidator
   * \brief Used for ProtocolHeader validation
   */
  class ProtocolHeaderValidator {
   public:
    ProtocolHeaderValidator();
    /**
     * \brief Setter/getter maximum payload size of packets
     */
    void set_max_payload_size(const size_t max_payload_size);
    size_t max_payload_size() const;
    /**
     * \brief Check ProtocolHeader according to protocol requiements
     */
    RESULT_CODE validate(const ProtocolHeader& header) const;
   private:
    size_t max_payload_size_;
  };

  /**
   * \brief Default constructor
   */
  ProtocolPacket();

  /**
   * \brief Constructor
   *
   * \param connection_id - Connection Identifier
   */
  explicit ProtocolPacket(ConnectionID connection_id);

  /**
   * \brief Constructor
   * \param connection_id - Connection Identifier
   * \param version Version of protocol
   * \param protection Protection flag
   * \param frameType Type of frame (Single/First/Consecutive)
   * \param serviceType Type of session (RPC/Bulk data)
   * \param frameData Information about frame: start/end session, number of
   * frame, etc
   * \param sessionID Number of frame within connection
   * \param dataSize Size of message string
   * \param messageID ID of message or hash code - only for second protocol
   * \param data Message string if provided
   */
  ProtocolPacket(ConnectionID connection_id,
                 uint8_t version, bool protection, uint8_t frameType,
                 uint8_t serviceType, uint8_t frameData,
                 uint8_t sessionId, uint32_t dataSize,
                 uint32_t messageID, const uint8_t *data = 0);

  /*Serialization*/
  /**
   * \brief Serializes info about message into protocol header.
   * \return RawMessagePtr with all data (header and message)
   */
  RawMessagePtr serializePacket() const;
  /**
   * \brief Appends message frame to existing message in
   * recieving multiframe messages.
   * \param chunkData Current frame's message string
   * \param chunkDataSize Size of current message string
   * \return \saRESULT_CODE Status of serialization
   */
  RESULT_CODE appendData(uint8_t *chunkData, uint32_t chunkDataSize);

  /**
   * \brief Getter of message size including protocol header
   * \return uint32_t size of message string
   */
  size_t packet_size() const;

  /*End of Serialization*/

  bool operator==(const protocol_handler::ProtocolPacket& other) const;

  /*Deserialization*/

  /**
   * \brief Parses protocol header
   * \param message Incoming message string containing both header and
   * message body
   * \param messageSize Incoming message size
   * \return \saRESULT_CODE Status of serialization
   */
  RESULT_CODE deserializePacket(const uint8_t *message,
                                const size_t messageSize);

  /**
   * \brief Getter of protocol version.
   */
  uint8_t protocol_version() const;

  /**
   * \brief Getter of protection flag
   */
  bool protection_flag() const;

  /**
   * \brief Setter of protection flag
   */
  void set_protection_flag(const bool protection);

  /**
   * \brief Getter of frame type (single/first/etc)
   */
  uint8_t frame_type() const;

  /**
   *\brief Getter of service type (RPC/Bulk data)
   */
  uint8_t service_type() const;

  /**
   *\brief Getter and setter of frame data (start/end session, number of frame etc)
   */
  uint8_t frame_data() const;

  void set_frame_data(const uint8_t frame_data);

  /**
   *\brief Getter of session number
   */
  uint8_t session_id() const;

  /**
   *\brief Getter of size of message body
   */
  uint32_t data_size() const;

  /**
   *\brief Getter of message id for second version of protocol
   */
  uint32_t message_id() const;

  /**
   *\brief Getter of message string
   */
  uint8_t *data() const;

  /**
   *\brief Setter for size of multiframe message
   */
  void set_total_data_bytes(size_t dataBytes);

  /**
   *\brief Setter for new data
   */
  void set_data(const uint8_t *const  new_data,
                const size_t new_data_size);

  /**
   *\brief Getter for size of multiframe message
   */
  uint32_t total_data_bytes() const;
  /*End of Deserialization*/

  /**
    * \brief Getter for Connection Identifier
    */
  ConnectionID connection_id() const;

  /**
    * \brief Getter for data payload size
    */
  uint32_t payload_size() const;

  /**
    * \brief Getter for full header information
    */
  const ProtocolHeader& packet_header() const;

 private:
  /**
   *\brief Protocol header
   */
  ProtocolHeader packet_header_;

  /**
   *\brief Message body (without header)
   */
  ProtocolData packet_data_;

  /**
   *\brief Actual size of received data
   */
  uint32_t payload_size_;

  /**
    * \brief Connection Identifier
    * Obtained from connection_handler
    */
  ConnectionID connection_id_;

  DISALLOW_COPY_AND_ASSIGN(ProtocolPacket);
};
}  // namespace protocol_handler
/**
 * @brief Type definition for variable that hold shared pointer to protocolol packet
 */
typedef utils::SharedPtr<protocol_handler::ProtocolPacket> ProtocolFramePtr;
typedef std::list<ProtocolFramePtr>                        ProtocolFramePtrList;

template<typename _CharT>
std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT>& stream,
                                       const protocol_handler::ProtocolPacket::ProtocolHeader& header) {
  stream << "Version: "       << static_cast<uint32_t>(header.version) <<
            ", Protection: "  << (header.protection_flag ? "ON" : "OFF") <<
            ", FrameType: "   << static_cast<uint32_t>(header.frameType) <<
            ", ServiceType: " << static_cast<uint32_t>(header.serviceType) <<
            ", FrameData: "   << static_cast<uint32_t>(header.frameData) <<
            ", SessionId: "   << static_cast<uint32_t>(header.sessionId) <<
            ", DataSize: "    << static_cast<uint32_t>(header.dataSize) <<
            ", MessageId: "   << static_cast<uint32_t>(header.messageId);
  return stream;
}
template<typename _CharT>
std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT>& stream,
                                       const protocol_handler::ProtocolPacket& packet) {
  stream << packet.packet_header() <<
            ", ConnectionID: "   << static_cast<uint32_t>(packet.connection_id()) <<
            ", TotalDataBytes: " << (packet.total_data_bytes()) <<
            ", Data: "           << static_cast<void*>(packet.data());
  return stream;
}
template<typename _CharT>
std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT>& stream,
                                       const ProtocolFramePtr packet_ptr) {
  if(packet_ptr) {
    return stream << *packet_ptr;
  }
  return stream << "empty smart pointer";
}
#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_
