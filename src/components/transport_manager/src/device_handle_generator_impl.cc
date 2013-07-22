/**
 * \file device_handle_generator_impl.cpp
 * \brief Class DeviceHandleGeneratorImpl.
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

#include "transport_manager/device_handle_generator_impl.h"

namespace transport_manager {

DeviceHandleGenerator::~DeviceHandleGenerator() {
}

DeviceHandleGeneratorImpl::DeviceHandleGeneratorImpl()
    : last_used_device_handle_(0) {
  pthread_mutex_init(&device_handle_generation_mutex_, 0);
}

DeviceHandleGeneratorImpl::~DeviceHandleGeneratorImpl() {
  pthread_mutex_destroy(&device_handle_generation_mutex_);
}

DeviceHandle DeviceHandleGeneratorImpl::generate() {
  DeviceHandle output_device_handle;

  pthread_mutex_lock(&device_handle_generation_mutex_);
  ++last_used_device_handle_;
  output_device_handle = last_used_device_handle_;
  pthread_mutex_unlock(&device_handle_generation_mutex_);

  return output_device_handle;
}

}  // namespace transport_manager

