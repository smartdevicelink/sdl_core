/*
 * Copyright (c) 2018, Ford Motor Company
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
 * POSSIBILITY OF SUCH DAMAGE. */

#include "gtest/gtest.h"
#include "rc_rpc_plugin/interior_data_cache_impl.h"

namespace rc_rpc_plugin_test {

class InteriorDataCacheTest : public ::testing::Test  {
};

TEST_F(InteriorDataCacheTest, InteriorDataCacheDoesNotContainRandomDataInitialy) {
    const rc_rpc_plugin::InteriorDataCacheImpl cache;
    std::string module_type_key = "random_module_type";
    EXPECT_FALSE(cache.Check(module_type_key));
    auto retrived_data = cache.Retrive(module_type_key);
    EXPECT_EQ(retrived_data.getType(), smart_objects::SmartType_Null);
}

TEST_F(InteriorDataCacheTest, CheckExstingSubscribed ) {
    rc_rpc_plugin::InteriorDataCacheImpl cache;
    const std::string module_type_key = "random_module_type";
    smart_objects::SmartObject data;
    data["key"] = "value";

    cache.Add(module_type_key, data);
    EXPECT_TRUE(cache.Check(module_type_key));
    auto retrived_data = cache.Retrive(module_type_key);
    EXPECT_EQ(retrived_data, data);
}

TEST_F(InteriorDataCacheTest, DataDoesNotExistAfterClear) {
    rc_rpc_plugin::InteriorDataCacheImpl cache;
    const std::string module_type_key = "random_module_type";
    smart_objects::SmartObject data;
    data["key"] = "value";

    cache.Add(module_type_key, data);
    EXPECT_TRUE(cache.Check(module_type_key));
    auto retrived_data = cache.Retrive(module_type_key);
    EXPECT_EQ(retrived_data, data);
    cache.Clear();
    auto retrived_data_after_clear = cache.Retrive(module_type_key);
    EXPECT_EQ(retrived_data_after_clear.getType(), smart_objects::SmartType_Null);
}

TEST_F(InteriorDataCacheTest, MultipleDataCached) {
    rc_rpc_plugin::InteriorDataCacheImpl cache;

    const std::string module_type_key1 = "random_module_type";
    smart_objects::SmartObject data1;
    data1["key"] = "value1";
    cache.Add(module_type_key1, data1);
    EXPECT_TRUE(cache.Check(module_type_key1));
    auto retrived_data1 = cache.Retrive(module_type_key1);
    EXPECT_EQ(retrived_data1, data1);

    std::string module_type_key2 = "random_module_type2";
    smart_objects::SmartObject data2;
    data2["key"] = "value2";
    cache.Add(module_type_key2, data2);
    EXPECT_TRUE(cache.Check(module_type_key2));
    auto retrived_data2 = cache.Retrive(module_type_key2);
    EXPECT_EQ(retrived_data2, data2);

    ASSERT_TRUE(data1 != data2);
    EXPECT_TRUE(retrived_data1 != data2);
    EXPECT_TRUE(retrived_data2 != data1);

}

}  // rc_rpc_plugin_test
