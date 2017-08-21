/*
 * @file named_pipe_notifier.cc
 * @brief NamedPipeNotifier class implementation file.
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

#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <errno.h>

#include "named_pipe_notifier.h"

void NamedPipeNotifier::run() {
  int fd = ::open(name_.toLocal8Bit().constData(), O_RDONLY);
  if (-1 == fd) {          // if open() fails
    if ((errno != ENOENT)  // we can only manage lack of pipe
        ||
        (-1 == ::mkfifo(name_.toLocal8Bit().constData(), 0666)) ||
        (-1 == (fd = ::open(name_.toLocal8Bit().constData(), O_RDONLY)))) {
      emit openFailed();
      return;
    }
  }
  ::fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(fd, &readfds);
  // this select() is supposed to block till pipe is empty
  if (::select(fd + 1, &readfds, 0, 0, 0) > 0) {
    emit readyRead();
  }
}
