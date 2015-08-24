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
#include "dbus/dbus_message.h"
#include "rpc_base/rpc_base.h"
#include "rpc_base/rpc_base_dbus_inl.h"

namespace test {
using namespace rpc;
using namespace dbus;

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
  switch (enm) {
    case kValue0:
      return "kValue0";
    case kValue1:
      return "kValue1";
    default:
      return "UNKNOWN";
  }
}

struct DbusDeserialization : public testing::Test {
  dbus::MessageRef msgref;
  DbusDeserialization()
      : msgref(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
  }
};

TEST_F(DbusDeserialization, DeserializeBool) {
  {
    dbus::MessageWriter writer(msgref);
    writer.PutBool(true);
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
    writer.PutByte(200);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer < uint8_t, 1, 220 > byte(&reader);
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
    writer.PutInt64(-1);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer < int64_t, -5, 220 > int64(&reader);
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
    writer.PutDouble(3.14);
  }
  {
    dbus::MessageReader reader(msgref);
    Float < 3, 4 > pi(&reader);
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
    writer.PutString("Hello");
  }
  {
    dbus::MessageReader reader(msgref);
    String < 3, 10 > hello(&reader);
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
    writer.PutInt32(kValue1);
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
    dbus::MessageWriter array_writer(&writer, dbus::kArray,
                                     array_signature.c_str());
    array_writer.PutInt32(5);
    array_writer.PutInt32(33);
  }
  {
    dbus::MessageReader reader(msgref);
    Array<Integer<int32_t, 1, 50>, 1, 100> array(&reader);
    ASSERT_TRUE(array.is_initialized());
    ASSERT_TRUE(array.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(array.size(), 2u);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 33);

  }
}

TEST_F(DbusDeserialization, DeserializeArrayOfArrays) {
  {
    dbus::MessageWriter writer(msgref);
    std::string array_signature;
    rpc::DbusSignature<Array<Integer<int32_t, 1, 50>, 1, 5> >(&array_signature);
    dbus::MessageWriter array_writer(&writer, dbus::kArray,
                                     array_signature.c_str());
    int val = 5;
    for (int i = 0; i < 2; ++i) {
      std::string subarray_signature;
      rpc::DbusSignature<Integer<int32_t, 1, 50> >(&subarray_signature);
      dbus::MessageWriter subarray_wirter(&array_writer, dbus::kArray,
                                          subarray_signature.c_str());

      subarray_wirter.PutInt32(val++);
      subarray_wirter.PutInt32(val++);
    }
  }
  {
    dbus::MessageReader reader(msgref);
    Array<Array<Integer<int32_t, 1, 50>, 1, 5>, 1, 5> array(&reader);
    ASSERT_TRUE(array.is_initialized());
    ASSERT_TRUE(array.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(array.size(), 2u);
    ASSERT_EQ(array[0].size(), 2u);
    ASSERT_EQ(array[1].size(), 2u);
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
    rpc::DbusSignature<Map<Enum<TestEnum>, 1, 5>::value_type>(&dict_signature);
    dbus::MessageWriter array_writer(&writer, dbus::kArray,
                                     dict_signature.c_str());
    const char* keys[] = { "Hello", "World" };
    int val = 0;
    for (int i = 0; i < 2; ++i) {
      dbus::MessageWriter dictval_wirter(&array_writer, dbus::kDictEntry, NULL);
      dictval_wirter.PutString(keys[val]);
      dictval_wirter.PutInt32(val++);
    }
  }
  {
    dbus::MessageReader reader(msgref);
    Map<Enum<TestEnum>, 1, 5> amap(&reader);
    ASSERT_TRUE(amap.is_initialized());
    ASSERT_TRUE(amap.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
    ASSERT_EQ(amap.size(), 2u);
    ASSERT_EQ(amap["Hello"], kValue0);
    ASSERT_EQ(amap["World"], kValue1);

  }
}

TEST_F(DbusDeserialization, InconsistentTypesTest) {
  {
    dbus::MessageWriter writer(msgref);
    writer.PutString("Hello");
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

TEST_F(DbusDeserialization, DeserializeOptionalString) {
  {
    dbus::MessageWriter writer(msgref);
    dbus::MessageWriter optwriter(&writer, dbus::kStruct, NULL);
    optwriter.PutBool(true);
    optwriter.PutString("Hello dear");
  }
  {
    dbus::MessageReader reader(msgref);
    Optional < String<1, 100> > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(std::string(*readback), "Hello dear");
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, DeserializeOptionalInt) {
  {
    dbus::MessageWriter writer(msgref);
    dbus::MessageWriter optwriter(&writer, dbus::kStruct, NULL);
    optwriter.PutBool(false);
    optwriter.PutInt32(42);
  }
  {
    dbus::MessageReader reader(msgref);
    Optional < Integer<int32_t, 1, 90>> readback(&reader);
    ASSERT_FALSE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());

  }
}


TEST_F(DbusDeserialization, SerializeDeserializeBool) {
  {
    Boolean true_bool(true);
    dbus::MessageWriter writer(msgref);
    true_bool.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Boolean readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback, true);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeInt8t) {
  {
    Integer < int8_t, 1, 100 > int8(42);
    dbus::MessageWriter writer(msgref);
    int8.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer < int8_t, 1, 100 > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback, 42);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, BadSerializeDeserializeInt8t) {
  {
    Integer < int8_t, 1, 12 > int8(42);
    dbus::MessageWriter writer(msgref);
    int8.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer < int8_t, 1, 12 > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_FALSE(readback.is_valid());
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeInt64t) {
  {
    Integer < int64_t, 1, 0xFFFFFFFFFF > int64(0xFFFFFFFFF1);
    dbus::MessageWriter writer(msgref);
    int64.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Integer < int64_t, 1, 0xFFFFFFFFFF > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback, 0xFFFFFFFFF1);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}


TEST_F(DbusDeserialization, SerializeDeserializeDouble) {
  {
    Float < 1, 5 > flt(3.14);
    dbus::MessageWriter writer(msgref);
    flt.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Float < 1, 5 > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_DOUBLE_EQ(readback, 3.14);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeString) {
  {
    String < 1, 12 > hello("Hello");
    dbus::MessageWriter writer(msgref);
    hello.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    String < 1, 12 > readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(std::string(readback), "Hello");
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeEnum) {
  {
    Enum<TestEnum> te(kValue1);
    dbus::MessageWriter writer(msgref);
    te.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Enum<TestEnum> readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback, kValue1);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeArray) {
  {
    Array<Integer<int32_t, 1, 100>, 1, 90> ints;
    ints.push_back(42);
    ints.push_back(17);
    dbus::MessageWriter writer(msgref);
    ints.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Array<Integer<int32_t, 1, 100>, 1, 90> readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback[0], 42);
    ASSERT_EQ(readback[1], 17);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeMap) {
  {
    Map<Integer<int32_t, 1, 100>, 1, 90> ints;
    ints["first"] = 42;
    ints["second"] = 17;
    dbus::MessageWriter writer(msgref);
    ints.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Map<Integer<int32_t, 1, 100>, 1, 90> readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback["first"], 42);
    ASSERT_EQ(readback["second"], 17);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusDeserialization, SerializeDeserializeMapOfArrays) {
  {
    Map<Array<Integer<int32_t, 1, 100>, 1, 5>, 1, 90> ints;
    ints["first"].push_back(1);
    ints["first"].push_back(42);
    ints["second"].push_back(17);
    ints["second"].push_back(3);
    dbus::MessageWriter writer(msgref);
    ints.ToDbusWriter(&writer);
  }
  {
    dbus::MessageReader reader(msgref);
    Map<Array<Integer<int32_t, 1, 100>, 1, 5>, 1, 90> readback(&reader);
    ASSERT_TRUE(readback.is_initialized());
    ASSERT_TRUE(readback.is_valid());
    ASSERT_EQ(readback.size(), 2u);
    ASSERT_EQ(readback["first"].size(), 2u);
    ASSERT_EQ(readback["second"].size(), 2u);
    ASSERT_EQ(readback["first"][0], 1);
    ASSERT_EQ(readback["first"][1], 42);
    ASSERT_EQ(readback["second"][0], 17);
    ASSERT_EQ(readback["second"][1], 3);
    ASSERT_FALSE(reader.has_failed());
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST(ValidatedTypes, TestBooleanDbusSignature) {
  std::string sign;
  DbusSignature<Boolean>(&sign);
  ASSERT_EQ(sign, "b");
}

TEST(ValidatedTypes, TestIntDbusSignature) {
  std::string sign;
  DbusSignature<Integer<int32_t, 1, 2> >(&sign);
  ASSERT_EQ(sign, "i");
}

TEST(ValidatedTypes, TestFloatDbusSignature) {
  std::string sign;
  DbusSignature<Float<1, 2> >(&sign);
  ASSERT_EQ(sign, "d");
}

TEST(ValidatedTypes, TestStringDbusSignature) {
  std::string sign;
  DbusSignature<String<1, 2> >(&sign);
  ASSERT_EQ(sign, "s");
}

TEST(ValidatedTypes, TestEnumDbusSignature) {
  std::string sign;
  DbusSignature<Enum<TestEnum> >(&sign);
  ASSERT_EQ(sign, "i");
}

TEST(ValidatedTypes, TestIntArrayDbusSignature) {
  std::string sign;
  DbusSignature<Array<Integer<int32_t, 1, 2>, 1, 3> >(&sign);
  ASSERT_EQ(sign, "ai");
}

TEST(ValidatedTypes, TestIntArrayArrayDbusSignature) {
  std::string sign;
  DbusSignature<Array<Array<Integer<int32_t, 1, 2>, 1, 3>, 4, 5> >(&sign);
  ASSERT_EQ(sign, "aai");
}

TEST(ValidatedTypes, TestMapDbusSignature) {
  std::string sign;
  DbusSignature<Map<Integer<int32_t, 1, 2>, 3, 4> >(&sign);
  ASSERT_EQ(sign, "a{si}");
}

TEST(ValidatedTypes, TestMandatoryEnumDbusSignature) {
  std::string sign;
  DbusSignature<Enum<TestEnum> >(&sign);
  ASSERT_EQ(sign, "i");
}

TEST(ValidatedTypes, TestOptionalEnumDbusSignature) {
  std::string sign;
  DbusSignature<Optional<Enum<TestEnum> > >(&sign);
  ASSERT_EQ(sign, "(bi)");
}

TEST(ValidatedTypes, TestOptionalFloatArrayDbusSignature) {
  std::string sign;
  DbusSignature<Optional<Array<Float<1, 2>, 3, 4> > >(&sign);
  ASSERT_EQ(sign, "(bad)");
}

TEST(DbusMessageConstructionTest, DbusMessageConstruction) {
  DBusMessage* rawmsg = dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL);
  dbus::MessageRef msgref(rawmsg);
}

class DbusTest : public testing::Test {
 public:
  dbus::MessageRef msgref;
  DbusTest()
      : msgref(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
  }
};

TEST_F(DbusTest, DbusWriterConstructionTest) {
  dbus::MessageWriter writer(msgref);
}

TEST_F(DbusTest, DbusEmptyMessageReaderTest) {
  dbus::MessageReader reader(msgref);
  ASSERT_TRUE(reader.has_failed());
}

TEST_F(DbusTest, DbusMessageWriterBoolWriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.PutBool(true);
  dbus::MessageReader reader(msgref);
  bool redback_value = reader.TakeBool();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_TRUE(redback_value);
}

TEST_F(DbusTest, DbusMessageWriterInt32WriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.PutInt32(42);
  dbus::MessageReader reader(msgref);
  int32_t readback_value = reader.TakeInt32();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_EQ(readback_value, 42);
}

TEST_F(DbusTest, DbusMessageWriterStringWriteRead) {
  dbus::MessageWriter writer(msgref);
  writer.PutString("Hello DBus!");
  dbus::MessageReader reader(msgref);
  std::string readback_value = reader.TakeString();
  ASSERT_FALSE(reader.has_failed());
  ASSERT_EQ(readback_value, "Hello DBus!");
}

TEST_F(DbusTest, DbusMultipleParamsReadWrite) {
  {
    dbus::MessageWriter writer(msgref);
    writer.PutString("Hello DBus!");
    writer.PutInt16(42);
    writer.PutDouble(3.14);
  }
  {
    dbus::MessageReader reader(msgref);
    std::string readback_string = reader.TakeString();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_string, "Hello DBus!");
    int16_t readback_int = reader.TakeInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_int, 42);
    double readback_double = reader.TakeDouble();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_DOUBLE_EQ(readback_double, 3.14);
    ASSERT_FALSE(reader.HasNext());
  }
}

TEST_F(DbusTest, DbusArrayTest) {
  {
    dbus::MessageWriter writer(msgref);
    dbus::MessageWriter array_writer(&writer, dbus::kArray,
    DBUS_TYPE_INT16_AS_STRING);
    array_writer.PutInt16(3);
    array_writer.PutInt16(4);
    array_writer.PutInt16(5);
  }
  {
    dbus::MessageReader reader(msgref);
    dbus::MessageReader array_reader = reader.TakeArrayReader();
    int16_t readback_val = array_reader.TakeInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 3);
    readback_val = array_reader.TakeInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 4);
    readback_val = array_reader.TakeInt16();
    ASSERT_FALSE(reader.has_failed());
    ASSERT_EQ(readback_val, 5);
    ASSERT_FALSE(array_reader.HasNext());
  }
}

class DbusFailuresTest : public testing::Test {
 public:
  dbus::MessageRef int_msg;
  DbusFailuresTest()
      : int_msg(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL)) {
    dbus::MessageWriter writer(int_msg);
    writer.PutInt64(42);
  }
};

TEST_F(DbusFailuresTest, DbusInconsistentTypeReadFailureTest) {
  dbus::MessageReader reader(int_msg);
  std::string str = reader.TakeString();
  ASSERT_EQ(str, std::string(""));
  ASSERT_TRUE(reader.has_failed());
}

TEST_F(DbusFailuresTest, DbusNonExistentArrayReadTest) {
  dbus::MessageReader reader(int_msg);
  ASSERT_FALSE(reader.has_failed());
  dbus::MessageReader array_reader = reader.TakeArrayReader();
  ASSERT_TRUE(array_reader.has_failed());
  ASSERT_TRUE(reader.has_failed());
  int64_t val = array_reader.TakeInt64();
  ASSERT_TRUE(array_reader.has_failed());
  ASSERT_EQ(val, 0);
}

}  // namespace test
