/**
 * \file device_adapter_socket_communication.h
 * \brief Header for classes responsible for communication over sockets.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION

#include "transport_manager/device_adapter_impl.h"

namespace transport_manager {

namespace device_adapter {

template<typename TaskData>
class Task {
 public:
  virtual void perform(TaskData task_data) = 0;
 protected:
  virtual ~Task() {
  }
};

template<typename TaskData>
class Thread {
 public:
  Thread(TaskData task_data)
      : thread_(),
        task_(0),
        repeat_(false),
        task_data_(task_data) {
  }

  ~Thread() {
    pthread_join(thread_, 0);
  }

  bool start() {
    const int status = pthread_create(&thread_, 0, &Thread::threadFunc, this);
    return status == 0;
  }

  void setTask(Task<TaskData>* task) {
    task_ = task;
    repeat_ = false;
  }

  void setRepeatTask(Task<TaskData>* task) {
    task_ = task;
    repeat_ = true;
  }

  static void* threadFunc(void* arg) {
    Thread* thread = static_cast<Thread*>(arg);
    while (thread->task_) {
      Task<TaskData>* task = thread->task_;
      if (!thread->repeat_)
        thread->task_ = 0;
      task->perform(thread->task_data_);
    }
    return 0;
  }

 private:
  pthread_t thread_;
  Task<TaskData>* task_;
  bool repeat_;
  TaskData task_data_;
};

class ThreadedSocketConnection : public Connection {
 public:
  typedef utils::SharedPtr<ThreadedSocketConnection> Sptr;
  ThreadedSocketConnection(const DeviceHandle device_handle,
                           const ApplicationHandle app_handle,
                           const SessionID session_id);
  virtual ~ThreadedSocketConnection();
  Thread<Sptr>* thread() {
    return thread_;
  }
  void start(int socket);

  int socket() const {
    return socket_;
  }
  void notify() const;
  int notification_pipe_read_fd() const;

 private:
  ThreadedSocketConnection(const ThreadedSocketConnection&);
  Thread<Sptr>* thread_;
  int notification_pipe_read_fd_;
  int notification_pipe_write_fd_;
  int socket_;
};

class SocketDataTransmitter : public DataTransmitter, public Task<
    ThreadedSocketConnection::Sptr> {
 public:
  SocketDataTransmitter();
 protected:
  virtual Error init();
  virtual void terminate();
  virtual void registerConnection(ConnectionSptr connection);
  virtual void unregisterConnection(ConnectionSptr connection);
  virtual void notifyDataAvailable(ConnectionSptr connection);
  virtual void perform(ThreadedSocketConnection::Sptr task_data);
  virtual ~SocketDataTransmitter() {
  }

 private:
  bool receive(ThreadedSocketConnection::Sptr connection);
  bool clearNotificationPipe(ThreadedSocketConnection::Sptr connection);
  bool send(ThreadedSocketConnection::Sptr connection);
};

}  // namespace

}  // namespace

#endif //SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION
