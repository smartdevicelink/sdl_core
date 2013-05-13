/**
* \file request_watchdog.h
* \brief DateTime class source file.
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

#include <sys/time.h>
#include "utils/date_time.h"

namespace date_time {

int const DateTime::MILLISECONDS_IN_SECOND;
int const DateTime::MICROSECONDS_IN_MILLISECONDS;

struct timeval DateTime::getCurrentTime() {
  struct timeval currentTime;
  struct timezone timeZone;

  gettimeofday(&currentTime, &timeZone);

  return currentTime;
}

int DateTime::calculateTimeSpan(struct timeval sinceTime) {
  struct timeval currentTime, timeDifference;
  struct timezone timeZone;

  gettimeofday(&currentTime, &timeZone);

  timeDifference.tv_sec = currentTime.tv_sec - sinceTime.tv_sec;

  timeDifference.tv_usec = currentTime.tv_usec - sinceTime.tv_usec;

  if ( timeDifference.tv_usec < 0 ) {
    timeDifference.tv_sec--;
    timeDifference.tv_usec += MILLISECONDS_IN_SECOND
                            * MICROSECONDS_IN_MILLISECONDS;
  }

  return timeDifference.tv_sec * MILLISECONDS_IN_SECOND
       + timeDifference.tv_usec / MICROSECONDS_IN_MILLISECONDS;
}

}  // namespace date_time
