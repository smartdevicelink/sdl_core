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

#ifndef FUNCTION_ID_METHOD_H_
#define FUNCTION_ID_METHOD_H_

#include "cppgen/cpp_function.h"

namespace codegen {
class FunctionMessage;

/*
 * Generates function_id() virtual method that is to be declared/defined
 * in every API message struct.
 */
class FunctionIdMethod : public CppFunction {
 public:
  FunctionIdMethod(const FunctionMessage* func);
  ~FunctionIdMethod();
 private:
  // Methods
  // CppFunction interface
  virtual void DefineBody(std::ostream* os) const;
 private:
  // Fields
  const FunctionMessage* func_;
};

/*
 * Generates function_string_id() virtual method that is to be declared/defined
 * in every API message struct.
 */
class FunctionStringIdMethod : public CppFunction {
 public:
  FunctionStringIdMethod(const FunctionMessage* func);
  ~FunctionStringIdMethod();
 private:
  // Methods
  // CppFunction interface
  virtual void DefineBody(std::ostream* os) const;
 private:
  // Fields
  const FunctionMessage* func_;
};

}  // namespace codegen

#endif /* FUNCTION_ID_METHOD_H_ */
