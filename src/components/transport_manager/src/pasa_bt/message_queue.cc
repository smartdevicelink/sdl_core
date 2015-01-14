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

#include <string.h>

#include "transport_manager/pasa_bt/message_queue.h"

namespace transport_manager {
namespace transport_adapter {

mqd_t OpenMsgQ(const char *queue_name,
               const bool is_outcome,
               const bool is_blocking) {
  struct mq_attr attributes;
  attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
  attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
  attributes.mq_flags = 0;
  const int open_flags = O_CREAT |
      (is_outcome ? O_WRONLY : O_RDONLY) |
      (is_blocking ? 0 : O_NONBLOCK);
  return mq_open(queue_name, open_flags, 0666, &attributes);
}

void CloseMsgQ(const mqd_t q_fd) {
  mq_close(q_fd);
}

int SendMsgQ(const mqd_t q_fd,
             const uint8_t msgType,
             const uint32_t length,
             const void *const data) {
  uint8_t sendBuf[MSGQ_MESSAGE_SIZE];
  if(!data || length <= 0 || (length-1) > MSGQ_MESSAGE_SIZE) {
    return -1;
  }
  memset(sendBuf, MSGQ_MESSAGE_SIZE, sizeof(*sendBuf));
  sendBuf[0] = msgType;
  // copy the msg to send buffer
  memcpy(sendBuf + 1, data, length);
  return mq_send(q_fd, (const char *)&sendBuf[0], sizeof(sendBuf), 0);
}

}  // namespace transport_adapter
}  // namespace transport_manager
