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

#include <transport_manager/timer.h>

#include "errno.h"

namespace transport_manager {

Timer::Timer() : thread_(0),
    milliseconds_(0),
    func_(0),
    params_(0),
    single_shot_(false) {
  pthread_cond_init(&cond_, nullptr);
  pthread_mutex_init(&mutex_, nullptr);
}

Timer::Timer(unsigned long milliseconds, Callback func, void *params, bool single_shot)
    : thread_(0),
      milliseconds_(milliseconds),
      func_(func),
      params_(params),
      single_shot_(single_shot) {
  pthread_cond_init(&cond_, nullptr);
  pthread_mutex_init(&mutex_, nullptr);
}

Timer::Timer(const Timer &other)
    : thread_(0),
      milliseconds_(other.milliseconds_),
      func_(other.func_),
      params_(other.params_),
      single_shot_(other.single_shot_) {
  pthread_cond_init(&cond_, nullptr);
  pthread_mutex_init(&mutex_, nullptr);
}

Timer::~Timer() {
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
}

Timer& Timer::operator = (const Timer &other)
{
    if (this != &other)
    {
      milliseconds_ = other.milliseconds_;
      func_ = other.func_;
      params_ = other.params_;
      single_shot_ = other.single_shot_;
    }
    return *this;
}

void Timer::Start() {
  pthread_create(&thread_, nullptr, &ThreadRoutine, this);
}

void Timer::Stop() {
  pthread_mutex_lock(&mutex_);
  pthread_cond_signal(&cond_);
  pthread_mutex_unlock(&mutex_);
}

void* Timer::ThreadRoutine(void* p) {
  Timer *t = static_cast<Timer*>(p);
  timespec time;
  time.tv_nsec = t->milliseconds_ * 1000000;
  do {
    pthread_mutex_lock(&t->mutex_);
    int ret = pthread_cond_timedwait(&t->cond_, &t->mutex_, &time);
    pthread_mutex_unlock(&t->mutex_);
    if (ret == ETIMEDOUT) {
      t->func_(t->params_);
    } else {
      break;
    }
  } while (!t->single_shot_);
  return nullptr;
}

}  // namespace transport_manager
