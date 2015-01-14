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

#include "cppgen/comment.h"

#include <iostream>
#include <ostream>
#include <sstream>

#include "utils/string_utils.h"

using std::endl;
using std::string;
using std::stringstream;

namespace codegen {

Comment::Comment(const Description& description) {
  for (Description::const_iterator i = description.begin();
      i != description.end(); ++i) {
    stringstream str(*i);
    string line;
    while (std::getline(str, line)) {
      line = trim(line);
      if (!line.empty()) {
        description_.push_back(line);
      }
    }
  }
}

Comment::Comment(const std::string& text) {
  description_.push_back(text);
}

Comment::~Comment() {
}

}  // namespace codegen

std::ostream& codegen::operator <<(std::ostream& os, const Comment& comment) {
  if (!comment.description_.empty()) {
    if (comment.description_.size() == 1) {
      os << "// " << comment.description_.front();
    } else {
      os << "/* ";
      for (Description::const_iterator i = comment.description_.begin(), end =
          comment.description_.end(); i != end; ++i) {
        os << *i << " ";
      }
      os << "*/";
    }
  }
  return os;
}
