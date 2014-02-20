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

#ifndef CODEGEN_CPP_CLASS_H
#define CODEGEN_CPP_CLASS_H

#include <iosfwd>
#include <string>
#include <vector>

#include "cppgen/cpp_function.h"

namespace codegen {
/*
 * Class helper abstract class for C++ classes generation
 * Must be inherited by a concrete class that provides
 * class information by implementing pure virtual methods
 */
class CppClass {
 public:
  // Types
  class Method;
  class Superclass;
  typedef std::vector<const Method*> MethodsList;
  typedef std::vector<Superclass> SuperclassList;
  // Member and inheritance access types specifier
  enum AccessSpec {
    kPublic, kProtected, kPrivate
  };
 public:
  // Methods
  // Creates class named |name|
  CppClass(const std::string& name);
  ~CppClass();
  // Add superclass to classess parent's list
  void Add(const Superclass& superclass);
  // Output class declaration code to |os|
  void Declare(std::ostream* os);
  // Output class member's definition code to |os|
  void Define(std::ostream* os);
  // Class name
  std::string name() const;
protected:
  // Methods
  // To be defined by a concrete class, to provide
  // Class member methods list
  virtual const MethodsList& methods() = 0;
private:
  // Helper method that gives method list filtered by
  // |access_spec|
  MethodsList functions(AccessSpec access_spec);
  // Fields
  std::string name_;
  SuperclassList superclasses_;
};

/*
 * Helper class produces code for class method
 */
class CppClass::Method: public CppFunction {
 public:
  Method(const CppClass* cls, AccessSpec access, const std::string& name,
           const std::string& return_type_name, int qualifiers = 0);
  AccessSpec access_specifier() const;

private:
  AccessSpec access_specifier_;
};

/*
 * Helper class that is used to specify class's parrents
 */
class CppClass::Superclass {
public:
  Superclass(const std::string& name, AccessSpec inheritance_type);
  const std::string& name() const;
  AccessSpec inheritance_type() const;

private:
  std::string name_;
  AccessSpec inheritance_type_;
};

} // namespace codegen

#endif // CODEGEN_CPP_CLASS_H
