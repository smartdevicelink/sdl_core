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

#include "utils/push_log.h"
#include "utils/log_message_loop_thread.h"
#include "utils/logger_status.h"

namespace logger {

static bool logs_enabled_ = false;
static LogMessageLoopThread* log_message_loop_thread = NULL;

bool push_log(log4cxx::LoggerPtr logger,
              log4cxx::LevelPtr level,
              const std::string& entry,
              log4cxx_time_t timeStamp,
              const log4cxx::spi::LocationInfo& location,
              const log4cxx::LogString& threadName
              ) {
  if (LoggerThreadCreated == logger_status) {
    LogMessage message = {logger, level, entry, timeStamp, location, threadName};
    if (log_message_loop_thread) {
      log_message_loop_thread->PostMessage(message);
      return true;
    }
  }

  if (LoggerThreadNotCreated == logger_status) {
    logger_status = CreatingLoggerThread;
// we'll have to drop messages
// while creating logger thread
    create_log_message_loop_thread();
    LogMessage message = {logger, level, entry, timeStamp, location, threadName};
    log_message_loop_thread->PostMessage(message);
    logger_status = LoggerThreadCreated;
    return true;
  }

// also we drop messages
// while deleting logger thread

  return false;
}

bool logs_enabled() {
  return logs_enabled_;
}

void set_logs_enabled(bool state) {
  logs_enabled_ = state;
}

void create_log_message_loop_thread() {
  if (!log_message_loop_thread) {
    log_message_loop_thread = new LogMessageLoopThread();
  }
}

void delete_log_message_loop_thread() {
  delete log_message_loop_thread;
  log_message_loop_thread = NULL;
}

void flush_logger() {
    logger::LoggerStatus old_status = logger::logger_status;
    // Stop pushing new messages to the log queue
    logger::logger_status = logger::DeletingLoggerThread;
    log_message_loop_thread->WaitDumpQueue();
    logger::logger_status = old_status;
}

}  // namespace logger
