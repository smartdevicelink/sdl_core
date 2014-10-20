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

#ifndef CODEGEN_MESSAGE_INTERFACE_H
#define CODEGEN_MESSAGE_INTERFACE_H

#include "cppgen/cpp_class.h"
#include "model/function.h"

namespace codegen {
class Interface;
class MessageInterface;

/*
 * Helper class that is used to produce code for
 * interface() method for interface-specific.
 * message base class.
 */
class InterfaceStringIdMethod: public CppClass::Method {
 public:
  InterfaceStringIdMethod(
      const MessageInterface* message_interface,
      const Interface* interface);
 protected:
  // CppFunction interface
  void DefineBody(std::ostream* os) const;
 private:
  const Interface* interface_;
};

/*
 * Helper class that is used to produce declaration and
 * definition of interface-specific message base class
 * for messages of given type
 */
class MessageInterface: public CppClass {
 public:
  MessageInterface(const Interface* interface,
                   FunctionMessage::MessageType message_type);
 private:
  // Methods
  // CppClass interface
  virtual const MethodsList& methods();
 private:
  // Fields
  Method constructor_;
  Method handle_with_method_;
  InterfaceStringIdMethod interface_string_id_method_;
  std::vector<const Method*> methods_;
};

} // namespace codegen

#endif // CODEGEN_MESSAGE_INTERFACE_H
