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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_RESOURCE_USAGE_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_RESOURCE_USAGE_H_

#include <sys/resource.h>
#if defined(__QNXNTO__)
#include <sys/procfs.h>
#endif

#include <string>
#include <iostream>

#include "utils/logger.h"

#define MAX_COMM_LEN	128
#define MAX_CMDLINE_LEN	128

namespace utils {

struct ResourseUsage {
  long long int utime;
  long long int stime;
  long long int memory;
};

class Resources {
  public:
  typedef uint32_t MemInfo;
#if defined(__QNXNTO__)
  typedef procfs_info PidStats;

#elif defined(OS_LINUX)

  struct PidStats {
    int pid;
    char comm[MAX_COMM_LEN];
    char state;
    int ppid;
    int pgrp;
    int session;
    int tty_nr;
    int tpgid;
    unsigned int flags;
    unsigned long minflt;
    unsigned long cminflt;
    unsigned long majflt;
    unsigned long cmajflt;
    unsigned long utime;
    long stime;
    long cutime;
    long cstime;
    long priority;
    long nice;
    long num_threads;
    long itrealvalue;
    unsigned long long starttime;
    unsigned long vsize;
    long rss;
    unsigned long rsslim;
    unsigned long startcode;
    unsigned long endcode;
    unsigned long startstack;
    unsigned long kstkesp;
    unsigned long kstkeip;
    unsigned long signal;
    unsigned long blocked;
    unsigned long sigignore;
    unsigned long sigcatch;
    unsigned long wchan;
    unsigned long nswap;
    unsigned long cnswap;
    int exit_signal;
    int processor;
    unsigned int rt_priority;
    unsigned int policy;
    unsigned long long delayacct_blkio_ticks;
    unsigned long guest_time;
    long int cguest_time;
    };
#else

#endif
  public:
    /*
     * @brief Returns current resource usage of process
     * @return Raw pointer on  ResourseUsage if success, otherwise return NULL
     */
  static ResourseUsage* getCurrentResourseUsage();

private:

  /*
   * @brief reads /proc/PID/stat file on linux
   *        do not work on QNX ( return false, output wan't be changed )
   * @param output - storage for result string ( there will be separated content of /proc/PID/stat )
   * @return true on succes false onb fail
   */
  static bool ReadStatFile(std::string& output);

  /*
   * @brief Grab information about curent process
   * @param output - storage for result struct
   * @return true on succes false onb fail
   */
  static bool GetProcInfo(PidStats& output);

  /*
   * @brief Grab process memory information
   * @param output - storage for result struct
   * @return true on succes false onb fail
   */
  static bool GetMemInfo(MemInfo& output);

  /*
   * @brief return path to /proc/PID/stat file on linux
   *        return path to /proc/PID/as file on linux
   * @return path to file
   */
  static std::string GetStatPath();

  /*
   * @brief return path to /proc/PID directry
   * @return path to dir
   */
  static std::string GetProcPath();

  /*
   * path to /proc/ directory
   */
  static const char* proc;
};

}



#endif /* SRC_COMPONENTS_UTILS_INCLUDE_UTILS_RESOURCE_USAGE_H_ */
