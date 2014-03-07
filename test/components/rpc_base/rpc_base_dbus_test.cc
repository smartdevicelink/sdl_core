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

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#include "rpc_base/rpc_base.h"
#include "rpc_base/rpc_base_dbus_inl.h"
#include "rpc_base/dbus_message.h"

namespace test {
using namespace rpc;

enum TestEnum {
  kValue0,
  kValue1,
  kInvalidValue
};

bool IsValidEnum(TestEnum val) {
  return val == kValue0 || val == kValue1;
}

bool EnumFromJsonString(const std::string& value, TestEnum* enm) {
  if (value == "kValue0") {
    *enm = kValue0;
    return true;
  } else if (value == "kValue1") {
    *enm = kValue1;
    return true;
  } else {
    return false;
  }
}

const char* EnumToJsonString(TestEnum enm) {
  switch(enm) {
    case kValue0: return "kValue0";
    case kValue1: return "kValue1";
    default: return "UNKNOWN";
  }
}

struct DbusDeserialization: public testing::Test {
  dbus::MessageRef msgref;
  DbusDeserialization()
    : msgref(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
  }
};

TEST_F(DbusDeserialization, DeserializeBool) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteBool(true);
  }
  {
    dbus::MessageReader reader(msgref);
    Boolean booln(&reader);
    ASSERT_TRUE(booln);
  }
}

TEST_F(DbusDeserialization, DeserializeByte) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteByte(200);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer<uint8_t, 1, 220> byte(&reader);
    ASSERT_TRUE(byte.is_initialized());
    ASSERT_TRUE(byte.is_valid());
    ASSERT_EQ(byte, 200);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeInt64) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteInt64(-1);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer<int64_t, -5, 220> int64(&reader);
    ASSERT_TRUE(int64.is_initialized());
    ASSERT_TRUE(int64.is_valid());
    ASSERT_EQ(int64, -1);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeFloat) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteDouble(3.14);
  }
  {
    dbus::MessageReader reader(msgref);
    Float<3, 4> pi(&reader);
    ASSERT_TRUE(pi.is_initialized());
    ASSERT_TRUE(pi.is_valid());
    ASSERT_DOUBLE_EQ(pi, 3.14);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeString) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteString("Hello");
  }
  {
    dbus::MessageReader reader(msgref);
    String<3, 10> hello(&reader);
    ASSERT_TRUE(hello.is_initialized());
    ASSERT_TRUE(hello.is_valid());
    ASSERT_EQ(std::string(hello), "Hello");
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeEnum) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteInt32(kValue1);
  }
  {
    dbus::MessageReader reader(msgref);
    Enum<TestEnum> enm(&reader);
    ASSERT_TRUE(enm.is_initialized());
    ASSERT_TRUE(enm.is_valid());
    ASSERT_EQ(enm, kValue1);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeArray) {
  {
    dbus::MessageWriter writer(msgref);
    std::string array_signature;
    rpc::DbusSignature<Integer<int32_t, 1, 50> >(&array_signature);
    dbus::MessageWriter array_writer(&writer,
                                     dbus::kArray,
                                     array_signature.c_str());
    array_writer.WriteInt32(5);
    array_writer.WriteInt32(33);
  }
  {
    dbus::MessageReader reader(msgref);
    Array< Integer<int32_t, 1, 50>, 1, 100 > array(&reader);
    ASSERT_TRUE(array.is_initialized());
    ASSERT_TRUE(array.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 33);

  }
}

TEST_F(DbusDeserialization, DeserializeArrayOfArrays) {
  {
    dbus::MessageWriter writer(msgref);
    std::string array_signature;
    rpc::DbusSignature<Array<Integer<int32_t, 1, 50>, 1, 5> >(&array_signature);
    dbus::MessageWriter array_writer(&writer,
                                     dbus::kArray,
                                     array_signature.c_str());
    int val = 5;
    for (int i = 0; i < 2; ++i) {
      std::string subarray_signature;
      rpc::DbusSignature<Integer<int32_t, 1, 50> >(&subarray_signature);
      dbus::MessageWriter subarray_wirter(&array_writer, dbus::kArray,
                                          subarray_signature.c_str());

      subarray_wirter.WriteInt32(val++);
      subarray_wirter.WriteInt32(val++);
    }
  }
  {
    dbus::MessageReader reader(msgref);
    Array<Array<Integer<int32_t, 1, 50>, 1, 5>, 1, 5>  array(&reader);
    ASSERT_TRUE(array.is_initialized());
    ASSERT_TRUE(array.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0].size(), 2);
    ASSERT_EQ(array[1].size(), 2);
    ASSERT_EQ(array[0][0], 5);
    ASSERT_EQ(array[0][1], 6);
    ASSERT_EQ(array[1][0], 7);
    ASSERT_EQ(array[1][1], 8);

  }
}

TEST_F(DbusDeserialization, DeserializeMap) {
  {
    dbus::MessageWriter writer(msgref);
    std::string dict_signature;
    rpc::DbusSignature<Map<Enum<TestEnum>, 1, 5 >::value_type>(&dict_signature);
    dbus::MessageWriter array_writer(&writer,
                                     dbus::kArray,
                                     dict_signature.c_str());
    const char* keys[] = { "Hello", "World" };
    int val = 0;
    for (int i = 0; i < 2; ++i) {
      dbus::MessageWriter dictval_wirter(&array_writer, dbus::kDictEntry, NULL);
      dictval_wirter.WriteString(keys[val]);
      dictval_wirter.WriteInt32(val++);
    }
  }
  {
    dbus::MessageReader reader(msgref);
    Map<Enum<TestEnum>, 1, 5 > amap(&reader);
    ASSERT_TRUE(amap.is_initialized());
    ASSERT_TRUE(amap.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(amap.size(), 2);
    ASSERT_EQ(amap["Hello"], kValue0);
    ASSERT_EQ(amap["World"], kValue1);

  }
}

TEST_F(DbusDeserialization, InconsistentTypesTest) {
  {
    dbus::MessageWriter writer(msgref);
    writer.WriteString("Hello");
  }
  {
    dbus::MessageReader reader(msgref);
    Boolean badbool(&reader);
    ASSERT_TRUE(badbool.is_initialized());
    ASSERT_FALSE(badbool.is_valid());
    ASSERT_TRUE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

}  // namespace test
