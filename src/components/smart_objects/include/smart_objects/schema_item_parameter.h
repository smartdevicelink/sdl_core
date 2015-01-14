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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_PARAMETER_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_PARAMETER_H_

#include "utils/macro.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
/**
 * @brief SchemaItem parameter.
 * Container for schema item parameter such as min/max value, size,
 * etc. which may be unspecified.
 * @tparam ParameterType Type of a parameter.
 */
template<typename ParameterType>
class TSchemaItemParameter FINAL {
 public:
  /**
   * @brief Constructor.
   * Initialize unspecified parameter.
   **/
  TSchemaItemParameter();
  /**
   * @brief Constructor.
   * Initialize parameter with specified value.
   * @param ParameterValue Value of a parameter.
   **/
  explicit TSchemaItemParameter(const ParameterType& ParameterValue);

  /**
   * @brief Get parameter value.
   * Get the value of the parameter if it is specified.
   * @param Value Reference to variable that must receive the
   *              value of the parameter. If parameter is not
   *              specified this variable is not modified.
   * @return true if the parameter is specified and its value
   *         has been stored in Value variable, false otherwise.
   **/
  bool getValue(ParameterType& Value) const;

 private:
  /**
   * @brief true if parameter is specified, false otherwise.
   **/
  const bool mIsSpecified;
  /**
   * @brief Parameter value.
   **/
  const ParameterType mValue;
};

template<typename ParameterType>
TSchemaItemParameter<ParameterType>::TSchemaItemParameter()
    : mIsSpecified(false),
      mValue() {
}

template<typename ParameterType>
TSchemaItemParameter<ParameterType>::TSchemaItemParameter(
    const ParameterType& ParameterValue)
    : mIsSpecified(true),
      mValue(ParameterValue) {
}

template<typename ParameterType>
bool TSchemaItemParameter<ParameterType>::getValue(ParameterType& Value) const {
  if (mIsSpecified) {
    Value = mValue;
  }
  return mIsSpecified;
}
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_PARAMETER_H_
