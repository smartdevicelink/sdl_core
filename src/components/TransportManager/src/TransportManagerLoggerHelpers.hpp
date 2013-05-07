/**
 * \file TransportManagerLoggerHelpers.hpp
 * \brief Class TransportManagerLoggerHelpers header.
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

#ifndef __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__
#define __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__

// Log helpers for logs related to Connection Handle
#define TM_CH_LOG4CXX_INFO(logger, ConnectionHandle, logEvent) LOG4CXX_INFO(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_INFO_STR(logger, ConnectionHandle, logEvent) LOG4CXX_INFO_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_TRACE_METHOD(logger, ConnectionHandle, logEvent) LOG4CXX_TRACE_METHOD(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_TRACE(logger, ConnectionHandle, logEvent) LOG4CXX_TRACE(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_TRACE_STR(logger, ConnectionHandle, logEvent) LOG4CXX_TRACE_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_DEBUG(logger, ConnectionHandle, logEvent) LOG4CXX_DEBUG(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_DEBUG_STR(logger, ConnectionHandle, logEvent) LOG4CXX_DEBUG_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_WARN(logger, ConnectionHandle, logEvent) LOG4CXX_WARN(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_WARN_STR(logger, ConnectionHandle, logEvent) LOG4CXX_WARN_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_ERROR(logger, ConnectionHandle, logEvent) LOG4CXX_ERROR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_ERROR_STR(logger, ConnectionHandle, logEvent) LOG4CXX_ERROR_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_FATAL(logger, ConnectionHandle, logEvent) LOG4CXX_FATAL(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CXX_FATAL_STR(logger, ConnectionHandle, logEvent) LOG4CXX_FATAL_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)

#endif //__TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__
