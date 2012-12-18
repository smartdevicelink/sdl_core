#ifndef __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__
#define __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__

// Log helpers for logs related to Connection Handle
#define TM_CH_LOG4CPLUS_INFO(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_INFO_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_METHOD(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_METHOD(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL_STR(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL_STR(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)

#endif //__TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__