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
#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_SERVICE_TYPE_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_SERVICE_TYPE_H_

#include <stdint.h>

namespace protocol_handler {

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
  * \brief Enum describing possible types of sessions: RPC for API messages,
    Navi for video streaming, bulk for PutFile.
  */
enum ServiceType {
  kControl =   SERVICE_TYPE_CONTROL,
  kRpc =       SERVICE_TYPE_RPC,
  kAudio =     SERVICE_TYPE_AUDIO,
  kMobileNav = SERVICE_TYPE_NAVI,
  kBulk =      SERVICE_TYPE_BULK,
  kInvalidServiceType
};

/*
 * Service type conversion functions that are used to read and output
 * Service types to binary stream
 */
ServiceType ServiceTypeFromByte(uint8_t type);
uint8_t ServiceTypeToByte(ServiceType type);

}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_SERVICE_TYPE_H_
