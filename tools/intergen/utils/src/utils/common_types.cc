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

#include "utils/common_types.h"

#include <cmath>

namespace codegen {

using std::string;

Fraction::Fraction()
    : numer_(0),
      denumer_(1) {
}

Fraction::Fraction(int64_t numer, int64_t denumer)
: numer_(numer), denumer_(denumer) {
}

// static
bool Fraction::FromFloatPointString(const std::string& literal, Fraction* fraction) {
  size_t dot_position = literal.find('.');
  string wholes_str = literal.substr(0, dot_position);
  string decs_str;
  if (dot_position != literal.npos) {
    decs_str = literal.substr(dot_position + 1);
  }
  int64_t numer_val = 0;
  if (StringToNumber(wholes_str + decs_str, &numer_val)) {
    int64_t denumer_val = pow(10, decs_str.size());
    *fraction = Fraction(numer_val, denumer_val);
    return true;
  }
  return false;
}

int64_t Fraction::numer() const {
  return numer_;
}

int64_t Fraction::denumer() const {
  return denumer_;
}

}  // namespace codegen


