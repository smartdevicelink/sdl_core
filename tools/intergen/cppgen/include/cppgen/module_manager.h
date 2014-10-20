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

#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_

#include <vector>

#include "cppgen/cpp_file.h"
#include "model/composite_type.h"
#include "model/function.h"

namespace codegen {
class Namespace;
class TypePreferences;

/*
 * Manages all the source files generated for given interface.
 * Every interface is a separate module.
 * Also manages namespaces used to store generated code.
 */
class ModuleManager {
 public:
  ModuleManager(const std::string& name, const TypePreferences& prefs);
  ~ModuleManager();

  /*
   * Methods that are used to find header file for entity that is being
   * generated
   */
  CppFile& HeaderForInterface();
  CppFile& HeaderForEnum(const Enum& enm);
  CppFile& HeaderForStruct(const Struct& strct);
  CppFile& HeaderForTypedef(const Typedef& tdef);
  CppFile& HeaderForFunction(const Function& function);
  CppFile& HeaderForResponse(const Response& request);
  CppFile& HeaderForNotification(const Notification& notification);

  /*
   * Methods that are used to find implementation file for entity that
   * is being generated
   */
  CppFile& SourceForInterface();
  CppFile& SourceForEnum(const Enum& enm);
  CppFile& SourceForStruct(const Struct& strct);
  CppFile& SourceForFunction(const Function& function);
  CppFile& SourceForResponse(const Response& request);
  CppFile& SourceForNotification(const Notification& notification);
  CppFile& SourceForValidator();

  /*
   * Creates and writes all the generated code to appropriate files
   */
  bool Write();
private:
  std::string module_name_;
  CppFile enums_header_;
  CppFile enums_source_;
  CppFile structs_header_;
  CppFile structs_source_;
  CppFile functions_header_;
  CppFile functions_source_;
  CppFile interface_header_;
  CppFile interface_source_;
  CppFile additional_validation_source_;
};

}  // namespace codegen

#endif /* MODULE_MANAGER_H_ */
