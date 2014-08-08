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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_ERROR_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_ERROR_H_

#include <string>
#include <utils/shared_ptr.h>

namespace transport_manager {

/**
 * @brief Class that hold information about error.
 */
class BaseError {
 public:
  /**
   * @brief Constructor.
   */
  BaseError() {}

  /**
   * @brief Constructor.
   *
   * @param desc Description of error.
   */
  explicit BaseError(const std::string& desc) : description_(desc) {}

  /**
   * @brief Constructor.
   *
   * @param other Error class.
   */
  BaseError(const BaseError& other) : description_(other.description_) {}

  /**
   * @brief Destructor.
   */
  virtual ~BaseError() {}

  /**
   * @brief Return description of error.
   *
   * @return string with description of error.
   */
  virtual const std::string& text() const {
    return description_;
  }

 private:
  std::string description_;
};
typedef utils::SharedPtr<BaseError> BaseErrorPtr;

/**
 * @brief Error that originates during device search.
 */
class SearchDeviceError : public BaseError {
};

/**
 * @brief Error that originates during connection.
 */
class ConnectError : public BaseError {
};

/**
 * @brief Error that originates during disconnection.
 */
class DisconnectError : public BaseError {
};

/**
 * @brief Error that originates during device disconnection.
 */
class DisconnectDeviceError : public BaseError {
};

/**
 * @brief Error that originates during data receive..
 */
class DataReceiveError : public BaseError {
};

/**
 * @brief Error that originates during data send.
 */
class DataSendError : public BaseError {
 public:
  /**
   * @brief Constructor.
   */
  DataSendError() : BaseError() {}

  /**
   * @brief Constructor.
   *
   * @param Error description.
   */
  explicit DataSendError(const std::string& desc) : BaseError(desc) {}
};

/**
 * @brief Error that originates during data sending timeout.
 */
class DataSendTimeoutError : public DataSendError {
};


/**
 * @brief Error that originates during communication.
 */
class CommunicationError : public BaseError {
};

}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_ERROR_H_
