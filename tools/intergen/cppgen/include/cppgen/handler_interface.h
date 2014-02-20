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

#ifndef HANDLER_INTERFACE_H
#define HANDLER_INTERFACE_H

#include "cppgen/cpp_class.h"

#include "model/function.h"
#include "utils/stl_utils.h"

namespace codegen {
class CppFile;
class Namespace;

/*
 * Class is used to declare message handler interfaces for specific messages of
 * given type for given interface
 */
class HandlerInterface: public CppClass {
public:
  // Methods
  // Creates HandlerInterface object that is used to produce declaration of
  // Handler interface. This interface defines handler methods for messages of
  // given |type| defined in given rpc |interface|.
  // Interface declaration requires message types to be forward-declared, these
  // declarations are placed into given |header_file|.
  HandlerInterface(FunctionMessage::MessageType type,
                   const Interface* interface,
                   CppFile* header_file);
protected:
  // Methods
  // CppClass interface
  virtual const MethodsList& methods();

private:
  // Types
  typedef std::vector<const FunctionMessage*> FunctionMessages;
private:
  // Methods
  // Collects all the messages that are to be handled and defines
  // Handler methods for them. Also declares virtual destructor.
  void CollectMethods();
  // Helper method that actually generates handler functions
  void AddFunctionMessageHandlers(const FunctionMessages& function_messages);
private:
  // Fields
  FunctionMessage::MessageType type_;
  const Interface* interface_;
  CppFile* header_file_;
  MethodsList methods_;
  utils::StdContainerDeleter<MethodsList> methods_deleter_;
};

}  // namespace codegen

#endif // HANDLER_INTERFACE_H
