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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_BYTE_ORDER_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_BYTE_ORDER_H_

#if defined(OS_POSIX)
#if defined(__QNX__)
#include <gulliver.h>
#define BE_TO_LE32(x) ENDIAN_SWAP32(&(x));
#define LE_TO_BE32(x) ENDIAN_SWAP32(&(x));
#else
#include <byteswap.h>
#define BE_TO_LE32(x) bswap_32(x)
#define LE_TO_BE32(x) bswap_32(x)
#endif
#elif defined(OS_WINDOWS)
#define bswap_16(x) (((x) << 8) & 0xff00) | (((x) >> 8) & 0xff)
#define bswap_32(x)                                      \
  (((x) << 24) & 0xff000000) | (((x) << 8) & 0xff0000) | \
      (((x) >> 8) & 0xff00) | (((x) >> 24) & 0xff)
#define bswap_64(x)                                                            \
  ((((x)&0xff00000000000000ull) >> 56) | (((x)&0x00ff000000000000ull) >> 40) | \
   (((x)&0x0000ff0000000000ull) >> 24) | (((x)&0x000000ff00000000ull) >> 8) |  \
   (((x)&0x00000000ff000000ull) << 8) | (((x)&0x0000000000ff0000ull) << 24) |  \
   (((x)&0x000000000000ff00ull) << 40) | (((x)&0x00000000000000ffull) << 56))
#define BE_TO_LE32(x) bswap_32(x)
#define LE_TO_BE32(x) bswap_32(x)
#endif

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_BYTE_ORDER_H_
