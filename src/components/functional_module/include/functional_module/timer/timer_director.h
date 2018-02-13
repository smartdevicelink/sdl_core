/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_TIMER_DIRECTOR_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_TIMER_DIRECTOR_H_

#include <map>
#include "utils/threads/thread.h"
#include "utils/conditional_variable.h"
#include "functional_module/timer/module_timer.h"

namespace functional_modules {

template <class T>
class TimerThreadDelegate : public threads::ThreadDelegate {
 public:
  explicit TimerThreadDelegate(ModuleTimer<T>& timer);
  void threadMain();
  void exitThreadMain();

  /**
   * @brief Reset awaiting timer by notifying cond var
   */
  void ResetTimer();

 private:
  ModuleTimer<T>& timer_;
  volatile bool keep_running_;
  mutable sync_primitives::Lock keep_running_lock_;
  mutable sync_primitives::ConditionalVariable keep_running_cond_;
  friend class TimerThreadDelegateTest;
};

class TimerDirector {
 public:
  TimerDirector();
  ~TimerDirector();

  /**
   * @brief Register timer for execution in separate thread.
   Registers only one timer of a type. Attempt to register timer
   of already existing type will fail.
   */
  template <class T>
  void RegisterTimer(ModuleTimer<T>& timer);
  template <class T>
  void UnregisterTimer(const ModuleTimer<T>& timer);
  void UnregisterAllTimers();
  /**
   * @brief Reset awaiting timeout for specified module timer
   * @param timer timer to reset awaiting timeout
   */
  template <class T>
  void ResetTimer(ModuleTimer<T>& timer);

 private:
  DISALLOW_COPY_AND_ASSIGN(TimerDirector);
  std::map<std::string, threads::Thread*> timer_threads_;
};

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_TIMER_DIRECTOR_H_
