/*
 * \file timer.h
 * \brief Timer class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TIMER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TIMER_H_

#include <pthread.h>

namespace transport_manager {

typedef void (*Callback)(void* param);

/**
 * @brief Class in charge of timers.
 */
class Timer {
 public:

  /**
   * @brief Constructor.
   */
  Timer();

  /**
   * @brief Constructor.
   *
   * @param milliseconds value of timer.
   * @param func Pointer to function that will be launched when timer runs out.
   * @param params Parameters for Callback function.
   * @param single_shot If true - timer call function once.
   * if false - timer call function every time timer runs out.
   */
  Timer(unsigned long milliseconds, Callback func, void *params, bool single_shot);

  /**
   * @brief Constructor.
   */
  Timer(const Timer &other);

  /**
   * @brief Destructor.
   */
  ~Timer();

  /**
   * @brief Overloaded operator "=";
   */
  Timer& operator = (const Timer &other);

  /**
   * @brief Start timer.
   */
  void Start();

  /**
   * @brief Stop timer.
   */
  void Stop();

 private:
  pthread_t thread_;
  pthread_cond_t cond_;
  pthread_mutex_t mutex_;
  unsigned long milliseconds_;
  Callback func_;
  void *params_;
  volatile bool single_shot_;

  static void *ThreadRoutine(void* param);
};

}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TIMER_H_
