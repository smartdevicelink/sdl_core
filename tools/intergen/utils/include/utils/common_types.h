/**
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

#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#include <string>
#include <vector>

#include "utils/string_utils.h"

namespace codegen {
/*
 * Entity description, a set of text strings.
 */
typedef std::vector<std::string> Description;

/*
 * Natural fraction class
 */
class Fraction {
 public:
  // Constructs fraction with value of 0/1
  Fraction();
  // Constructs fraction from given values
  Fraction(int64_t numer, int64_t denumer);
  // Parses string |literal| containing float point string definition, result is stored in
  // |fraction| parameter.
  // If |literal| can not be parsed, returns false.
  static bool FromFloatPointString(const std::string& literal, Fraction* fraction);
  // Field accessors
  int64_t numer() const;
  int64_t denumer() const;

 private:
  int64_t numer_;
  int64_t denumer_;

};

/*
 * Template class representing generic range with lower
 * and upper bounds (min and max).
 * Allows value or another range to be tested for inclusion.
 * Range boundaries are always included in tests.
 */
template<typename T>
class BasicRange {
 public:
  typedef T value_type;
  // Create a range with specified bounds
  BasicRange(value_type min, value_type max)
      : min_(min),
        max_(max) {
    Fraction::FromFloatPointString(NumberToString(min_), &min_fract_);
    Fraction::FromFloatPointString(NumberToString(max_), &max_fract_);
  }
  // Constructs the range from given string representations of value.
  // Used to avoid converting fractional boundaries to floating point
  // Representation.
  BasicRange(const std::string& min_str, const std::string& max_str)
      : min_(0),
        max_(0) {
    bool valid_min = StringToNumber(min, &min_)
        && Fraction::FromFloatPointString(min, &min_fract_);
    bool valid_max = StringToNumber(max, &max_)
        && Fraction::FromFloatPointString(max, &max_fract_);
    assert(valid_min && valid_max);
  }
  // Tells if |value| belongs to current range (boundaries are included)
  bool Includes(value_type value) const {
    return min_ <= value && value <= max_;
  }

  // Tells if another range completely belongs to current range
  // (boundaries are included)
  bool Includes(const BasicRange& that) {
    return min() <= that.min() && max() >= that.max();
  }

  // Field access methods
  value_type min() const {
    return min_;
  }
  value_type max() const {
    return max_;
  }
  const Fraction& min_fract() const {
    return min_fract_;
  }
  const Fraction& max_fract() const {
    return max_fract_;
  }
 private:
  value_type min_;
  value_type max_;
  Fraction min_fract_;
  Fraction max_fract_;
};

} // namespace codegen

#endif /* COMMON_TYPES_H_ */
