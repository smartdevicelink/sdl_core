/*
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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BACK_TRACE_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BACK_TRACE_H_

#include <ostream>
#include <vector>
#include <string>
#include "utils/threads/thread.h"

namespace utils {

/*
 * Class that captures stack trace in place where it was created.
 * Can be stored and passed for further processing
 * ostream output operator is available for these objects so stacktrace
 * here and now can be easily printed as
 *
 * std::cout<<utils::Backtrace()<<std::endl;
 */
class Backtrace {
 public:
  // Inspect stack up to 128 calls back
  static const int32_t kDefaultDepth = 128;
  static const int32_t kSkipTop = 0;
  /* Capture backtrace and store. Limit captured stack length to
   * count symbols and remove first skip_top elements from it
   * (to avoid polluting stack trace with debugging function names)
   */
  Backtrace(int32_t count = kDefaultDepth, int32_t skip_top = kSkipTop);
  ~Backtrace();

  // Captured symbols in order from topmost stack frame to last captured
  std::vector<std::string> CallStack() const;
  threads::PlatformThreadHandle ThreadId() const;

 private:
  threads::PlatformThreadHandle thread_id_;
  std::vector<void*> backtrace_;
};

std::ostream& operator<< (std::ostream& os, const Backtrace& bt);

} // namespace utils

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BACK_TRACE_H_
