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

#ifndef CPP_FILE_H_
#define CPP_FILE_H_

#include <iosfwd>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include "cppgen/namespace.h"
#include "model/function.h"
#include "utils/common_types.h"

namespace codegen {
class Type;

/*
 * Class representing C++ source file (generic) which contains C++ declarations
 * and definitions placed in namespaces
 * Can represent header or source file which basically differ in whether
 * file's contents is wrapped with include guards
 */
class CppFile {
 public:
  // Types
  class Header;
 public:
  // Methods
  // Creates C++ file with name |file_name| that is part of module
  // |module_name|.
  // If |write_guards| is true, file contents is wrapped with include guards
  // Module name is used to generate header guards and as a directory to
  // write file into.
  CppFile(const std::string& file_name, const std::string& module_name, bool write_guards);
  ~CppFile();
  const std::string& file_name() const;
  // Adds header to file's include list
  void Include(const Header& header);
  // Automatically adds header where given type is defined
  void IncludeType(const Type& type);
  // Returns predefined namespaces for entities of different type
  Namespace& global_namespace();
  Namespace& types_ns();
  Namespace& requests_ns();
  Namespace& responses_ns();
  Namespace& notifications_ns();
  Namespace& NamespaceByMessageType(FunctionMessage::MessageType type);

  // Generates code file named module_name/file_name and saves all namespaces
  // into it.
  void Write(std::ostream* os);

 private:
  Namespace& module_namespace();
  // Methods
 private:
  bool write_guards_;
  std::string file_name_;
  std::string module_name_;
  // Fields
  std::stringstream contents_;
  std::set<Header> headers_;
  Namespace global_namespace_;
};

/*
 *  Class representing single header that is to be included into current one
 */
class CppFile::Header {
 public:
  // Methods

  // Creates include directive with |name|.
  // If |local| is true, name will be included in quotes, else
  // it is included in <>
  Header(const std::string& name, bool local);
  bool is_local() const;
  const std::string& name() const;
  bool operator<(const Header& that) const;

 private:
  // Fields
  std::string name_;
  bool local_;
};

}  // namespace codegen

#endif /* CPP_FILE_H_ */
