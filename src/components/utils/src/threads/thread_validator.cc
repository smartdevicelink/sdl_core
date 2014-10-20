/**
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
#include "utils/threads/thread_validator.h"

#include "utils/logger.h"
#include "utils/back_trace.h"

namespace threads {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

SingleThreadSimpleValidator::SingleThreadSimpleValidator()
    : creation_thread_id_(Thread::CurrentId()) {
}

SingleThreadSimpleValidator::~SingleThreadSimpleValidator() {
}

void SingleThreadSimpleValidator::AssertRunningOnCreationThread() const {
  Thread::Id current_id = Thread::CurrentId();
  if (creation_thread_id_ != current_id) {
    LOG4CXX_ERROR(logger_, "Single-threaded object created at thread "
                          << creation_thread_id_
                          <<" is accessed from thread "
                          << current_id
#ifdef BACKTRACE_SUPPORT
                          << "\n"
                          << utils::Backtrace()
#endif
    );
  }
}


SingleThreadValidator::SingleThreadValidator()
    : owning_thread_id_(Thread::CurrentId()){
}

SingleThreadValidator::~SingleThreadValidator() {
}

void SingleThreadValidator::PassToThread(Thread::Id thread_id) const {
  owning_thread_id_ = thread_id;
}

void SingleThreadValidator::AssertRunningOnValidThread() const {
  Thread::Id current_id = Thread::CurrentId();
  if (owning_thread_id_ != current_id) {
    LOG4CXX_ERROR(logger_, "Single-threaded object owned by thread "
                         << owning_thread_id_
                         << " is accessed from thread "
                         << current_id << "\n"
#ifdef BACKTRACE_SUPPORT
                         << utils::Backtrace()
#endif
                         );
  }
}


} // namespace threads

// vim: set ts=2 sw=2 et:
