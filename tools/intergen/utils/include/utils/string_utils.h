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

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <stdint.h>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>

/*
 * Commonly used string processing functions
 */

/*
 * Stream indentation helper class. Hooks the stream that is
 * passed to it's constructor processing all the input to that stream
 * and issuing indentation spaces when newline symbol is put to the stream.
 * Unhooks from stream on destruction.
 */
class Indent: private std::streambuf {
public:
  /*
   *  Constructs indentation 'sentry' object hooking on |dest| stream.
   *  |ident_size| is the indentation width (spaces).
   */
  explicit Indent(std::ostream& dest, size_t ident_size = 2)
    : dest_buf_(dest.rdbuf()),
      new_line_started_(true),
      ident_(ident_size, ' '),
      owner_(&dest) {
    owner_->rdbuf(this);
  }
  ~Indent() {
    owner_->rdbuf( dest_buf_ );
  }
private:
  virtual int overflow( int ch ) {
    if ( new_line_started_ && ch != '\n' ) {
          dest_buf_->sputn(ident_.data(), ident_.size());
    }
    new_line_started_ = ch == '\n';
    return dest_buf_->sputc(ch);
  }
private:
  std::streambuf*     dest_buf_;
  bool                new_line_started_;
  std::string         ident_;
  std::ostream*       owner_;
};

/*
 * String to numerical (int, double) conversion function.
 * Checks wether |str_val| contains a number and converts
 * it to |out_val|.
 * Returns false on failure.
 */
template<typename T>
bool StringToNumber(const std::string& str_val, T* out_val) {
  assert(out_val);
  std::stringstream stream(str_val);
  stream>>(*out_val);
  return bool(stream);
}

/*
 * Number to string conversion function.
 */
template<typename T>
inline std::string NumberToString(T number) {
  std::stringstream stream;
  std::string result;
  stream<<number;
  stream>>result;
  return result;
}

/*
 * In-place string trimming function, trims spaces at string beginning
 */
inline std::string& ltrim(std::string &s) {
  s.erase(
      s.begin(),
      std::find_if(s.begin(), s.end(),
                   std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

/*
 * In-place string trimming function, trims spaces at string end
 */
inline std::string& rtrim(std::string &s) {
  s.erase(
      std::find_if(s.rbegin(), s.rend(),
                   std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
      s.end());
  return s;
}

/*
 * In-place string trimming function, trims spaces around text
 */
inline std::string& trim(std::string &s) {
  return ltrim(rtrim(s));
}

/*
 * Converts ASCII string to lowercase
 */
inline std::string to_lower(std::string str) {
  for (std::string::iterator i = str.begin(); i != str.end(); ++i) {
    *i = std::tolower(*i);
  }
  return str;
}

/*
 * Converts ASCII string to uppercase
 */
inline std::string to_upper(std::string str) {
  for (std::string::iterator i = str.begin(); i != str.end(); ++i) {
    *i = std::toupper(*i);
  }
  return str;
}

#endif /* STRING_UTILS_H_ */
