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

#ifndef TYPE_H_
#define TYPE_H_


namespace codegen {

class Boolean;
class Integer;
class Float;
class String;
class Enum;
class Array;
class Map;
class Struct;
class NullableType;
class Typedef;

class ConstantsCreator;

/*
 * An interface for Type visitor that is used to generate code
 * for one of the types (built-in or composite)
 * An object implementing TypeCodeGenerator should be applied to
 * the abstract Type, which in turn calls one of the methods below
 * methods depending on it's exact type.
 */
class TypeCodeGenerator {
 public:
  // Called to generate code for exact type
  virtual void GenerateCodeForBoolean(const Boolean* boolean);
  virtual void GenerateCodeForInteger(const Integer* integer);
  virtual void GenerateCodeForFloat(const Float* flt);
  virtual void GenerateCodeForString(const String* string);
  virtual void GenerateCodeForEnum(const Enum* enm);
  virtual void GenerateCodeForArray(const Array* array);
  virtual void GenerateCodeForMap(const Map* map);
  virtual void GenerateCodeForNullable(const NullableType* nullable);
  virtual void GenerateCodeForStruct(const Struct* strct);
  virtual void GenerateCodeForTypedef(const Typedef* tdef);
  virtual ~TypeCodeGenerator();
};

/*
 * An interface for all the types that can be defined by XML interface specification.
 */
class Type {
 public:
  // Accepts |code_generator| visitor calling one of it's methods
  // (depending on exact type) with this object
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const = 0;
  // Get factory that creates constants from given string literals
  // Returns NULL if type values can not be represented with string literals
  virtual const ConstantsCreator* SupportsConstants() const = 0;
  virtual ~Type();
};


}  // namespace codegen

#endif /* TYPE_H_ */
