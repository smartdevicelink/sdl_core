/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <log4cxx/logstring.h>
#include <log4cxx/appender.h>
#include <log4cxx/logger.h>
#include <log4cxx/varia/fallbackerrorhandler.h>
#include <log4cxx/helpers/loglog.h>
#include <log4cxx/helpers/stringhelper.h>
#include <log4cxx/spi/loggingevent.h>

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::spi;
using namespace log4cxx::varia;

IMPLEMENT_LOG4CXX_OBJECT(FallbackErrorHandler)

FallbackErrorHandler::FallbackErrorHandler()
    : backup_(), primary_(), loggers() {
}

void FallbackErrorHandler::setLogger(const LoggerWeakPtr& logger) {
    LogLog::debug(((LogString) LOG4CXX_STR("FB: Adding logger ["))
                  + logger.lock()->getName() + LOG4CXX_STR("]."));
    loggers.push_back(logger);
}

void FallbackErrorHandler::error(const LogString& message,
                                 const std::exception& e,
                                 int errorCode) const {
    error(message, e, errorCode, 0);
}

void FallbackErrorHandler::error(const LogString& message,
                                 const std::exception& e,
                                 int, const spi::LoggingEventPtr&) const {
    LogLog::debug(((LogString) LOG4CXX_STR("FB: The following error reported: "))
                  +  message, e);
    LogLog::debug(LOG4CXX_STR("FB: INITIATING FALLBACK PROCEDURE."));

    auto primary = primary_.lock();
    auto backup = backup_.lock();

    for(size_t i = 0; i < loggers.size(); i++) {
        auto& logger = (LoggerWeakPtr&)loggers.at(i);
        auto  l = logger.lock();
        LogLog::debug(((LogString) LOG4CXX_STR("FB: Searching for ["))
                      + primary->getName() + LOG4CXX_STR("] in logger [")
                      + l->getName() + LOG4CXX_STR("]."));
        LogLog::debug(((LogString) LOG4CXX_STR("FB: Replacing ["))
                      + primary->getName() + LOG4CXX_STR("] by [")
                      + backup->getName() + LOG4CXX_STR("] in logger [")
                      + l->getName() + LOG4CXX_STR("]."));
        l->removeAppender(primary);
        LogLog::debug(((LogString) LOG4CXX_STR("FB: Adding appender ["))
                      + backup->getName() + LOG4CXX_STR("] to logger ")
                      + l->getName());
        l->addAppender(backup);
    }
}

void FallbackErrorHandler::setAppender(const AppenderWeakPtr& primary1) {
    LogLog::debug(((LogString) LOG4CXX_STR("FB: Setting primary appender to ["))
                  + primary1.lock()->getName() + LOG4CXX_STR("]."));
    primary_ = primary1;
}

void FallbackErrorHandler::setBackupAppender(const AppenderWeakPtr& backup1) {
    LogLog::debug(((LogString) LOG4CXX_STR("FB: Setting backup appender to ["))
                  + backup1.lock()->getName() + LOG4CXX_STR("]."));
    backup_ = backup1;
}

void FallbackErrorHandler::activateOptions(Pool&) {
}

void FallbackErrorHandler::setOption(const LogString&, const LogString&) {
}

