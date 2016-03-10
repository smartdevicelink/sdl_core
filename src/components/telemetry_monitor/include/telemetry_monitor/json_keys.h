/*
 *
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

#ifndef SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_JSON_KEYS_H_
#define SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_JSON_KEYS_H_

namespace telemetry_monitor {
  namespace strings {
    const char logger[] = "logger";
    const char begin[] = "begin";
    const char end[] = "end";
    const char data_size[] = "data_size";
    const char message_id[] = "message_id";
    const char session_id[] = "session_id";
    const char correlation_id[] = "correlation_id";
    const char connection_key[] = "connection_key";
    const char stime[] = "stime";
    const char utime[] = "utime";
    const char memory[] = "RAM";
  }
}
#endif  // SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_JSON_KEYS_H_
