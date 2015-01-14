/* Copyright (c) 2014, Ford Motor Company
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

#include "cppgen/cpp_class.h"

#include <algorithm>

#include "cppgen/cpp_function.h"
#include "utils/safeformat.h"
#include "utils/string_utils.h"

using std::endl;
using std::ostream;
using std::string;
using typesafe_format::strmfmt;


namespace codegen {

namespace {

// Produces C++ literals for given access specifier
const char* AccessSpecName(CppClass::AccessSpec access) {
  switch (access) {
    case CppClass::kPublic:
      return "public";
    case CppClass::kPrivate:
      return "private";
    case CppClass::kProtected:
      return "protected";
    default:
      assert(!"Invalid access specifier");
      return "";
  }
}

/*
 * Helper class that automatically declares section inside
 * C++ class declaration and manages indentation
 */
class Section {
  struct SectionDeclarer {
    SectionDeclarer(const char* name, ostream* os) {
      *os << " " << name << ":" << endl;
    }
  };
 public:
  Section(const char* name, ostream* os)
      : declarer_(name, os),
        indent_(*os) {
  }
 private:
  SectionDeclarer declarer_;
  Indent indent_;
};

void DeclareMethods(const CppClass::MethodsList& method_list,
                    ostream* os, bool in_class) {
  for (CppClass::MethodsList::const_iterator i = method_list.begin(),
       end = method_list.end(); i != end; ++i) {
    const CppClass::Method* method = *i;
    method->Declare(os, in_class);
  }
}

void DefineMethods(const CppClass::MethodsList& method_list,
                    ostream* os) {
  for (CppClass::MethodsList::const_iterator i = method_list.begin(),
       end = method_list.end(); i != end; ++i) {
    const CppClass::Method* method = *i;
    method->Define(os, false);
  }
}

}  // namespace

CppClass::CppClass(const string& name)
    : name_(name) {
}

CppClass::~CppClass() {
}

void CppClass::Add(const CppClass::Superclass& superclass) {
  superclasses_.push_back(superclass);
}

void CppClass::Declare(ostream* os) {
  strmfmt(*os, "class {0}{1} ",
          name_, superclasses_.empty() ? "" : ":");
  for (SuperclassList::const_iterator i = superclasses_.begin(),
       end = superclasses_.end(); i != end; ++i) {
    strmfmt(*os, "{0} {1}",
            AccessSpecName(i->inheritance_type()),
            i->name());
  }
  *os << " {\n";
  MethodsList public_methods = functions(kPublic);
  if (!public_methods.empty()) {
    Section pub("public", os);
    DeclareMethods(public_methods, os, true);
  }
  MethodsList protected_methods = functions(kProtected);
  if (!protected_methods.empty()) {
    Section prot("protected", os);
    DeclareMethods(protected_methods, os, true);
  }
  MethodsList private_methods = functions(kPrivate);
  if (!private_methods.empty()) {
    Section priv("private", os);
    DeclareMethods(private_methods, os, true);
  }
  *os << "};\n";
}

void CppClass::Define(std::ostream* os) {
  DefineMethods(functions(kPublic), os);
  DefineMethods(functions(kProtected), os);
  DefineMethods(functions(kPrivate), os);
}

std::string CppClass::name() const {
  return name_;
}

CppClass::MethodsList CppClass::functions(
    CppClass::AccessSpec access_spec) {
  std::vector<const CppClass::Method*> specific_methods;
  const std::vector<const CppClass::Method*>& all_methods = methods();
  for (MethodsList::const_iterator i = all_methods.begin(),
       end = all_methods.end(); i != end; ++i) {
    const Method* method = *i;
    if (method->access_specifier() == access_spec) {
      specific_methods.push_back(method);
    }
  }
  return specific_methods;
}

CppClass::Method::Method(const CppClass* cls, AccessSpec access,
                         const std::string& name,
                         const std::string& return_type_name,
                         int qualifiers)
    : CppFunction(cls->name(), name, return_type_name, qualifiers),
      access_specifier_(access){
}

CppClass::AccessSpec CppClass::Method::access_specifier() const {
  return access_specifier_;
}

CppClass::Superclass::Superclass(const std::string& name,
                                 AccessSpec inheritance_type)
    : name_(name),
      inheritance_type_(inheritance_type) {

}
CppClass::AccessSpec CppClass::Superclass::inheritance_type() const {
  return inheritance_type_;
}

const std::string& CppClass::Superclass::name() const {
  return name_;
}

} // namespace codegen
