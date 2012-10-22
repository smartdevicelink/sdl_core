#ifndef __LOGGER_HELPER_H_
#define __LOGGER_HELPER_H_

#include "Logger.hpp"

namespace log4cplus
{
	
#define LOG4CPLUS_INFO_EXT(logger, logEvent) LOG4CPLUS_INFO(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_INFO_STR_EXT(logger, logEvent) LOG4CPLUS_INFO_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_TRACE_METHOD_EXT(logger, logEvent) LOG4CPLUS_TRACE_METHOD(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_TRACE_EXT(logger, logEvent) LOG4CPLUS_TRACE(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_TRACE_STR_EXT(logger, logEvent) LOG4CPLUS_TRACE_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_DEBUG_EXT(logger, logEvent) LOG4CPLUS_DEBUG(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_DEBUG_STR_EXT(logger, logEvent) LOG4CPLUS_DEBUG_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_WARN_EXT(logger, logEvent) LOG4CPLUS_WARN(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_WARN_STR_EXT(logger, logEvent) LOG4CPLUS_WARN_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_ERROR_EXT(logger, logEvent) LOG4CPLUS_ERROR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_ERROR_STR_EXT(logger, logEvent) LOG4CPLUS_ERROR_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_FATAL_EXT(logger, logEvent) LOG4CPLUS_FATAL(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
#define LOG4CPLUS_FATAL_STR_EXT(logger, logEvent) LOG4CPLUS_FATAL_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
	
};

#endif
