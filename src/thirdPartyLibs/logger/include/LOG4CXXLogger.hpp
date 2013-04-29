/*
 * LOG4CXXLogger.hpp
 *
 *  Created on: Apr 26, 2013
 *      Author: meskalito
 */

#ifndef LOG4CXXLOGGER_HPP_
  #include <log4cxx/logger.h>
  #include <log4cxx/propertyconfigurator.h>

namespace log4cplus
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

    // @TODO: Move this defibition to components/Utils/include/Utils/macro.h
    // after log4cplus is completely removed from the project
    #define DCHECK(condition) \
        if (!condition) { \
            LOG4CXX_ERROR_EXT(logger_, "Check failed: " #condition); \
        assert(false); \
    }
};

#define LOG4CXXLOGGER_HPP_


#endif /* LOG4CXXLOGGER_HPP_ */
