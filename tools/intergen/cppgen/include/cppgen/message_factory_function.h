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

#ifndef CODEGEN_MESSAGE_FACTORY_FUNCTION_H
#define CODEGEN_MESSAGE_FACTORY_FUNCTION_H

#include "cppgen/cpp_function.h"
#include "model/function.h"

namespace codegen {
class Interface;

/*
 * This class is used to produce interface-specific,
 * Message-type specific message factory function that is
 * to be used to produce message from Json::Value object
 */
class MessageFactoryFunction: public CppFunction {
 public:
  enum SerializationType {
    kJson
  };

  // Methods
  // Creates function code generation object that produces
  // declaration and definition of a factory for messages of type
  // |factory_type| that belong to given |interface|
  MessageFactoryFunction(const Interface* interface,
                         SerializationType serialization_type,
                         FunctionMessage::MessageType factory_type);
 private:
  // Types
  typedef std::vector<const FunctionMessage*> MessageList;
 private:
  // Methods
  virtual void DefineBody(std::ostream* os) const;
  void DefineCases(std::ostream* os,
                   const MessageList& functions) const;
 private:
  // Fields
  const Interface* interface_;
  FunctionMessage::MessageType factory_type_;
};

} // namespace codegen

#endif // CODEGEN_MESSAGE_FACTORY_FUNCTION_H
