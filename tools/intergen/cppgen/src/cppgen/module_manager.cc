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

#include "cppgen/module_manager.h"

#include <errno.h>
#include <fstream>

#include "cppgen/generator_preferences.h"

#ifdef OS_POSIX
#include <sys/stat.h>
#include <sys/types.h>

namespace {
bool CreateDirectory(const std::string& name) {
  int result = mkdir(name.c_str(), 0777) == 0;
  if (0 == result) {
    return true;
  } else if (EEXIST == result || EPERM == result) {
    struct stat s;
    result = stat(name.c_str(), &s);
    if (result != 0) {
      return false;
    }
    return (S_ISDIR(s.st_mode));
  }
  return false;
}

}  // namespace

#else
#error Please provide create directory function for your platform
#endif


namespace codegen {

ModuleManager::ModuleManager(const std::string& name,
                             const TypePreferences& prefs)
    : module_name_(name),
      enums_header_(module_name_ + "/enums.h", module_name_, true),
      enums_source_(module_name_ + "/enums.cc", module_name_, false),
      structs_header_(module_name_ + "/types.h", module_name_, true),
      structs_source_(module_name_ + "/types.cc", module_name_, false),
      functions_header_(module_name_ + "/functions.h", module_name_, true),
      functions_source_(module_name_ + "/functions.cc", module_name_, false),
      interface_header_(module_name_ + "/interface.h", module_name_, true),
      interface_source_(module_name_ + "/interface.cc", module_name_, false),
      additional_validation_source_(module_name_ + "/validation.cc", module_name_, false){
  structs_header_.Include(CppFile::Header("rpc_base/rpc_message.h", true));
  structs_header_.Include(CppFile::Header(enums_header_.file_name(), true));
  functions_header_.Include(CppFile::Header(enums_header_.file_name(), true));
  functions_header_.Include(CppFile::Header(structs_header_.file_name(), true));
  functions_header_.Include(CppFile::Header(interface_header_.file_name(), true));
  interface_header_.Include(CppFile::Header("rpc_base/rpc_message.h", true));
  interface_header_.Include(CppFile::Header(enums_header_.file_name(), true));
  enums_header_.Include(CppFile::Header("string", false));
  enums_source_.Include(CppFile::Header(enums_header_.file_name(), true));
  structs_source_.Include(CppFile::Header(structs_header_.file_name(), true));
  functions_source_.Include(CppFile::Header(functions_header_.file_name(),
                                            true));
  interface_source_.Include((CppFile::Header(interface_header_.file_name(),
                                             true)));
  interface_source_.Include(CppFile::Header(functions_header_.file_name(),
                                            true));

  additional_validation_source_.Include(
      CppFile::Header(structs_header_.file_name(), true));
  additional_validation_source_.Include(
      CppFile::Header(functions_header_.file_name(), true));
  if (prefs.generate_json) {
    structs_source_.Include(
        CppFile::Header("rpc_base/rpc_base_json_inl.h", true));
    functions_source_.Include(
        CppFile::Header("rpc_base/rpc_base_json_inl.h", true));
  }
}

ModuleManager::~ModuleManager() {
}

CppFile& ModuleManager::HeaderForInterface() {
  return interface_header_;
}

CppFile& ModuleManager::HeaderForEnum(const Enum& enm) {
  return enums_header_;
}

CppFile& ModuleManager::HeaderForStruct(const Struct& strct) {
  return structs_header_;
}

CppFile& ModuleManager::HeaderForTypedef(const Typedef& tdef) {
  return structs_header_;
}

CppFile& ModuleManager::HeaderForFunction(const Function& function) {
  return functions_header_;
}

CppFile& ModuleManager::HeaderForResponse(const Response& request) {
  return functions_header_;
}

CppFile& ModuleManager::HeaderForNotification(
    const Notification& notification) {
  return functions_header_;
}

CppFile& ModuleManager::SourceForInterface() {
  return interface_source_;
}

CppFile& ModuleManager::SourceForEnum(const Enum& enm) {
  return enums_source_;
}

CppFile& ModuleManager::SourceForStruct(const Struct& strct) {
  return structs_source_;
}

CppFile& ModuleManager::SourceForFunction(const Function& function) {
  return functions_source_;
}

CppFile& ModuleManager::SourceForResponse(const Response& request) {
  return functions_source_;
}

CppFile& ModuleManager::SourceForNotification(
    const Notification& notification) {
  return functions_source_;
}

CppFile& ModuleManager::SourceForValidator() {
  return additional_validation_source_;
}

bool ModuleManager::Write() {
  if (!CreateDirectory(module_name_)) {
    return false;
  }
  CppFile* files[] = { &enums_header_, &enums_source_,
                       &structs_header_, &structs_source_,
                       &functions_header_, &functions_source_,
                       &interface_header_, &interface_source_,
                       &additional_validation_source_ };
  for (size_t i = 0; i != sizeof(files)/sizeof(files[0]); ++i) {
    CppFile* file = files[i];
    std::ofstream stream(file->file_name().c_str());
    file->Write(&stream);
    if (!stream) {
      return false;
    }
  }
  return true;
}

}  // namespace codegen
