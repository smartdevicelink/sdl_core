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

#ifndef CPPGEN_STRUCT_TYPE_DBUS_SERIALIZER_H
#define CPPGEN_STRUCT_TYPE_DBUS_SERIALIZER_H

#include "cppgen/cpp_function.h"

namespace codegen {
class Interface;
class Struct;
class TypePreferences;

class StructTypeDbusMessageSignatureMethod: public CppFunction {
 public:
  StructTypeDbusMessageSignatureMethod(const TypePreferences* preferences,
                                       const Struct* strct,
                                       bool substructure);
  ~StructTypeDbusMessageSignatureMethod();
 private:
  // CppFunction interface
  void DefineBody(std::ostream* os) const;
 private:
  const TypePreferences* preferences_;
  bool substructure_;
  const Struct* strct_;
};

class StructTypeFromDbusReaderConstructor : public CppStructConstructor {
 public:
  StructTypeFromDbusReaderConstructor(const TypePreferences* preferences,
                                      const Struct* strct,
                                      bool substructure,
                                      const std::string& base_class_name);
  ~StructTypeFromDbusReaderConstructor();
 private:
  // CppFunction interface
  void DefineBody(std::ostream* os) const;
 private:
  const TypePreferences* preferences_;
  bool substructure_;
  const Struct* strct_;
};

class StructTypeToDbusWriterMethod : public CppFunction {
 public:
  StructTypeToDbusWriterMethod(const Struct* strct,
                               bool substructure);
  ~StructTypeToDbusWriterMethod();
 private:
  // CppFunction interface
  void DefineBody(std::ostream* os) const;
 private:
  bool substructure_;
  const Struct* strct_;
};

} // namespace codegen

#endif // CPPGEN_STRUCT_TYPE_DBUS_SERIALIZER_H
