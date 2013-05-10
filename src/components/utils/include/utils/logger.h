/**
 * \file LOG4CXXLogger.hpp
 * \brief Definitions required by logger.
 * Stores device information
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

#ifndef LOG4CXXLOGGER_HPP_
  #include <log4cxx/logger.h>
  #include <log4cxx/propertyconfigurator.h>

namespace log4cxx
{
    #define LOG4CXX_INFO_EXT(logger, logEvent) LOG4CXX_INFO(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_INFO_STR_EXT(logger, logEvent) LOG4CXX_INFO_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_TRACE_EXT(logger, logEvent) LOG4CXX_TRACE(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_TRACE_STR_EXT(logger, logEvent) LOG4CXX_TRACE_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_DEBUG_EXT(logger, logEvent) LOG4CXX_DEBUG(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_DEBUG_STR_EXT(logger, logEvent) LOG4CXX_DEBUG_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_WARN_EXT(logger, logEvent) LOG4CXX_WARN(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_WARN_STR_EXT(logger, logEvent) LOG4CXX_WARN_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_ERROR_EXT(logger, logEvent) LOG4CXX_ERROR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_ERROR_STR_EXT(logger, logEvent) LOG4CXX_ERROR_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_FATAL_EXT(logger, logEvent) LOG4CXX_FATAL(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_FATAL_STR_EXT(logger, logEvent) LOG4CXX_FATAL_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_TRACE_ENTER(logger) LOG4CXX_TRACE(logger, "ENTER: " << __PRETTY_FUNCTION__ )
    #define LOG4CXX_TRACE_EXIT(logger) LOG4CXX_TRACE(logger, "EXIT: " << __PRETTY_FUNCTION__ )

    #define LOG4CXX_ERROR_WITH_ERRNO(logger, message) LOG4CXX_ERROR(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")
};

#define LOG4CXXLOGGER_HPP_


#endif /* LOG4CXXLOGGER_HPP_ */
