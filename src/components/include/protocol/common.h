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

#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_COMMON_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_COMMON_H_

#include "utils/macro.h"
#include "protocol/raw_message.h"

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {

/**
 *\brief Size of protocol header version 1.
 */
const uint8_t PROTOCOL_HEADER_V1_SIZE = 8;

/**
 *\brief Size of protocol header version 2 and higher.
 */
const uint8_t PROTOCOL_HEADER_V2_SIZE = 12;

/**
 *\brief Protocol versions constants
 * First 4-bit field of AppLink protocol packet
 */
enum {
  /**
   *\brief Constant: number of protocol version (1).
   */
  PROTOCOL_VERSION_1 = 0x01,
  /**
   *\brief Constant: number of protocol version (2).
   * Has difference with v.1 in Mobile_API.xml and frame Header difference
   * Support SSL with Encryption, Video and Audio services
   */
  PROTOCOL_VERSION_2 = 0x02,
  /**
   *\brief Constant: number of protocol version (3).
   * Has no difference with v.2 in Mobile_API.xml and frame Header difference
   * Support SSL with Encryption, Video and Audio services
   * Suuport ControlFrame, End Service to/from mobile support, HeartBeat future
   */
  PROTOCOL_VERSION_3 = 0x03,
  /**
   *\brief Maximum value of packet version field (size 4-bit) specified AppLink Protocol v.7
   */
  PROTOCOL_VERSION_MAX = 0x0F
};

/**
 *\brief Flag of protection packet
 * 1-bit field of AppLink protocol packet
 */
const bool PROTECTION_ON  = true;
const bool PROTECTION_OFF = false;

/**
 *\brief Protocol frame type constants
 * 3-bit field of AppLink protocol packet
 */
enum {
  /**
  *\brief Constant: Control type of frame
  */
  FRAME_TYPE_CONTROL = 0x00,
  /**
  *\brief Constant: Single type of frame.
  */
  FRAME_TYPE_SINGLE = 0x01,
  /**
  *\brief Constant: First frame for multiple frames
  */
  FRAME_TYPE_FIRST = 0x02,
  /**
  *\brief Constant: Consecutive type of frame for multiple frames
  */
  FRAME_TYPE_CONSECUTIVE = 0x03,
  /**
   *\brief Maximum value of frame type field (size 3-bit)
   */
  FRAME_TYPE_MAX_VALUE = 0x07
};

/**
 *\brief Protocol frame info
 * 8-bit field of AppLink protocol packet
 */
enum {
  // Frame type FRAME_TYPE_CONTROL (0x0)
  /**
   *\brief Hartbeat frame type
   */
  FRAME_DATA_HEART_BEAT = 0x00,
  /**
   *\brief Start service frame
   */
  FRAME_DATA_START_SERVICE = 0x01,
  /**
   *\brief Start service acknowledgement frame
   */
  FRAME_DATA_START_SERVICE_ACK = 0x02,
  /**
   *\brief Start service not acknowledgement frame
   */
  FRAME_DATA_START_SERVICE_NACK = 0x03,
  /**
   *\brief End service request frame
   */
  FRAME_DATA_END_SERVICE = 0x04,
  /**
   * \brief End service acknowledgement frame
   */
  FRAME_DATA_END_SERVICE_ACK = 0x05,
  /**
   *\brief End service not acknowledgement frame
   */
  FRAME_DATA_END_SERVICE_NACK = 0x06,
  /**
   *\brief Service data ACK frame
   */
  FRAME_DATA_SERVICE_DATA_ACK = 0xFE,
  /**
   *\brief Frame is for heart beat ack
   */
  FRAME_DATA_HEART_BEAT_ACK = 0xFF,

  // Frame type FRAME_TYPE_SINGLE (0x2)
  /**
   *\brief Single frame has reserver frame data value 0x0
   */
  FRAME_DATA_SINGLE = 0x00,

  // Frame type FRAME_TYPE_FIRST (0x3)
  /**
   *\brief First frame has reserver frame data value 0x0
   */
  FRAME_DATA_FIRST = 0x00,

  // Frame type FRAME_TYPE_CONSECUTIVE (0x3)
  /**
   *\brief Marks last frame in mutliframe message
   */
  FRAME_DATA_LAST_CONSECUTIVE = 0x00,
  /**
   *\brief Maximum of consecutive frame numbers after which count has to roll over to 0x01
   */
  FRAME_DATA_MAX_CONSECUTIVE = 0xFF,

  /**
   *\brief Maximum value of frame info field (size 8-bit)
   */
  FRAME_DATA_MAX_VALUE = 0xFF
};

/**
 *\brief If FRAME_TYPE_CONTROL: Constant: Maximum size of one frame excluding
 *\brief frame header (used Ethernet MTU as default target transport)
 */
const uint32_t MAXIMUM_FRAME_DATA_SIZE = 1500;

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
  RESULT_HEARTBEAT_IS_NOT_SUPPORTED = 17,
  RESULT_UNKNOWN = 255
};
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_COMMON_H_
