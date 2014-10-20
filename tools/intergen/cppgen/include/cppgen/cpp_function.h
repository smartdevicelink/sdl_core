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

#ifndef CPP_FUNCTION_H_
#define CPP_FUNCTION_H_

#include <iosfwd>
#include <string>
#include <vector>

namespace codegen {

/*
 * Class representing single generated C++ function or method
 * Has methods to output that function declaration or definition to
 * ostream
 */
class CppFunction {
 public:
  // Types and constants
  struct Parameter;
  struct OptionalParameter;
  struct Initializer;
  // C++ Function qualifiers
  static const int kVirtual  = 1 << 0;
  static const int kStatic   = 1 << 1;
  static const int kExplicit = 1 << 2;
  static const int kConst    = 1 << 3;
  static const int kVolatile = 1 << 4;
  static const int kAbstract = 1 << 5;
 public:
  // Methods
  // Creates a C++ function that belongs to |class_name|
  // (should be empty for free functions), has a |name| and returns
  // value of |return_type_name|. Might have combination of optional
  // |qualifiers| (kVirtual | kConst, etc.)
  CppFunction(const std::string& class_name, const std::string& name,
              const std::string& return_type_name, int qualifiers = 0);
  virtual ~CppFunction();
  // Adds a (mandatory) parameter to a function, parameters are declared in the
  // same order they were added
  void Add(const Parameter& parameter);
  // Adds an optional parameter. Optional parameters are declared
  // after mandatory parameters in the order they were added
  void Add(const OptionalParameter& parameter);
  // Adds initializer statement, used by constructors
  void Add(const Initializer& initializer);
  // Outputs function declaration to ostream |os|, if
  // |in_class| is true, declaration will have all the
  // qualifiers present and class name omitted
  // following C++ rules of method declaration
  void Declare(std::ostream* os, bool in_class) const;
  // Outputs function definition (signature and body) to  ostream |os|.
  // if |in_class| is true, declaration will have all the
  // qualifiers present and class name omitted
  // following C++ rules of method declaration
  void Define(std::ostream* os, bool in_class) const;
  // Returns true if generated function has at least one parameter that is not
  // optional e.g. can not be called without parameters.
  bool has_mandatory_parameters() const;
 private:
  // Outputs function prototype to |os|.
  // if |in_class| is true, declaration will have all the
  // qualifiers present and class name omitted
  // following C++ rules of method declaration
  // If |default_values| is true, output values for optional
  // parameters (these should be written when function is declared).
  void WriteFunctionPrototype(std::ostream* os, bool in_class,
                              bool default_values) const;
  // Outputs constructor initializer list
  void WriteInitializerList(std::ostream* os) const;
 protected:
  // A method to be defined by derived classes that outputs function code
  virtual void DefineBody(std::ostream* os) const;
  int qualifiers_;
  std::string class_name_;
  std::string name_;
  std::string return_type_name_;
  std::vector<Parameter> parameters_;
  std::vector<OptionalParameter> optional_params_;
  std::vector<Initializer> initializers_;
};

/*
 * Represents single function parameter
 */
struct CppFunction::Parameter {
  // Every parameter has name and type, which are passed as
  // |name| and |type_name|
  Parameter(const std::string& name, const std::string& type_name);
  ~Parameter();
  std::string name;
  std::string type_name;
};

/*
 * Represents single optional parameter function can accept
 * it is different from usual parameter as it has default value
 */
struct CppFunction::OptionalParameter : CppFunction::Parameter {
  // Every optional parameter has name and type, which are passed as
  // |name| and |type_name|. Default initializer expression is passed as
  // |default_value| parameter.
  OptionalParameter(const std::string& name, const std::string& type_name,
                    const std::string& default_value);
  ~OptionalParameter();
  std::string default_value;
};

/*
 * Represents initializer entry for constructor initializer list.
 * Every entry consists of |field_name| to be initialized and it's
 * |initializer| expression.
 */
struct CppFunction::Initializer {
  Initializer(const std::string& field_name, const std::string& initializer);
  ~Initializer();
  std::string field_name;
  std::string initializer;
};

/*
 * Constructor is a specialized C++ function that only needs type_name
 * to be defined. Parameters (if any) can be added later.
 */
class CppStructConstructor: public CppFunction {
 public:
  // Generates a constructor for type named |type_name|.
  CppStructConstructor(const std::string& type_name);
  ~CppStructConstructor();
};

/*
 * Destructor is a specialized C++ function that only needs type_name
 * to be defined.
 */
class CppStructDestructor: public CppFunction {
 public:
  CppStructDestructor(const std::string& type_name, bool abstract = false);
  ~CppStructDestructor();
};

}  // namespace codegen

#endif /* CPP_FUNCTION_H_ */
