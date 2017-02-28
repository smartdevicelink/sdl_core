/*
 * Copyright (c) 2017, Ford Motor Company
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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_OPTIONAL_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_OPTIONAL_H_
namespace optional {
template <typename Type>
class Optional {
 public:
  typedef Optional<Type> OptionalImpl;
  // Methods
  Optional() : value_state_(false) {}
  explicit Optional(const Type& value) {
    this->value_state_ = true;
    value_ = value;
  }
  OptionalImpl& operator=(Type new_val) {
    this->value_state_ = true;
    value_ = new_val;
    return *this;
  }
  OptionalImpl& operator=(const OptionalImpl& new_val) {
    this->value_ = new_val.value_;
    if (new_val.is_initialized()) {
      this->value_state_ = new_val.value_state_;
    }
    return *this;
  }
  const Type& operator*() const {
    return value_;
  }
  operator Type() const {
    return value_;
  }
  const Type& asType() const {
    return value_;
  }
  bool is_initialized() const {
    return value_state_;
  }
 private:
  Type value_;
  bool value_state_;
};
}
#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_OPTIONAL_H_
