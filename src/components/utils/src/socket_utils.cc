/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "utils/socket_utils.h"

#if defined(OS_POSIX)
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#elif defined(OS_WINDOWS)
#include "utils/winhdr.h"
#endif

void utils::EnableKeepalive(int socket,
                            int keepalive_time_sec,
                            int keepalive_Interval_sec) {
#ifdef __linux__
  int yes = 1;
  int keepcnt = 5;
  int user_timeout = 7000;  // milliseconds
  setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
  setsockopt(socket,
             IPPROTO_TCP,
             TCP_KEEPIDLE,
             &keepalive_time_sec,
             sizeof(keepalive_time_sec));
  setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
  setsockopt(socket,
             IPPROTO_TCP,
             TCP_KEEPINTVL,
             &keepalive_Interval_sec,
             sizeof(keepalive_Interval_sec));
  setsockopt(socket,
             IPPROTO_TCP,
             TCP_USER_TIMEOUT,
             &user_timeout,
             sizeof(user_timeout));
#elif defined(OS_WINDOWS)
  struct tcp_keepalive settings;
  settings.onoff = 1;
  settings.keepalivetime = keepalive_time_sec * 1000;
  settings.keepaliveinterval = keepalive_Interval_sec * 1000;

  DWORD bytesReturned;
  WSAOVERLAPPED overlapped;
  overlapped.hEvent = NULL;
  WSAIoctl(socket,
           SIO_KEEPALIVE_VALS,
           &settings,
           sizeof(struct tcp_keepalive),
           NULL,
           0,
           &bytesReturned,
           &overlapped,
           NULL);
#elif defined(__QNX__)  // __linux__
  // TODO(KKolodiy): Out of order!
  int yes = 1;
  int keepcnt = 5;
  const int kMidLength = 4;
  int mib[kMidLength];

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPIDLE;
  sysctl(mib,
         kMidLength,
         NULL,
         NULL,
         &keepalive_time_sec,
         sizeof(keepalive_time_sec));

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPCNT;
  sysctl(mib, kMidLength, NULL, NULL, &keepcnt, sizeof(keepcnt));

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPINTVL;
  sysctl(mib,
         kMidLength,
         NULL,
         NULL,
         &keepalive_Interval_sec,
         sizeof(keepalive_Interval_sec));

  struct timeval tval = {0};
  tval.tv_sec = keepalive_time_sec;
  setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
  setsockopt(socket, IPPROTO_TCP, TCP_KEEPALIVE, &tval, sizeof(tval));
#endif                  // __QNX__
}
