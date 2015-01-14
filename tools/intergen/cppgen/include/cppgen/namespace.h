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

#ifndef NAMESPACE_H_
#define NAMESPACE_H_

#include <map>
#include <set>
#include <sstream>
#include <string>

namespace codegen {

/*
 * Represents single C++ namespace where all the declarations and definitions live.
 * Automatically generates all the namespace scope declarations,
 * type forward declarations and 'using' type import directives.
 */
class Namespace {
 public:
  // Types
  class ForwardDeclaration;
  class ImportedName;
 public:
  // Methods
  // Constructs unnamed (anonymous) namespace
  Namespace();
  // Constructs named namespace
  explicit Namespace(const std::string& name);
  Namespace(const Namespace& that);
  ~Namespace();
  // Creates (if absent) nested namespace with |name|
  // and provides access to it.
  Namespace& nested(const std::string& name);
  // Namespace ostream, used to write actual code into namespace
  std::ostream& os();
  // Add type described with |type_name| forward declaration
  // to the top of namespace block
  void ForwardDeclare(const ForwardDeclaration& type_name);
  // Add import directive for thing described with |name| to the top
  // of the current namespace block.
  void ImportName(const ImportedName& name);
  // Write this namespace and all it's nested namespaces to the given ostream.
  void Write(std::ostream* os);
  // Name (relative) of this namespace
  const std::string& name() const;

 private:
  // Methods
  // Tells whether this namespace or any of it's nested namspaces
  // has non-empty list of forward declarations or using directives
  bool HasForwardDeclarations();
  // Tells whether this namespace or any of it's nested namspaces
  // has any contents
  bool HasContents();
  // Output nested namespaces's list of forward declarations and
  // 'using' import directives followed by this namespace's list of
  // forward declarations and import directives
  void WriteForwardDeclarations(std::ostream* os);
  // Writes all nested namespaces' contents followed by this namespace contents
  void WriteContents(std::ostream* os);
  // Writes namespace begin declaratoin
  void BeginNamespace(std::ostream* os);
  // Writes namespace end declaration
  void EndNamespace(std::ostream* os);
 private:
  // Types
  bool global_;
  std::string name_;
  std::set<ImportedName> imported_names_;
  std::set<ForwardDeclaration> forward_declarations_;
  std::map<std::string, Namespace> nested_;
  std::stringstream contents_;
};

/*
 * Represents namespace forward declaration item
 */
class Namespace::ForwardDeclaration {
 public:
   // Only classes and structs can be forward-declared
  enum Type {
    kClass,
    kStruct
  };
 public:
  // Constructs forward declaration item of |type| and |name|
  ForwardDeclaration(Type type, const std::string& name);
  // Forward declarations are sorted in alphabetical order of their names
  bool operator<(const ForwardDeclaration& that) const;
  const std::string& name() const;
  Type type() const;

 private:
  Type type_;
  std::string name_;

};

/*
 * Represents single namespace import item
 */
class Namespace::ImportedName {
 public:
  // Imports a |name| into current namespace. |name| must specify fully
  // qualified name including namespace name from which it is imported.
  // If |single_name| is false, the whole namespace is imported
  ImportedName(std::string name, bool single_name);
  // Imports are sorted by their type (single name or whole namespace)
  // then by name alphabetically
  bool operator<(const ImportedName& that) const;
  const std::string& name() const;
  bool is_single_name() const;

 private:
  bool single_name_;
  std::string name_;
};


}  // namespace codegen

#endif /* NAMESPACE_H_ */
