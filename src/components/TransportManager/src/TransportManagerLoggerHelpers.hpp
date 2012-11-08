#ifndef __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__
#define __TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__

// Log helpers for logs related to Connection Handle
#define TM_CH_LOG4CPLUS_INFO_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_INFO_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_INFO_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_METHOD_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_METHOD_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_TRACE_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_TRACE_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_DEBUG_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_DEBUG_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_WARN_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_WARN_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_ERROR_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_ERROR_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)
#define TM_CH_LOG4CPLUS_FATAL_STR_EXT(logger, ConnectionHandle, logEvent) LOG4CPLUS_FATAL_STR_EXT(logger, "[ConnectionHandle: " << ConnectionHandle << "]: " << logEvent)

#endif //__TRANSPORTMANAGER_TRANSPORTMANAGERLOGGERHELPERS_HPP__