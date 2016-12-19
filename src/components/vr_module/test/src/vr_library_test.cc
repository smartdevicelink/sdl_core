/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <dlfcn.h>

#include "gtest/gtest.h"
#include "vr_module/vr_module.h"

using functional_modules::PluginInfo;

namespace vr_module {

::testing::AssertionResult IsError(void* error) {
  if (error) {
    return ::testing::AssertionSuccess() << static_cast<const char*>(error);
  } else {
    return ::testing::AssertionFailure() << error;
  }
}

TEST(VRModuleLibraryTest, Load) {
  const std::string kLibraryPath = "libvrmoduleplugin.so";

  void* handle = dlopen(kLibraryPath.c_str(), RTLD_LAZY);
  EXPECT_FALSE(IsError(dlerror()));
  ASSERT_TRUE(handle != NULL);

  const std::string kSymbol = "Create";
  void* symbol = dlsym(handle, kSymbol.c_str());
  EXPECT_FALSE(IsError(dlerror()));
  ASSERT_TRUE(symbol != NULL);

  typedef VRModule* (*Create)();
  Create create_manager = reinterpret_cast<Create>(symbol);
  VRModule* module = create_manager();
  ASSERT_TRUE(module != NULL);

  PluginInfo plugin = module->GetPluginInfo();
  EXPECT_EQ(plugin.name, "VRModulePlugin");
  EXPECT_EQ(plugin.version, 1);

  delete module;
  int ret = dlclose(handle);
  EXPECT_FALSE(ret);
  EXPECT_FALSE(IsError(dlerror()));
}

}  // namespace vr_module
