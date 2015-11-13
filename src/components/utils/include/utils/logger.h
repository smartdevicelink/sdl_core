/**
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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_

#ifdef ENABLE_LOG
  #include <errno.h>
  #include <string.h>
#if !defined(CUSTOM_LOG)
  #include <log4cxx/logger.h>
  #include <log4cxx/propertyconfigurator.h>
#else
    #include <string>
    #include <fstream>
    #include <ostream>
#endif

#if defined(OS_WIN32) 
#	if defined(DEBUG)
#		define __PRETTY_FUNCTION__ __FUNCTION__
#	else
#		define __PRETTY_FUNCTION__ __FUNCTION__
#	endif
#endif
#else // ENABLE_LOG
// do nothing
#endif // ENABLE_LOG

#ifdef OS_WIN32
# include <Windows.h>
# include <sstream>
#endif

namespace log4cxx {
#ifdef ENABLE_LOG
#if defined(CUSTOM_LOG)
 typedef int LogString;
 typedef int apr_pool_t;
 namespace helpers
 {
    class Pool
    {
       public:
        Pool(){};
        Pool(apr_pool_t* pool, bool release){};
        ~Pool();
				apr_pool_t* getAPRPool(){ return NULL;};
        apr_pool_t* create(){return NULL;};
        void* palloc(size_t length){return NULL;};
        char* pstralloc(size_t length){return NULL;};
        char* itoa(int n){return NULL;};
        char* pstrndup(const char* s, size_t len){return NULL;};
        char* pstrdup(const char*s){return NULL;};
        char* pstrdup(const std::string&){return NULL;};
                       
      };
 } // namespace helpers
    class LoggerPtr
    {
	public:
	LoggerPtr(){};
     };
    class Logger
    {
	public:
        Logger(log4cxx::helpers::Pool& pool, const LogString& name){};
	static LoggerPtr getLogger(const std::string& name){return LoggerPtr();};
        static LoggerPtr getLogger(const char* const name){return LoggerPtr();};
     };
		
   	
    #define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name) \
      namespace { \
        CREATE_LOGGERPTR_LOCAL(logger_var, logger_name); \
      }

    #define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name) \
      log4cxx::LoggerPtr logger_var = log4cxx::LoggerPtr(log4cxx::Logger::getLogger(logger_name));

    #define INIT_LOGGER(file_name) 

    // without this line log4cxx threads continue using some instances destroyed by exit()
    #define DEINIT_LOGGER() 

    #define LOG4CXX_IS_TRACE_ENABLED(logger) logger->isTraceEnabled()

    
   #define  LOGE(MSG)    {std::ofstream ff("/sdcard/sdllog.txt",std::iostream::out|std::iostream::app);ff<<"["<<__TIME__<<"]["<<__FILE__<<"]["<<__FUNCTION__<<"][Line:"<<__LINE__<<"]"<<MSG;ff.flush();ff.close();}

		
	#define LOG4CXX_INFO(logger,message)     LOGE(message<<"\n") 
	#define LOG4CXX_ERROR(logger,message)    LOGE(message<<"\n") 
	#define LOG4CXX_TRACE(logger,message)  	LOGE(message<<"\n") 
	#define LOG4CXX_WARN(logger,message)	  	LOGE(message<<"\n")   
	#define LOG4CXX_DEBUG(logger,message)	  LOGE(message<<"\n")  
	#define LOG4CXX_FATAL(logger,message)	  LOGE(message<<"\n")
		
		
    #define LOG4CXX_INFO_EXT(logger, logEvent) LOGE(__PRETTY_FUNCTION__<<": "<<logEvent<<"\n")
    #define LOG4CXX_INFO_STR_EXT(logger, logEvent) LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")

    #define LOG4CXX_TRACE_EXT(logger, logEvent) LOGE(TYPE_TRACE_EXT,__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_TRACE_STR_EXT(logger, logEvent)  LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_DEBUG_EXT(logger, logEvent)    LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_DEBUG_STR_EXT(logger, logEvent)  LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_WARN_EXT(logger, logEvent)   LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_WARN_STR_EXT(logger, logEvent)   LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")

    #define LOG4CXX_ERROR_EXT(logger, logEvent)    LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")
    #define LOG4CXX_ERROR_STR_EXT(logger, logEvent)    LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n")

    #define LOG4CXX_FATAL_EXT(logger, logEvent)   LOGE(TYPE_FATAL_EXT,__PRETTY_FUNCTION__ << ": " << logEvent<<"\n") //LOG4CXX_FATAL(logger, __PRETTY_FUNCTION__ << ": " << logEvent)
    #define LOG4CXX_FATAL_STR_EXT(logger, logEvent)   LOGE(__PRETTY_FUNCTION__ << ": " << logEvent<<"\n") //LOG4CXX_FATAL_STR(logger, __PRETTY_FUNCTION__ << ": " << logEvent)

    #define LOG4CXX_TRACE_ENTER(logger)   LOGE("ENTER:"<<__PRETTY_FUNCTION__<<"\n")//LOG4CXX_TRACE(logger, "ENTER: " << __PRETTY_FUNCTION__ )
    #define LOG4CXX_TRACE_EXIT(logger)    LOGE("EXIT:"<<__PRETTY_FUNCTION__<<"\n")
    #define LOG4CXX_ERROR_WITH_ERRNO(logger, message)    LOGE(message << ", error code " << errno << " (" << strerror(errno) << ")"<<"\n")
	
	#else // OS_Android

		#undef LOG4CXX_TRACE
		#define LOG4CXX_TRACE(x, y) LOG4CXX_INFO(x, y)

		#undef LOG4CXX_ERROR
		#define LOG4CXX_ERROR(x, y) LOG4CXX_INFO(x, y)

    #define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name) \
      namespace { \
        CREATE_LOGGERPTR_LOCAL(logger_var, logger_name); \
      }

    #define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name) \
      log4cxx::LoggerPtr logger_var = log4cxx::LoggerPtr(log4cxx::Logger::getLogger(logger_name));

    #define INIT_LOGGER(file_name) \
      log4cxx::PropertyConfigurator::configure(file_name);

    // without this line log4cxx threads continue using some instances destroyed by exit()
    #define DEINIT_LOGGER() \
      log4cxx::Logger::getRootLogger()->closeNestedAppenders();

    #define LOG4CXX_IS_TRACE_ENABLED(logger) logger->isTraceEnabled()

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
#ifdef OS_WINCE
#define LOG4CXX_ERROR_WITH_ERRNO(logger, message) LOG4CXX_ERROR(logger, message << ", error code " << errno )
#else // OS_WINCE
    #define LOG4CXX_ERROR_WITH_ERRNO(logger, message) LOG4CXX_ERROR(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")
#endif
#endif

#else // ENABLE_LOG is OFF

    #define CREATE_LOGGERPTR_GLOBAL(logger_var, logger_name)

    #define CREATE_LOGGERPTR_LOCAL(logger_var, logger_name)

    #define INIT_LOGGER(file_name)

    #define DEINIT_LOGGER(file_name)

    #define LOG4CXX_IS_TRACE_ENABLED(logger) false

    #undef LOG4CXX_INFO
    #define LOG4CXX_INFO(x,y)

    #undef LOG4CXX_WARN
    #define LOG4CXX_WARN(x,y)

    #undef LOG4CXX_ERROR
    #define LOG4CXX_ERROR(x,y)

    #undef LOG4CXX_ERROR_WITH_ERRNO
    #define LOG4CXX_ERROR_WITH_ERRNO(x,y)

    #undef LOG4CXX_TRACE
    #define LOG4CXX_TRACE(x,y)

    #undef LOG4CXX_DEBUG
    #define LOG4CXX_DEBUG(x,y)

    #undef LOG4CXX_FATAL
    #define LOG4CXX_FATAL(x,y)

    #define LOG4CXX_INFO_EXT(logger, logEvent)
    #define LOG4CXX_INFO_STR_EXT(logger, logEvent)

    #define LOG4CXX_TRACE_EXT(logger, logEvent)
    #define LOG4CXX_TRACE_STR_EXT(logger, logEvent)

    #define LOG4CXX_DEBUG_EXT(logger, logEvent)
    #define LOG4CXX_DEBUG_STR_EXT(logger, logEvent)

    #define LOG4CXX_WARN_EXT(logger, logEvent)
    #define LOG4CXX_WARN_STR_EXT(logger, logEvent)

    #define LOG4CXX_ERROR_EXT(logger, logEvent)
    #define LOG4CXX_ERROR_STR_EXT(logger, logEvent)

    #define LOG4CXX_FATAL_EXT(logger, logEvent)
    #define LOG4CXX_FATAL_STR_EXT(logger, logEvent)

    #define LOG4CXX_TRACE_ENTER(logger)
    #define LOG4CXX_TRACE_EXIT(logger)
#endif // ENABLE_LOG

}  // namespace log4cxx
#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOGGER_H_
