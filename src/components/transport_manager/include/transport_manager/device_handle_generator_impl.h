/**
 * \file handle_generator.h
 * \brief HandleGeneratorImpl class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPOR_MANAGER_DEVICE_HANDLE_GENERATOR_IMPL
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPOR_MANAGER_DEVICE_HANDLE_GENERATOR_IMPL

#include <pthread.h>

#include "device_handle_generator.h"

namespace transport_manager {

/**
 * @brief Interface for device handle generator.
 * @interface DeviceHandleGenerator
 **/
class DeviceHandleGeneratorImpl : public DeviceHandleGenerator {
 public:

  DeviceHandleGeneratorImpl();

  /**
   * @brief Destructor.
   **/
  virtual ~DeviceHandleGeneratorImpl();

  /**
   * @brief Generate new device handle.
   *
   * Method used for generation of unique device handle.
   *
   * @return New device handle.
   **/
  virtual DeviceHandle generate();

 private:
  /**
   * @brief Mutex restricting access to new device handle generation
   **/
  mutable pthread_mutex_t device_handle_generation_mutex_;

  /**
   * @brief Last used device handle
   *
   * Used during device handle generation
   **/
  DeviceHandle last_used_device_handle_;

};

}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPOR_MANAGER_DEVICE_HANDLE_GENERATOR_IMPL
