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
 *\brief Constant: number of protocol version (3).
 */
const uint8_t PROTOCOL_VERSION_3 = 0x03;

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
 *\brief frame header (used Ethernet MTU as default target transport)
 */
const uint32_t MAXIMUM_FRAME_DATA_SIZE = 1500;

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
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_COMMON_H_
