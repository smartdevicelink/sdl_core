/*
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_COMMON_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_COMMON_H_

#include <vector>
#include <string>

/**
 * @brief - transport_manager namespace
 */
namespace transport_manager {

/**
 * @enum Transport manager states.
 */
enum {
  E_SUCCESS = 0,
  E_TM_IS_NOT_INITIALIZED,
  E_INVALID_HANDLE,
  E_CONNECTION_IS_TO_SHUTDOWN,
  E_CONNECTION_EXISTS,
  E_ADAPTER_EXISTS,
  E_ADAPTERS_FAIL,
  E_INTERNAL_ERROR
};

/**
 * @brief Type definition for variable that hold handle of device.
 */
typedef unsigned int DeviceHandle;

/**
 * @brief Type definition for variable that hold connection unique identifier.
 */
typedef unsigned int ConnectionUID;

/**
 * @brief Type definition for variable that hold connection type.
 */
typedef std::string ConnectionType;

/**
 * @brief Type definition for variable that hold device unique identifier.
 */
typedef std::string DeviceUID;

/**
 * @brief Type definition of container(vector) that holds device unique
 * identifiers.
 */
typedef std::vector<DeviceUID> DeviceList;

/**
 * @brief Type definition for variable that hold handle of application.
 */
typedef int ApplicationHandle;

/**
 * @brief Type definition for vector that contain ApplicationHandle variables.
 */
typedef std::vector<ApplicationHandle> ApplicationList;
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_COMMON_H_
