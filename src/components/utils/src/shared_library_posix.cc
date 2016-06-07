/*
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
#include <string>
#include <dlfcn.h>

#include "utils/shared_library.h"

namespace {

const std::string kLibPrefix = "lib";
const std::string kLibSuffix = ".so";

}  // namespace

utils::SharedLibrary::SharedLibrary() : handle_(NULL) {}

utils::SharedLibrary::SharedLibrary(const char* library_name) : handle_(NULL) {
  Load(library_name);
}

bool utils::SharedLibrary::Load(const char* library_name) {
  if (handle_) {
    return true;
  }
  const std::string platform_name = kLibPrefix + library_name + kLibSuffix;
  handle_ = dlopen(platform_name.c_str(), RTLD_LAZY);
  return IsLoaded();
}

void utils::SharedLibrary::Unload() {
  if (handle_) {
    dlclose(handle_);
    handle_ = NULL;
  }
}

bool utils::SharedLibrary::IsLoaded() const {
  return handle_ != NULL;
}

void* utils::SharedLibrary::GetSymbol(const char* name) {
  void* result = NULL;
  if (handle_) {
    result = dlsym(handle_, name);
  }
  return result;
}

bool utils::SharedLibrary::HasSymbol(const char* name) {
  return GetSymbol(name) != NULL;
}
