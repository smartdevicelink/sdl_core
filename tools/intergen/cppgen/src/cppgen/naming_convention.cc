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

#include "cppgen/naming_convention.h"

#include <algorithm>
#include <cctype>
#include <memory>
#include <set>

#include "model/interface.h"
#include "utils/string_utils.h"

using std::string;
using std::vector;

namespace codegen {

namespace {

bool IsAllUpper(const string& id) {
  for (string::const_iterator i = id.begin(); i != id.end(); ++i) {
    if (std::isalpha(*i) && std::islower(*i)) {
      return false;
    }
  }
  return true;
}

bool IsAllLower(const string& id) {
  for (string::const_iterator i = id.begin(); i != id.end(); ++i) {
    if (std::isalpha(*i) && std::isupper(*i)) {
      return false;
    }
  }
  return true;
}

}

WordList::WordList() {
}

WordList::~WordList() {
}

WordList WordList::FromUnknown(const string& identifier) {
  if (IsAllUpper(identifier)) {
    return FromUnderscoreSeparated(identifier);
  } else if (IsAllLower(identifier)) {
    return FromUnderscoreSeparated(identifier);
  } else {
    return FromCamelCase(identifier);
  }
}

WordList WordList::FromUnderscoreSeparated(const string& id) {
  WordList res;
  string::const_iterator word_begin = id.begin();
  bool know_word_begin = true;
  for (string::const_iterator i = id.begin() + 1; i != id.end(); ++i) {
    if (std::isalnum(*i)) {
      if (!know_word_begin) {
        word_begin = i;
        know_word_begin = true;
      } else {
      }
    } else {  // not isalnum
      if (know_word_begin) {
        res.words_.push_back(string(word_begin, i));
        know_word_begin = false;
      } else {
      }
    }
  }
  res.words_.push_back(string(word_begin, id.end()));
  res.Normalize();
  return res;
}

WordList WordList::FromCamelCase(const string& id) {
  WordList res;
  string::const_iterator word_begin = id.begin();
  bool last_upper = true;
  bool multiple_upper = false;
  for (string::const_iterator i = id.begin() + 1; i != id.end(); ++i) {
    if (std::isupper(*i)) {
      if (last_upper) {
        multiple_upper = true;
      } else {
        res.words_.push_back(string(word_begin, i));
        word_begin = i;
        last_upper = true;
        multiple_upper = false;
      }
    } else { // not isupper
      if (multiple_upper) {
        res.words_.push_back(string(word_begin, i - 1));
        word_begin = i - 1;
        last_upper = false;
        multiple_upper = false;
      } else {
        last_upper = false;
      }
    }
  }
  res.words_.push_back(string(word_begin, id.end()));
  res.Normalize();
  return res;
}

WordList& WordList::operator +=(const WordList& that) {
  words_.insert(words_.end(), that.words_.begin(), that.words_.end());
  return *this;
}

string WordList::ToUpperCase() const {
  string res;
  if (!words_.empty())
    res = to_upper(words_.front());
  for (vector<string>::const_iterator i = words_.begin() + 1; i != words_.end();
      ++i) {
    res += "_" + to_upper(*i);
  }
  return res;
}

string WordList::ToCamelCase() const {
  string res;
  if (!words_.empty())
    res = words_.front();
  for (vector<string>::const_iterator i = words_.begin() + 1; i != words_.end();
      ++i) {
    string word = *i;
    word[0] = std::toupper(word[0]);
    res += word;
  }
  return res;
}

string WordList::ToLowerCase() const {
  string res;
  if (!words_.empty())
    res = words_.front();
  for (vector<string>::const_iterator i = words_.begin() + 1; i != words_.end();
      ++i) {
    res += "_" + *i;
  }
  return res;
}

string WordList::ToUpperCamelCase() const {
  string res;
  for (vector<string>::const_iterator i = words_.begin(); i != words_.end();
      ++i) {
    string word = *i;
    word[0] = std::toupper(word[0]);
    res += word;
  }
  return res;
}

void WordList::Normalize() {
  for (vector<string>::iterator i = words_.begin(); i != words_.end(); ++i) {
    *i = to_lower(*i);
    trim(*i);
  }
}

string WordList::Abbreviate() const {
  string res;
  for (vector<string>::const_iterator i = words_.begin(); i != words_.end(); ++i) {
    res += (*i)[0];
  }
  return res;
}

std::string LowercaseIntefaceName(const Interface& interface) {
  return WordList::FromUnknown(interface.name()).ToLowerCase();
}

std::string UpperCamelCaseInterfaceName(const Interface& interface) {
  return WordList::FromUnknown(interface.name()).ToUpperCamelCase();
}

std::string InterfaceNamespaceName(const Interface& interface) {
  return LowercaseIntefaceName(interface);
}

std::string Capitalize(const std::string& str) {
  string res = str;
  res[0] = std::toupper(res[0]);
  return res;
}

std::string AvoidKeywords(const std::string& name) {
  static const char* keywords_init[] = {
    "asm",
    "bool",
    "catch",
    "class",
    "const_cast",
    "default",
    "delete",
    "dynamic_cast",
    "explicit",
    "false",
    "final",
    "friend",
    "inline",
    "mutable",
    "namespace",
    "new",
    "operator",
    "override",
    "private",
    "protected",
    "public",
    "reinterpret_cast",
    "static_cast",
    "template",
    "this",
    "throw",
    "true",
    "try",
    "typeid",
    "typename",
    "using",
    "virtual",
    // std map functions to avoid collisions in frankenstruct
    "at",
    "begin",
    "cbegin",
    "cend",
    "clear",
    "count",
    "crbegin",
    "crend",
    "emplace",
    "emplace_hint",
    "empty",
    "end",
    "equal_range",
    "erase",
    "find",
    "get_allocator",
    "insert",
    "key_comp",
    "lower_bound",
    "max_size",
    "rbegin",
    "rend",
    "size",
    "swap",
    "upper_bound",
    "value_comp",
  };
  static const std::set<std::string> keywords(
      keywords_init,
      keywords_init + sizeof(keywords_init) / sizeof(keywords_init[0]));
  if (keywords.count(name) != 0) {
    return name + "_";
  } else {
    return name;
  }
}

}  // namespace codegen

