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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SIGNALS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SIGNALS_H_

#ifdef __QNXNTO__
typedef void (*sighandler_t)(int);
#else
#include <signal.h>
#endif
#include "appMain/low_voltage_signals_handler.h"

namespace utils {

class Signals {
 public:
  /**
   * @brief Unsubscribe thread from termination signals SIGINT and SIGTERM
   * @return True if thread unsubscribed successfuly, otherwise false
   */
  static bool UnsubscribeFromTermination();

  /**
   * @brief Triggers thread to wait for termination signals SIGINT and SIGTERM
   * @param sig_handler - handler to work with signals specidied above
   * @return True if handler handles signals successfuly, otherwise false
   */
  static bool WaitTerminationSignals(sighandler_t sig_handler);

  /**
   * @brief Unsubscribe thread from low voltage signals
   *  SIGLOWVOLTAGE, SIGWAKEUP and SIGIGNOFF
   * @return True if thread unsubscribed successfuly, otherwise false
   */
  static bool UnsubscribeFromLowVoltageSignals(
      const main_namespace::LowVoltageSignalsOffset& offset_data);

  /**
   * @brief Sends signal to specified process
   * @param signal to send
   * @param destination process signal to be sent to
   */
  static void SendSignal(const int signo, const pid_t pid);

  /**
   * @brief Creates child process
   * @return created process id or -1 in case of error
   */
  static pid_t Fork();

  /**
   * @brief Wait for child process to be terminated
   * @param cpid - process to wait for termination
   * @param status store status information in the int to which it points
   * @param options - options for exit form function
   * detailed options can be found here: https://linux.die.net/man/2/waitpid
   */
  static void WaitPid(pid_t cpid, int* status, int options);

  /**
   * @brief Exits for process
   * @param status - exit status code
   */
  static void ExitProcess(const int status);
};

}  //  namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SIGNALS_H_
