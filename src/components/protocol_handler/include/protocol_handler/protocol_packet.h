/**
 * \file ProtocolPacket.h
 * \brief ProtocolPacket class header file.
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_

#include "utils/macro.h"

/**
 *\namespace NsProtocolHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
/**
 *\brief Size of protocol header for first version of protocol.
 */
const uint8_t PROTOCOL_HEADER_V1_SIZE = 8;

/**
 *\brief Size of protocol header for second version of protocol.
 */
const uint8_t PROTOCOL_HEADER_V2_SIZE = 12;

/**
 *\brief Constant: number of protocol version (1).
 */
const uint8_t PROTOCOL_VERSION_1 = 0x01;

/**
 *\brief Constant: number of protocol version (2).
 */
const uint8_t PROTOCOL_VERSION_2 = 0x02;

/**
 *\brief Constant: flag of protection
 */
const bool PROTECTION_ON = true;
/**
 *\brief Constant: flag of no protection
 */
const bool PROTECTION_OFF = false;

/**
 *\brief Constant: Control type of frame used in protocol header.
 */
const uint8_t FRAME_TYPE_CONTROL = 0x00;

/**
 *\brief Constant: Single type of frame used in protocol header.
 */
const uint8_t FRAME_TYPE_SINGLE = 0x01;

/**
 *\brief Constant: First frame for multiple frames used in protocol header.
 */
const uint8_t FRAME_TYPE_FIRST = 0x02;

/**
 *\brief Constant: Consecutive type of frame for multiple frames used in
 *\brief protocol header.
 */
const uint8_t FRAME_TYPE_CONSECUTIVE = 0x03;


/**
 *\brief Constant: Frame type for HeartBeat
 */
const uint8_t SERVICE_TYPE_CONTROL = 0x00;

/**
 *\brief Constant: RPC type of session
 */
const uint8_t SERVICE_TYPE_RPC = 0x07;

/**
 *\brief Constant: Raw PCM audio service
 */
const uint8_t SERVICE_TYPE_AUDIO = 0x0A;

/**
 * \brief Constant: Mobile Navi type of session for map streaming
 */
const uint8_t SERVICE_TYPE_NAVI = 0x0B;

/**
 *\brief Constant: Bulk data type of session (hybrid)
 */
const uint8_t SERVICE_TYPE_BULK = 0x0F;

/**
 *\brief Unused: If FRAME_TYPE_CONTROL: Constant: Frame is for heart beat.
 */
const uint8_t FRAME_DATA_HEART_BEAT = 0x00;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Start service frame
 */
const uint8_t FRAME_DATA_START_SERVICE = 0x01;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Start service acknowledgement frame
 */
const uint8_t FRAME_DATA_START_SERVICE_ACK = 0x02;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Start service not acknowledgement
 *\brief frame
 */
const uint8_t FRAME_DATA_START_SERVICE_NACK = 0x03;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: End service request frame
 */
const uint8_t FRAME_DATA_END_SERVICE = 0x04;

/**
 * \brief if FRAME_TYPE_CONTROL: Constant: End service acknowledgement frame
 */
const uint8_t FRAME_DATA_END_SERVICE_ACK = 0x05;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: End service not acknowledgement frame
 */
const uint8_t FRAME_DATA_END_SERVICE_NACK = 0x06;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Service data ACK frame
 */
const uint8_t FRAME_DATA_SERVICE_DATA_ACK = 0xFE;

/**
 *\brief Unused: If FRAME_TYPE_CONTROL: Constant: Frame is for heart beat ack.
 */
const uint8_t FRAME_DATA_HEART_BEAT_ACK = 0xFF;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Maximum of consecutive frame numbers
 * after which count has to roll over to 0x01
 */
const uint8_t FRAME_DATA_MAX_VALUE = 0xFF;

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Maximum size of one frame excluding
 *\brief frame header.
 */
const uint32_t MAXIMUM_FRAME_DATA_SIZE = 1488;

/**
 *\brief If FRAME_TYPE_CONSECUTIVE: Constant: Marks last frame in mutliframe
 *\brief message.
 */
const uint8_t FRAME_DATA_LAST_FRAME = 0x00;

/**
 *\brief If FRAME_TYPE_CONSECUTIVE: Constant: Size of first frame in
 *\brief mutliframe message.
 */
const uint8_t FIRST_FRAME_DATA_SIZE = 0x08;

/**
 *\enum RESULT_CODE
 *\brief Return type for operations with message handling.
 */
enum RESULT_CODE {
  RESULT_OK = 1,
  RESULT_FAIL = 2,
  RESULT_NUMBER_BUSY = 3,
  RESULT_CONNECTION_LOST = 4,
  RESULT_DISCONNECTED = 5,
  RESULT_NOTFOUND = 6,
  RESULT_TIMEOUT = 7,
  RESULT_WRONG_SEQUENCE = 8,
  RESULT_AGAIN = 9,
  RESULT_WRONG_FRAME_SEQUENCE = 10,
  RESULT_IN_PROGRESS = 11,
  RESULT_REASSIGN = 12,
  RESULT_XML_PARSING = 13,
  RESULT_RESEND_ACK = 14,
  RESULT_DEFRERRED = 15,
  RESULT_ENCRYPTION_FAILED = 16,
  RESULT_UNKNOWN = 255
};

/**
 * \struct ProtocolHeader
 * \brief Used for storing protocol header of a message.
 */
// TODO(AK): Can we move this to cc file?
struct ProtocolHeader {
  /**
   * \brief Constructor
   */
  ProtocolHeader()
    : version(0x00),
      protection_flag(0x00),
      frameType(0x00),
      serviceType(0x00),
      frameData(0x00),
      sessionId(0x00),
      dataSize(0x00),
      messageId(0x00) {
  }

  /**
   *\brief Version of protocol
   */
  uint8_t version;

  /**
   *\brief protection flag
   */
  bool protection_flag;

  /**
   *\brief Type of frame (Single/First/Consecutive)
   */
  uint8_t frameType;

  /**
   *\brief Type of service (RPC/Buld data)
   */
  uint8_t serviceType;

  /**
   *\brief Data about frame (Start/End session etc)
   */
  uint8_t frameData;

  /**
   *\brief Session number withing connection
   */
  uint8_t sessionId;

  /**
   *\brief Size of message itself, excluding size of header
   */
  uint32_t dataSize;

  /**
   *\brief Used only in second versin of protocol: Message ID
   */
  uint32_t messageId;
};

/**
 * \struct ProtocolData
 * \brief Used for storing message and its size.
 */
struct ProtocolData {
  ProtocolData()
    : data(0),
      totalDataBytes(0x00) {
  }

  uint8_t* data;
  uint32_t totalDataBytes;
};

/**
 * \class ProtocolPacket
 * \brief Class for forming/parsing protocol headers of the message and
 * handling multiple frames of the message.
 */
class ProtocolPacket {
  public:
    /**
     * \brief Default constructor
     */
    ProtocolPacket();

    /**
     * \brief Constructor
     *
     * \param connectionKey Identifier of connection within wich message
     * is transferred
     * \param data Message string
     * \param dataSize Message size
     */
    ProtocolPacket(uint8_t connection_key, uint8_t* data_param,
                   uint32_t data_size);

    /**
     * \brief Constructor
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
    ProtocolPacket(uint8_t connection_key,
                   uint8_t version, bool protection, uint8_t frameType,
                   uint8_t serviceType, uint8_t frameData,
                   uint8_t sessionId, uint32_t dataSize,
                   uint32_t messageID, const uint8_t* data = 0,
                   uint32_t packet_id = 0);
    /**
     * \brief Destructor
     */
    ~ProtocolPacket();

    /*Serialization*/
    /**
     * \brief Serializes info about message into protocol header.
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
     * \return \saRESULT_CODE Status of serialization
     */
    RESULT_CODE serializePacket(uint8_t version, bool protection,
                                uint8_t frameType,
                                uint8_t serviceType,
                                uint8_t frameData, uint8_t sessionId,
                                uint32_t dataSize, uint32_t messageID,
                                const uint8_t* data = 0);

    /**
     * \brief Appends message frame to existing message in
     * recieving multiframe messages.
     * \param chunkData Current frame's message string
     * \param chunkDataSize Size of current message string
     * \return \saRESULT_CODE Status of serialization
     */
    RESULT_CODE appendData(uint8_t* chunkData, uint32_t chunkDataSize);

    /**
     * \brief Getter of serialized message with protocol header
     * \return uint8_t * Message string or 0 if not serialized properly.
     */
    uint8_t* packet() const;

    /**
     * \brief Getter of message ID
     * \return uint32_t size of message string
     */
    uint32_t packet_size() const;

    /**
     * \brief Getter of message size including protocol header
     * \return uint32_t size of message string
     */
    uint32_t packet_id() const;

    /*End of Serialization*/

    /*Deserialization*/

    /**
     * \brief Parses protocol header
     * \param message Incoming message string containing both header and
     * message body
     * \param messageSize Incoming message size
     * \return \saRESULT_CODE Status of serialization
     */
    RESULT_CODE deserializePacket(const uint8_t* message,
                                  uint32_t messageSize);

    /**
     * \brief Getter of protocol version.
     */
    uint8_t protocol_version() const;

    /**
     * \brief Getter of protection flag
     */
    bool protection_flag() const;

    /**
     * \brief Getter of frame type (single/first/etc)
     */
    uint8_t frame_type() const;

    /**
     *\brief Getter of service type (RPC/Bulk data)
     */
    uint8_t service_type() const;

    /**
     *\brief Getter of frame data (start/end session, number of frame etc)
     */
    uint8_t frame_data() const;

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
    uint8_t* data() const;

    /**
     *\brief Setter for size of multiframe message
     */
    void set_total_data_bytes(uint32_t dataBytes);

    /**
     *\brief Setter for new data
     */
    void set_data(const uint8_t* const  new_data,
                        const size_t new_data_size);

    /**
     *\brief Getter for size of multiframe message
     */
    uint32_t total_data_bytes() const;
    /*End of Deserialization*/

    /**
    * \brief Getter for Connection Identifier
    */
    uint8_t connection_key() const;

  private:
    /**
     *\brief Serialized message string
     */
    uint8_t* packet_;

    /**
     *\brief Serialized message string size
     */
    uint32_t total_packet_size_;

    /**
     *\brief Protocol header
     */
    ProtocolHeader packet_header_;

    /**
     *\brief Message body
     */
    ProtocolData packet_data_;

    /**
     *\brief Offset for multiframe messages
     */
    uint32_t data_offset_;

    /**
     *\brief ID for multiframe messages
     */
    uint32_t packet_id_;

    /**
    * \brief Connection Identifier
    * Obtained from connection_handler
    */
    uint8_t connection_key_;

    DISALLOW_COPY_AND_ASSIGN(ProtocolPacket);
};
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_PACKET_H_
