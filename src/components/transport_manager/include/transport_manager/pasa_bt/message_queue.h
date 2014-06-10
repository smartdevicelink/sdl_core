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

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <mqueue.h>
#include <stdint.h>

namespace transport_manager {
namespace transport_adapter {

#define MAX_QUEUE_NAME_SIZE      24
#define MAX_QUEUE_MSG_SIZE     4095
#define MSGQ_MAX_MESSAGES       128
#define MSGQ_MESSAGE_SIZE      MAX_QUEUE_MSG_SIZE

/**
 * @brief Establish connection between a process and a message queue
 * @param queue_name Name of message queue
 * @param is_outcome shall be true for sending message queue, false for income message queue
 * @param is_blocking shall be true for blocking message queue
 *
 * @return message queue descriptor or -1 on error
 */
mqd_t OpenMsgQ(const char *queue_name,
               const bool is_outcome,
               const bool is_blocking);

/**
 * @brief Close message queue by descriptor
 *
 * @param q_fd message queue descriptor
 */
void CloseMsgQ(const mqd_t q_fd);

/**
 * @brief Send data to message queue
 *
 * @param q_fd message queue descriptor
 * @param msgType message type
 * @param length length of data to be send
 * @param data pointer to data array
 *
 * @return On success return zero; on error -1 is
     returned, with errno set to indicate the error.
 */
int SendMsgQ(const mqd_t q_fd,
            const uint8_t msgType,
            const uint32_t length,
            const void* const data);

}  // namespace transport_adapter
}  // namespace transport_manager
#endif // MESSAGE_QUEUE_H
