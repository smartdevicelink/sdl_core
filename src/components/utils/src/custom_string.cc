/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "utils/custom_string.h"
#include <cwchar>
#include <cstdlib>
#include <clocale>
#include <cwctype>
#include <new>
#include "utils/logger.h"

namespace utils {
namespace custom_string {

CREATE_LOGGERPTR_GLOBAL(logger_, "CustomString")

CustomString::CustomString() : size_(0), is_ascii_string_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
}

CustomString::CustomString(const std::string& str)
    : mb_string_(str), size_(0), is_ascii_string_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
  InitData();
}

CustomString::CustomString(const char* str)
    : mb_string_(str), size_(0), is_ascii_string_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
  InitData();
}

CustomString::CustomString(size_t n, char c)
    : mb_string_(n, c), size_(0), is_ascii_string_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
  InitData();
}

size_t CustomString::size() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return size_;
}

size_t CustomString::length() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return size_;
}

size_t CustomString::length_bytes() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_.size();
}

bool CustomString::is_ascii_string() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return is_ascii_string_;
}

bool CustomString::empty() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return 0 == size_;
}

bool CustomString::operator==(const CustomString& str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_ == str.mb_string_;
}

bool CustomString::operator==(const std::string& str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_ == str;
}

CustomString CustomString::operator+(const CustomString& str) const {
  if (!str.empty()) {
    CustomString result_str(mb_string_ + str.AsMBString());
    return result_str;
  }
  return *this;
}

CustomString CustomString::operator+(const std::string& str) const {
  if (!str.empty()) {
    CustomString result_str(mb_string_ + str);
    return result_str;
  }
  return *this;
}

char& CustomString::at(size_t pos) {
  return mb_string_.at(pos);
}

int CustomString::compare(const char* str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_.compare(str);
}

int CustomString::compare(const std::string& str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_.compare(str);
}

bool CustomString::CompareIgnoreCase(const CustomString& str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (is_ascii_string() && str.is_ascii_string()) {
    return !strcasecmp(c_str(), str.c_str());
  } else {
    std::wstring wstr_first(AsWString()), wstr_second(str.AsWString());
    ConvertWStringToLowerCase(wstr_first);
    ConvertWStringToLowerCase(wstr_second);
    return wstr_first == wstr_second;
  }
}

bool CustomString::CompareIgnoreCase(const char* str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  size_t amount_of_byte = strlen(str);
  size_t amount_of_symbols = CalculateLengthOfString(str);
  if (is_ascii_string() && (amount_of_byte == amount_of_symbols)) {
    return !strcasecmp(c_str(), str);
  } else {
    std::wstring wstr_first(AsWString()), wstr_second(AsWString(str));
    ConvertWStringToLowerCase(wstr_first);
    ConvertWStringToLowerCase(wstr_second);
    return wstr_first == wstr_second;
  }
}
const char* CustomString::c_str() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_.c_str();
}

std::wstring CustomString::AsWString() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return AsWString(mb_string_.c_str());
}

std::string CustomString::AsMBString() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return mb_string_;
}

std::wstring CustomString::AsWStringLowerCase() const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::wstring wstr(AsWString(mb_string_.c_str()));
  ConvertWStringToLowerCase(wstr);
  return wstr;
}

void CustomString::ConvertWStringToLowerCase(std::wstring& str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string current_locale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "");
  for (size_t i = 0; i < str.size(); ++i) {
    str[i] = towlower(str[i]);
  }
  setlocale(LC_ALL, current_locale.c_str());
}

std::wstring CustomString::AsWString(const char* str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::wstring wstr;
  size_t size = strlen(str);
  wchar_t* wchar_buff = new (std::nothrow) wchar_t[size + 1];
  if (NULL == wchar_buff) {
    return wstr;
  }
  wchar_buff[size] = L'\0';
  std::string current_locale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "");  // system locale
  mbstowcs(wchar_buff, str, size);
  setlocale(LC_ALL, current_locale.c_str());
  wstr = wchar_buff;
  delete[] wchar_buff;
  return wstr;
}

size_t CustomString::CalculateLengthOfString(const char* str) const {
  LOG4CXX_AUTO_TRACE(logger_);
  size_t lenght_of_string = 0;
  for (size_t i = 0; str[i] != '\0'; ++i) {
    if ((str[i] & kHigestByteOfUTF8Byte2) != kByteOfUTF8) {
      ++lenght_of_string;
    }
  }
  return lenght_of_string;
}

void CustomString::InitData() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (mb_string_.empty()) {
    return;
  }
  size_ = CalculateLengthOfString(mb_string_.c_str());
  is_ascii_string_ = size_ == mb_string_.size() ? true : false;
}

}  // namespace custom_string
}  // namespace utils
