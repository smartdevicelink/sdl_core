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

#ifndef NAMING_CONVENTION_H_
#define NAMING_CONVENTION_H_

#include <string>
#include <vector>

namespace codegen {
class Interface;

/*
 * Helper class that constructs word list from a sentence or
 * single-word identifier written in UPPER_CASE, lower_case,
 * camelCase or UpperCamelCase and provides conversion between these
 * identifier naming conventions.
 */
class WordList {
 public:
  WordList();
  ~WordList();
  // Guess identifier naming convention and construct WordList from it
  static WordList FromUnknown(const std::string& identifier);
  // Split identifier as UPPER_CASE_ONE, also used to split
  // lower_case_identifiers
  static WordList FromUnderscoreSeparated(const std::string& identifier);
  // Split identifier as camelCase identifier or upperCamelCase
  static WordList FromCamelCase(const std::string& identifier);
  // Concatenates |that| word list to this one producing single word list
  WordList& operator+=(const WordList& that);
  // Converts word list to one of the supported identifier naming conventions
  std::string ToUpperCase() const;
  std::string ToCamelCase() const;
  std::string ToLowerCase() const;
  std::string ToUpperCamelCase() const;
  // Produces lower-case abbreviaton of a given WordList
  std::string Abbreviate() const;

 private:
  // Normalizes word list: all words are converted to lower case,
  // spaces are trimmed
  void Normalize();
  std::vector<std::string> words_;
};

// Capitalizes given string
std::string Capitalize(const std::string& str);

// Translate interface name to lower case
std::string LowercaseIntefaceName(const Interface& interface);

std::string UpperCamelCaseInterfaceName(const Interface& interface);

// Produces namespace name for |interface|.
// Currently it just lower_case_interface_name
std::string InterfaceNamespaceName(const Interface& interface);

// Produces new name (generally for struct fields) if it
// conflicts with known keywords
std::string AvoidKeywords(const std::string& name);

}  // namespace codegen

#endif /* NAMING_CONVENTION_H_ */
