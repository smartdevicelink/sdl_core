/*
 * \file timer.cc
 * \brief 
 *
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

#include <pthread.h>

#include <transport_manager/timer.h>

namespace transport_manager {

Timer::Timer()
    : thread_(0),
      milliseconds(0),
      routine(nullptr),
      routineParam(nullptr),
      need_to_process(false) {
}

Timer::Timer(unsigned long milliseconds, void (*routine)(void*), void *param)
    : thread_(0),
      milliseconds(milliseconds),
      routine(routine),
      routineParam(param),
      need_to_process(true) {
}

void Timer::start() {
  pthread_cond_init(&cond_, nullptr);
  pthread_mutex_init(&mutex_, nullptr);
  pthread_create(&thread_, nullptr, &threadRoutine, this);
}

void Timer::stop() {
  need_to_process = false;
  pthread_cond_signal(&cond_);
  void *retval;
  pthread_join(thread_, &retval);
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
}

void* Timer::threadRoutine(void* p) {
  Timer *t = static_cast<Timer*>(p);
  timespec time;
  time.tv_nsec = t->milliseconds * 1000000;
  pthread_mutex_lock(&t->mutex_);
  pthread_cond_timedwait(&t->cond_, &t->mutex_, &time);
  if (t->need_to_process) {
    t->routine(t->routineParam);
  }
  // TODO (dmitry.chmerev@luxoft.com): to stop or not to stop? That's the question
  return nullptr;
}
}
