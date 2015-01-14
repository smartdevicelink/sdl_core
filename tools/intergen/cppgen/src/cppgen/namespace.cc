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

#include "cppgen/namespace.h"

#include "cppgen/comment.h"
#include "utils/safeformat.h"

using std::endl;
using std::make_pair;
using std::map;
using std::set;
using std::string;
using typesafe_format::strmfmt;

namespace codegen {

Namespace::Namespace()
    : global_(true) {
}

Namespace::Namespace(const std::string& name)
    : global_(false),
      name_(name) {
}

Namespace::~Namespace() {
}

Namespace::Namespace(const Namespace& that)
    : global_(that.global_),
      name_(that.name_),
      nested_(that.nested_),
      contents_(that.contents_.str()) {
}

const std::string& Namespace::name() const {
  return name_;
}

Namespace& codegen::Namespace::nested(const std::string& name) {
  map<string, Namespace>::iterator res = nested_.find(name);
  if (res == nested_.end()) {
    return nested_.insert(make_pair(name, Namespace(name))).first->second;
  } else {
    return res->second;
  }
}

std::ostream& Namespace::os() {
  return contents_;
}

void Namespace::ForwardDeclare(const ForwardDeclaration& type_name) {
  forward_declarations_.insert(type_name);
}

void Namespace::Write(std::ostream* os) {
  WriteForwardDeclarations(os);
  *os << endl;
  WriteContents(os);
}

void Namespace::WriteForwardDeclarations(std::ostream* os) {
  if (HasForwardDeclarations()) {
    BeginNamespace(os);
    for (map<string, Namespace>::iterator i = nested_.begin(), end =
        nested_.end(); i != end; ++i) {
      i->second.WriteForwardDeclarations(os);
    }
    for (set<ImportedName>::iterator i = imported_names_.begin(), end =
        imported_names_.end(); i != end; ++i) {
      *os << (i->is_single_name() ? "using " : "using namespace ") << i->name()
          << ";" << endl;
    }
    for (set<ForwardDeclaration>::iterator i = forward_declarations_.begin(), end =
        forward_declarations_.end(); i != end; ++i) {
      const char* metatype = i->type() == ForwardDeclaration::kClass ? "class" : "struct";
      const string& type_name = i->name();
      strmfmt(*os, "{0} {1};", metatype, type_name) << '\n';
    }
    EndNamespace(os);
  }
}

void Namespace::WriteContents(std::ostream* os) {
  if (HasContents()) {
    BeginNamespace(os);
    for (map<string, Namespace>::iterator i = nested_.begin(), end =
        nested_.end(); i != end; ++i) {
      i->second.WriteContents(os);
    }
    *os << contents_.str();
    EndNamespace(os);
  }
}

bool Namespace::HasForwardDeclarations() {
  if (!forward_declarations_.empty() || !imported_names_.empty()) {
    return true;
  }
  for (map<string, Namespace>::iterator i = nested_.begin(), end =
      nested_.end(); i != end; ++i) {
    if (i->second.HasForwardDeclarations()) {
      return true;
    }
  }
  return false;
}

bool Namespace::HasContents() {
  if (contents_.rdbuf()->in_avail()) {
    return true;
  }
  for (map<string, Namespace>::iterator i = nested_.begin(), end =
      nested_.end(); i != end; ++i) {
    if (i->second.HasContents()) {
      return true;
    }
  }
  return false;
}

void Namespace::BeginNamespace(std::ostream* os) {
  if (!global_) {
    *os << "namespace";
    if (!name_.empty()) {
      *os << " " << name_;
    }
    *os << " {" << endl;
  }
}

void Namespace::ImportName(const ImportedName& name) {
  imported_names_.insert(name);
}

void Namespace::EndNamespace(std::ostream* os) {
  if (!global_) {
    *os << "}  " << Comment("namespace " + name_);
  }
  *os << endl;
}

Namespace::ForwardDeclaration::ForwardDeclaration(Type type,
                                                  const std::string& name)
    : type_(type),
      name_(name) {
}

bool Namespace::ForwardDeclaration::operator <(
    const ForwardDeclaration& that) const {
  return this->name_ < that.name_;
}

const std::string& Namespace::ForwardDeclaration::name() const {
  return name_;
}

Namespace::ForwardDeclaration::Type Namespace::ForwardDeclaration::type() const {
  return type_;
}

Namespace::ImportedName::ImportedName(std::string name, bool single_name)
    : single_name_(single_name),
      name_(name) {
}

bool Namespace::ImportedName::operator <(const ImportedName& that) const {
  if (single_name_ == that.single_name_)
    return int(single_name_) < int(that.single_name_);
  return name_ < that.name_;
}

const std::string& Namespace::ImportedName::name() const {
  return name_;
}

bool Namespace::ImportedName::is_single_name() const {
  return single_name_;
}

}  // namespace codegen

