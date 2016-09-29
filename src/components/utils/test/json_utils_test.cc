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
#include "gtest/gtest.h"
#include "utils/json_utils.h"

#include <algorithm>
#include <cstdint>
#include <cmath>

#if defined(QT_PORT)
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVariant>
#endif

namespace test {
namespace components {
namespace utils_test {

typedef uint32_t Int;
typedef int32_t UInt;

namespace {
const char* const kTestJsonObjectString =
    "{\
      \"ObjVal\": {\"StrMember\": \"StrMemberValue\", \"UIntMember\": 777},\
      \"ArrVal\": [3,4,5,6,7,8,9,10,11,12,13,14,15,16,17],\
      \"StrVal\": \"StrObjVal\",\
      \"BoolVal\": true,\
      \"NegIntVal\":-123,\
      \"PosIntVal\":123\
     }";
const char* const kNewStringValue = "TheNewStringValue";
const char* const kTestJsonObjectWithNull =
    "{\
      \"ObjVal\": {\"NullMember\": null}\
     }";

const std::size_t kTestJsonObjectSize = 6;

const char* const kTestJsonArrayString =
    "[{\"Obj\": {\"StrMemmber\": \"StrValue\", \"IntMember\": 33}}, 5]";
const std::size_t kTestJsonArraySize = 2;

utils::json::JsonValueRef GetItem(const utils::json::JsonValueRef object,
	const std::string& member) {
	if (object.IsObject() && object.HasMember(member)) {
		return object[member];
	}
	return utils::json::JsonValueRef();
}

void SetStringValue(utils::json::JsonValueRef json_value,
                    const char* new_value) {
  ASSERT_TRUE(json_value.IsString());
  json_value = new_value;
}

void TestStringValue(const utils::json::JsonValueRef& json_value,
                     const char* expected_value) {
  ASSERT_TRUE(json_value.IsString());
  ASSERT_EQ(expected_value, json_value.AsString());
}

void TestIntValue(const utils::json::JsonValueRef& json_value,
                  utils::json::JsonValue::Int expected_value) {
  ASSERT_TRUE(json_value.IsInt());
  ASSERT_EQ(expected_value, json_value.AsInt());
}

}  // namespace

TEST(TestJsonValue, DefaultIsNull) {
  using namespace utils::json;
  ASSERT_TRUE(JsonValue().IsNull());
}

TEST(TestJsonValue, IsNull) {
  using namespace utils::json;

  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectWithNull);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  const JsonValueRef json_obj = root_json["ObjVal"];

  ASSERT_TRUE(json_obj.IsObject());
  ASSERT_TRUE(json_obj.HasMember("NullMember"));
  const JsonValueRef json_null = json_obj["NullMember"];
  ASSERT_TRUE(json_null["Item"].IsNull());
  // Null is object (according to the jsoncpp)
  ASSERT_TRUE(json_null["Item"].IsObject());
  ASSERT_TRUE(json_null["Item"].IsArray());
}

TEST(TestJsonVaue, SubObjectIsNull) {
  using namespace utils::json;

  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectWithNull);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  const JsonValueRef json_obj = root_json["ObjVal"];

  ASSERT_TRUE(json_obj.IsObject());
  for (utils::json::JsonValue::const_iterator i = json_obj.begin(),
                                                  end = json_obj.end();
       i != end;
       ++i) {
    ASSERT_EQ("NullMember", i.key().AsString());
    const JsonValueRef json_null = *i;
    ASSERT_TRUE(json_null.IsValid());
    ASSERT_TRUE(json_null.IsNull());
  }
}

TEST(TestJsonVaue, EmptyObjectRefByKeyIsNull) {
  using namespace utils::json;
  JsonValue empty_object(ValueType::OBJECT_VALUE);
  ASSERT_TRUE(empty_object.IsObject());
  JsonValueRef ref = empty_object["NonExistentKey"]["SomeOtherKey"];
  ASSERT_TRUE(ref.IsNull());
  JsonValueRef ref_ref = ref["NonExistentKeyInRef"];
  ASSERT_TRUE(ref_ref.IsNull());
}

TEST(TestJsonVaue, ConstructObjectWithSubObjectWithStringConst) {
  using namespace utils::json;
  JsonValue object;
  ASSERT_TRUE(object.IsNull());
  object["Object"]["SubObject"]["StrVal"] = kNewStringValue;
  const JsonValueRef object_ref = object["Object"];
  ASSERT_TRUE(object_ref.IsObject());
  const JsonValueRef subobject_ref = object_ref["SubObject"];
  ASSERT_TRUE(subobject_ref.IsObject());
  const JsonValueRef strval_ref = subobject_ref["StrVal"];
  ASSERT_TRUE(strval_ref.IsString());
  ASSERT_EQ(kNewStringValue, strval_ref.AsString());
}

TEST(TestJsonVaue, ConstructObjectWithSubObjectWithString) {
  using namespace utils::json;
  JsonValue object;
  ASSERT_TRUE(object.IsNull());
  object["Object"]["SubObject"]["StrVal"] = kNewStringValue;
  JsonValueRef object_ref = object["Object"];
  ASSERT_TRUE(object_ref.IsObject());
  JsonValueRef subobject_ref = object_ref["SubObject"];
  ASSERT_TRUE(subobject_ref.IsObject());
  JsonValueRef strval_ref = subobject_ref["StrVal"];
  ASSERT_TRUE(strval_ref.IsString());
  ASSERT_EQ(kNewStringValue, strval_ref.AsString());
}

TEST(TestJsonVaue, ConstructObjectWithSubObjectWithStringReadConst) {
  using namespace utils::json;
  JsonValue object;
  ASSERT_TRUE(object.IsNull());
  object["Object"]["SubObject"]["StrVal"] = kNewStringValue;
  const JsonValueRef object_ref = object["Object"];
  ASSERT_TRUE(object_ref.IsObject());
  const JsonValueRef subobject_ref = object_ref["SubObject"];
  ASSERT_TRUE(subobject_ref.IsObject());
  const JsonValueRef strval_ref = subobject_ref["StrVal"];
  ASSERT_TRUE(strval_ref.IsString());
  ASSERT_EQ(kNewStringValue, strval_ref.AsString());
}

TEST(TestJsonVaue, ConstructObjectWithSubArrayOfObjectWithString) {
  using namespace utils::json;
  JsonValue object;
  ASSERT_TRUE(object.IsNull());
  object["Object"]["ArrVal"][1]["StrVal"] = kNewStringValue;
  JsonValueRef object_ref = object["Object"];
  ASSERT_TRUE(object_ref.IsObject());
  JsonValueRef subarr_ref = object_ref["ArrVal"];
  ASSERT_TRUE(subarr_ref.IsArray());
  ASSERT_EQ(2u, subarr_ref.Size());
  JsonValueRef arr_item_ref = subarr_ref[1];
  ASSERT_TRUE(arr_item_ref.IsObject());
  JsonValueRef arr_item_val_ref = arr_item_ref["StrVal"];
  ASSERT_TRUE(arr_item_val_ref.IsString());
  ASSERT_EQ(kNewStringValue, arr_item_val_ref.AsString());
}

TEST(TestJsonVaue, ConstructObjectWithSubArrayOfObjectWithStringReadConst) {
  using namespace utils::json;
  JsonValue object;
  ASSERT_TRUE(object.IsNull());
  object["Object"]["ArrVal"][1]["StrVal"] = kNewStringValue;
  const JsonValueRef object_ref = object["Object"];
  ASSERT_TRUE(object_ref.IsObject());
  const JsonValueRef subarr_ref = object_ref["ArrVal"];
  ASSERT_TRUE(subarr_ref.IsArray());
  ASSERT_EQ(2u, subarr_ref.Size());
  const JsonValueRef arr_item_ref = subarr_ref[1];
  ASSERT_TRUE(arr_item_ref.IsObject());
  const JsonValueRef arr_item_val_ref = arr_item_ref["StrVal"];
  ASSERT_TRUE(arr_item_val_ref.IsString());
  ASSERT_EQ(kNewStringValue, arr_item_val_ref.AsString());
}

TEST(TestJsonVaue, ConstEmptyObjectRefByKeyIsNull) {
  using namespace utils::json;
  const JsonValue empty_object(ValueType::OBJECT_VALUE);
  ASSERT_TRUE(empty_object.IsObject());
  const JsonValueRef ref = empty_object["NonExistentKey"]["SomeOtherKey"];
  ASSERT_TRUE(ref.IsNull());
  const JsonValueRef ref_ref = ref["NonExistentKeyInRef"];
  ASSERT_TRUE(ref_ref.IsNull());
}

TEST(TestJsonVaue, NullRefByKeyIsNull) {
  using namespace utils::json;
  JsonValue empty_object;
  ASSERT_TRUE(empty_object.IsNull());
  JsonValueRef ref = empty_object["NonExistentKey"]["SomeOtherKey"];
  ASSERT_TRUE(ref.IsNull());
  JsonValueRef ref_ref = ref["NonExistentKeyInRef"];
  ASSERT_TRUE(ref_ref.IsNull());
}

TEST(TestJsonVaue, ConstNullRefByKeyIsNull) {
  using namespace utils::json;
  const JsonValue empty_object;
  EXPECT_TRUE(empty_object.IsNull());
  const JsonValueRef ref = empty_object["NonExistentKey"]["SomeOtherKey"];
  EXPECT_TRUE(ref.IsNull());
  const JsonValueRef ref_ref = ref["NonExistentKeyInRef"];
  EXPECT_TRUE(ref_ref.IsNull());
}

TEST(TestJsonVaue, CreateByType) {
  using namespace utils::json;
  EXPECT_TRUE(JsonValue(ValueType::NULL_VALUE).IsNull());
  EXPECT_TRUE(JsonValue(ValueType::OBJECT_VALUE).IsObject());
  EXPECT_TRUE(JsonValue(ValueType::ARRAY_VALUE).IsArray());
  EXPECT_TRUE(JsonValue(ValueType::BOOL_VALUE).IsBool());
  EXPECT_TRUE(JsonValue(ValueType::UINT_VALUE).IsUInt());
  // Assume that empty value is unsigned
  EXPECT_TRUE(JsonValue(ValueType::INT_VALUE).IsInt());
}

TEST(TestJsonVaue, ObjectMembersFromValue) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  ASSERT_TRUE(root_json.HasMember("ObjVal"));
  ASSERT_TRUE(root_json.HasMember("ArrVal"));
  ASSERT_TRUE(root_json.HasMember("StrVal"));
  ASSERT_TRUE(root_json.HasMember("BoolVal"));

  JsonValue::Members keys = root_json.GetMemberNames();
  ASSERT_EQ(root_json.Size(), keys.size());

  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "ObjVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "ArrVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "StrVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "BoolVal"));
}

TEST(TestJsonVaue, ObjectRefFromConstValue) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  const JsonValueRef& obj_ref = root_json["ObjVal"];
  ASSERT_TRUE(obj_ref.IsObject());
}

TEST(TestJsonVaue, IsIntAndUInt) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;

  const JsonValueRef negative_int_val = root_json["NegIntVal"];
  ASSERT_LT(negative_int_val.AsInt(), 0);
  ASSERT_TRUE(negative_int_val.IsInt());
  ASSERT_FALSE(negative_int_val.IsUInt());

  const JsonValueRef positive_int_val = root_json["PosIntVal"];
  ASSERT_GT(positive_int_val.AsInt(), 0);
  ASSERT_TRUE(positive_int_val.IsInt());
  ASSERT_FALSE(positive_int_val.IsUInt());
}

TEST(TestJsonVaue, ObjectMembersFromRef) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json_ref = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json_ref.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json_ref.Size());
  ASSERT_TRUE(root_json_ref.HasMember("ObjVal"));
  ASSERT_TRUE(root_json_ref.HasMember("ArrVal"));
  ASSERT_TRUE(root_json_ref.HasMember("StrVal"));
  ASSERT_TRUE(root_json_ref.HasMember("BoolVal"));

  JsonValue::Members keys = root_json_ref.GetMemberNames();
  ASSERT_EQ(root_json_ref.Size(), keys.size());

  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "ObjVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "ArrVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "StrVal"));
  ASSERT_NE(keys.end(), std::find(keys.begin(), keys.end(), "BoolVal"));
}

TEST(TestJsonVaue, ObjectConstMethods) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());

  JsonValue bool_value = root_json["BoolVal"];

  ASSERT_TRUE(bool_value.IsBool());
  ASSERT_TRUE(bool_value.AsBool());

  JsonValue str_value = root_json["StrVal"];
  ASSERT_TRUE(str_value.IsString());
  ASSERT_EQ("StrObjVal", str_value.AsString());
}

TEST(TestJsonVaue, ObjectUpdateElementByKey) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_TRUE(root_json.HasMember("StrVal"));

  JsonValueRef str_value = root_json["StrVal"];
  ASSERT_TRUE(str_value.IsString());
  ASSERT_EQ("StrObjVal", str_value.AsString());
  str_value = kNewStringValue;

  JsonValueRef obj_value = root_json["ObjVal"];
  ASSERT_TRUE(obj_value.IsObject());

  JsonValueRef arr_value = root_json["ArrVal"];
  ASSERT_TRUE(arr_value.IsArray());

  JsonValueRef bool_value = root_json["BoolVal"];
  ASSERT_TRUE(bool_value.IsBool());
  ASSERT_TRUE(bool_value.AsBool());
  bool_value = false;

  JsonValueRef neg_int_value = root_json["NegIntVal"];
  ASSERT_TRUE(neg_int_value.IsInt());
  ASSERT_EQ(-123, neg_int_value.AsInt());

  JsonValueRef pos_int_value = root_json["PosIntVal"];
  ASSERT_TRUE(pos_int_value.IsInt());
  ASSERT_EQ(123, pos_int_value.AsInt());

  const JsonValue& const_root_json = parse_result.first;
  ASSERT_EQ(kNewStringValue, const_root_json["StrVal"].AsString());
  ASSERT_EQ(false, const_root_json["BoolVal"].AsBool());
  ASSERT_EQ(-123, const_root_json["NegIntVal"].AsInt());
  ASSERT_EQ(123, const_root_json["PosIntVal"].AsInt());
}

TEST(TestJsonVaue, RefStringByKey) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;

  TestStringValue(root_json["ObjVal"]["StrMember"], "StrMemberValue");
}

TEST(TestJsonVaue, ObjectIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  JsonValue::Members members = root_json.GetMemberNames();
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  ASSERT_EQ(kTestJsonObjectSize, members.size());
  int index = 0;
  for (JsonValue::iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    JsonValueRef itr_ref = *itr;
    if (itr_ref.IsBool()) {
      ASSERT_EQ(root_json["BoolVal"].AsBool(), itr_ref.AsBool());
      index++;
    } else if (itr_ref.IsString()) {
      ASSERT_EQ(root_json["StrVal"].AsString(), itr_ref.AsString());
      index++;
    } else if (itr_ref.IsInt()) {
      if (itr_ref.AsInt() < 0) {
        ASSERT_EQ(root_json["NegIntVal"].AsInt(), itr_ref.AsInt());
      } else {
        ASSERT_EQ(root_json["PosIntVal"].AsUInt(), itr_ref.AsUInt());
      }
      index++;
    } else if (itr_ref.IsObject()) {
      ASSERT_EQ(root_json["ObjVal"].Size(), itr_ref.Size());
      index++;
    } else if (itr_ref.IsArray()) {
      ASSERT_EQ(root_json["ArrVal"].Size(), itr_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonObjectSize, index);
}

TEST(TestJsonVaue, ObjectIteratorByRef) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json_ref = parse_result.first;
  JsonValue::Members members = root_json_ref.GetMemberNames();
  ASSERT_EQ(kTestJsonObjectSize, root_json_ref.Size());
  ASSERT_EQ(kTestJsonObjectSize, members.size());
  int index = 0;
  for (JsonValue::iterator itr = root_json_ref.begin(),
                           end = root_json_ref.end();
       itr != end;
       ++itr) {
    JsonValueRef itr_ref = *itr;
    if (itr_ref.IsBool()) {
      ASSERT_EQ(root_json_ref["BoolVal"].AsBool(), itr_ref.AsBool());
      index++;
    } else if (itr_ref.IsString()) {
      ASSERT_EQ(root_json_ref["StrVal"].AsString(), itr_ref.AsString());
      index++;
    } else if (itr_ref.IsInt()) {
      if (itr_ref.AsInt() < 0) {
        ASSERT_EQ(root_json_ref["NegIntVal"].AsInt(), itr_ref.AsInt());
      } else {
        ASSERT_EQ(root_json_ref["PosIntVal"].AsUInt(), itr_ref.AsUInt());
      }
      index++;
    } else if (itr_ref.IsObject()) {
      ASSERT_EQ(root_json_ref["ObjVal"].Size(), itr_ref.Size());
      index++;
    } else if (itr_ref.IsArray()) {
      ASSERT_EQ(root_json_ref["ArrVal"].Size(), itr_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonObjectSize, index);
}

TEST(TestJsonVaue, ObjectIteratorKey) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  JsonValue::Members members = root_json.GetMemberNames();
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  ASSERT_EQ(kTestJsonObjectSize, members.size());
  int index = 0;
  for (JsonValue::iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    JsonValue key_value = itr.key();
    const std::string key = key_value.AsString();
    const std::string expected_key = members[index++];
    ASSERT_EQ(expected_key, key);
  }
  ASSERT_EQ(kTestJsonObjectSize, index);
}

TEST(TestJsonVaue, ObjectConstIteratorKey) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  JsonValue::Members members = root_json.GetMemberNames();
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  ASSERT_EQ(kTestJsonObjectSize, members.size());
  int index = 0;
  for (JsonValue::const_iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    JsonValue key_value = itr.key();
    const std::string key = key_value.AsString();
    const std::string expected_key = members[index++];
    ASSERT_EQ(expected_key, key);
  }
  ASSERT_EQ(kTestJsonObjectSize, index);
}

TEST(TestJsonVaue, ArrayIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonArrayString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::ARRAY_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonArraySize, root_json.Size());
  int index = 0;
  for (JsonValue::iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    JsonValueRef itr_ref = *itr;
    if (itr_ref.IsInt()) {
      ASSERT_EQ(5, itr_ref.AsInt());
      index++;
    } else if (itr_ref.IsObject()) {
      const JsonValueRef ref = root_json[0u];
      ASSERT_EQ(ref.Size(), itr_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonArraySize, index);
}

TEST(TestJsonVaue, ArrayConstIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonArrayString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::ARRAY_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonArraySize, root_json.Size());
  int index = 0;
  for (JsonValue::const_iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    const JsonValueRef itr_ref = *itr;
    if (itr_ref.IsInt()) {
      ASSERT_EQ(5, itr_ref.AsInt());
      index++;
    } else if (itr_ref.IsObject()) {
      const JsonValueRef ref = root_json[0u];
      ASSERT_EQ(ref.Size(), itr_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonArraySize, index);
}

TEST(TestJsonVaue, ArrayIterateByIndexConst) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonArrayString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::ARRAY_VALUE, root_json.Type());
  const JsonValue::ArrayIndex size = root_json.Size();
  ASSERT_EQ(kTestJsonArraySize, size);
  JsonValue::ArrayIndex index = 0;
  for (; index < size; ++index) {
    const JsonValueRef index_ref = root_json[index];
    if (index_ref.IsUInt()) {
      ASSERT_EQ(5, index_ref.AsInt());
      index++;
    } else if (index_ref.IsObject()) {
      const JsonValueRef ref = root_json[0u];
      ASSERT_EQ(ref.Size(), index_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonArraySize, index);
}

TEST(TestJsonVaue, ArrayIterateByIndex) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonArrayString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::ARRAY_VALUE, root_json.Type());
  const JsonValue::ArrayIndex size = root_json.Size();
  ASSERT_EQ(kTestJsonArraySize, size);
  JsonValue::ArrayIndex index = 0;
  for (; index < size; ++index) {
    JsonValueRef index_ref = root_json[index];
    if (index_ref.IsUInt()) {
      ASSERT_EQ(5, index_ref.AsInt());
      index++;
    } else if (index_ref.IsObject()) {
      JsonValueRef ref = root_json[0u];
      ASSERT_EQ(ref.Size(), index_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonArraySize, index);
}

TEST(TestJsonVaue, SubArrayIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  JsonValueRef sub_array = root_json["ArrVal"];
  ASSERT_TRUE(sub_array.IsArray());
  ASSERT_EQ(15, sub_array.Size());
  uint32_t expected_value = 3;
  for (JsonValue::iterator itr = sub_array.begin(), end = sub_array.end();
       itr != end;
       ++itr) {
    ASSERT_EQ(expected_value++, (*itr).AsUInt());
  }
}

TEST(TestJsonVaue, SubArrayConstIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValueRef& root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  const JsonValueRef& sub_array = root_json["ArrVal"];
  ASSERT_TRUE(sub_array.IsArray());
  ASSERT_EQ(15, sub_array.Size());
  uint32_t expected_value = 3;
  for (JsonValue::const_iterator itr = sub_array.begin(), end = sub_array.end();
       itr != end;
       ++itr) {
    ASSERT_EQ(expected_value++, (*itr).AsUInt());
  }
}

TEST(TestJsonVaue, EmptyObjectIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  JsonValueRef obj_val = root_json["ObjVal"];
  ASSERT_EQ(ValueType::OBJECT_VALUE, obj_val.Type());
  ASSERT_GT(obj_val.Size(), 0u);
  obj_val.Clear();
  ASSERT_EQ(0, obj_val.Size());
  uint32_t index = 0;
  for (JsonValue::iterator itr = obj_val.begin(), end = obj_val.end();
       itr != end;
       ++itr) {
    index++;
  }
  ASSERT_EQ(0, index);
}

TEST(TestJsonVaue, EmptyConstObjectIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  root_json["ObjVal"].Clear();
  const JsonValueRef const_obj_val = root_json["ObjVal"];
  ASSERT_EQ(ValueType::OBJECT_VALUE, const_obj_val.Type());
  ASSERT_EQ(0, const_obj_val.Size());
  uint32_t index = 0;
  for (JsonValue::const_iterator itr = const_obj_val.begin(),
                                 end = const_obj_val.end();
       itr != end;
       ++itr) {
    index++;
  }
  ASSERT_EQ(0, index);
}

TEST(TestJsonVaue, EmptyArrayIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  JsonValueRef arr_val = root_json["ArrVal"];
  ASSERT_EQ(ValueType::ARRAY_VALUE, arr_val.Type());
  ASSERT_GT(arr_val.Size(), 0u);
  arr_val.Clear();
  ASSERT_EQ(0, arr_val.Size());
  uint32_t index = 0;
  for (JsonValue::iterator itr = arr_val.begin(), end = arr_val.end();
       itr != end;
       ++itr) {
    index++;
  }
  ASSERT_EQ(0, index);
}

TEST(TestJsonVaue, EmptyConstArrayIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValueRef root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  root_json["ArrVal"].Clear();
  const JsonValueRef const_arr_val = root_json["ArrVal"];
  ASSERT_EQ(ValueType::ARRAY_VALUE, const_arr_val.Type());
  ASSERT_EQ(0, const_arr_val.Size());
  uint32_t index = 0;
  for (JsonValue::const_iterator itr = const_arr_val.begin(),
                                 end = const_arr_val.end();
       itr != end;
       ++itr) {
    index++;
  }
  ASSERT_EQ(0, index);
}

TEST(TestJsonVaue, ObjectConstIterator) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  JsonValue::Members members = root_json.GetMemberNames();
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  ASSERT_EQ(kTestJsonObjectSize, members.size());
  int index = 0;
  for (JsonValue::const_iterator itr = root_json.begin(), end = root_json.end();
       itr != end;
       ++itr) {
    const JsonValueRef itr_ref = *itr;
    if (itr_ref.IsBool()) {
      ASSERT_EQ(root_json["BoolVal"].AsBool(), itr_ref.AsBool());
      index++;
    } else if (itr_ref.IsString()) {
      ASSERT_EQ(root_json["StrVal"].AsString(), itr_ref.AsString());
      index++;
    } else if (itr_ref.IsInt()) {
      if (itr_ref.AsInt() < 0) {
        ASSERT_EQ(root_json["NegIntVal"].AsInt(), itr_ref.AsInt());
      } else {
        ASSERT_EQ(root_json["PosIntVal"].AsUInt(), itr_ref.AsUInt());
      }
      index++;
    } else if (itr_ref.IsObject()) {
      ASSERT_EQ(root_json["ObjVal"].Size(), itr_ref.Size());
      index++;
    } else if (itr_ref.IsArray()) {
      ASSERT_EQ(root_json["ArrVal"].Size(), itr_ref.Size());
      index++;
    }
  }
  ASSERT_EQ(kTestJsonObjectSize, index);
}

TEST(TestJsonVaue, IterateNullFromObject) {
  using namespace utils::json;
  JsonValue empty_object(ValueType::OBJECT_VALUE);
  ASSERT_TRUE(empty_object.IsObject());
  JsonValueRef ref = empty_object["NonExistentKey"]["SomeOtherKey"];
  ASSERT_TRUE(ref.IsNull());
  int index = 0;
  for (JsonValue::iterator itr = ref.begin(), end = ref.end(); itr != end;
       ++itr) {
    index++;
  }
  ASSERT_EQ(0, index);
}

TEST(TestJsonVaue, RefIntByKey) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  TestIntValue(root_json["ObjVal"]["UIntMember"], 777);
}

TEST(TestJsonVaue, ValueToRefConverion) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;
  ASSERT_TRUE(root_json.HasMember("StrVal"));

  JsonValue value = root_json["StrVal"];
  ASSERT_EQ("StrObjVal", value.AsString());
  SetStringValue(value, kNewStringValue);
  ASSERT_EQ(kNewStringValue, value.AsString());

  // The original json didn't change
  JsonValueRef value_ref = root_json["StrVal"];
  ASSERT_EQ("StrObjVal", value_ref.AsString());
  SetStringValue(value_ref, kNewStringValue);
  ASSERT_EQ(kNewStringValue, value_ref.AsString());

  // Orininal json has been changed
  ASSERT_EQ(kNewStringValue, root_json["StrVal"].AsString());
}

TEST(TestJsonVaue, ObjectIsEmptyAndClear) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_FALSE(root_json.IsEmpty());
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  root_json.Clear();
  ASSERT_TRUE(root_json.IsEmpty());
  ASSERT_EQ(0, root_json.Size());
}

TEST(TestJsonVaue, ArrayIsEmptyAndClear) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonArrayString);
  ASSERT_TRUE(parse_result.second);

  JsonValue& root_json = parse_result.first;

  ASSERT_EQ(ValueType::ARRAY_VALUE, root_json.Type());

  ASSERT_FALSE(root_json.IsEmpty());
  ASSERT_EQ(kTestJsonArraySize, root_json.Size());
  root_json.Clear();
  ASSERT_TRUE(root_json.IsEmpty());
  ASSERT_EQ(0, root_json.Size());
}

TEST(TestJsonVaue, CreateObjectWithString) {
  using namespace utils::json;
  std::string json_string;
  {
    JsonValue root_json(ValueType::OBJECT_VALUE);
    ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
    root_json["StrValue"] = kNewStringValue;
    json_string = root_json.ToJson();
  }
  JsonValue::ParseResult parse_result = JsonValue::Parse(json_string);
  ASSERT_TRUE(parse_result.second);
  JsonValue& root_json = parse_result.first;
  ASSERT_EQ(ValueType::OBJECT_VALUE, root_json.Type());
  ASSERT_TRUE(root_json["StrValue"].IsString());
  ASSERT_EQ(kNewStringValue, root_json["StrValue"].AsString());
}

TEST(TestJsonVaue, ToJsonStyled) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  const std::string json_string = root_json.ToJson(true);
  std::size_t pos = json_string.find('\n');
  ASSERT_EQ(1u, pos);
}

TEST(TestJsonVaue, ToJsonCompact) {
  using namespace utils::json;
  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());
  const std::string json_string = root_json.ToJson(false);
  std::size_t pos = json_string.find('\n');
  ASSERT_EQ(std::string::npos, pos);
}

TEST(TestJsonVaue, NonValidRef) {
  using namespace utils::json;

  ASSERT_FALSE(JsonValueRef().IsValid());

  JsonValue::ParseResult parse_result = JsonValue::Parse(kTestJsonObjectString);
  ASSERT_TRUE(parse_result.second);

  const JsonValue& root_json = parse_result.first;
  ASSERT_EQ(kTestJsonObjectSize, root_json.Size());

  const JsonValueRef valid_ref = GetItem(root_json, "ObjVal");
  ASSERT_TRUE(valid_ref.IsValid());
  ASSERT_TRUE(valid_ref.IsObject());

  const JsonValueRef invalid_ref = GetItem(root_json, "NonExistentKey");
  ASSERT_FALSE(invalid_ref.IsValid());
}

#if defined(QT_PORT)

namespace {
	inline QVariant Parse(const std::string& json) {
		QJsonParseError parsing_error;
		QJsonDocument json_document =
			QJsonDocument::fromJson(json.c_str(), &parsing_error);
		if (parsing_error.error != QJsonParseError::NoError) {
			return QVariant();
		}

		return json_document.toVariant();
	}

	template <typename T>
	T& GetValueRef(QVariant& variant) {
		// If the variant stores a T, return it.
		if (variant.userType() == qMetaTypeId<T>()) {
			return *reinterpret_cast<T*>(variant.data());
		}

		// Some value to return, if the variant does not store a T.
		static T defaultValue;
		return defaultValue;
	}

	template <typename T>
	const T& GetValueRef(const QVariant& variant) {
		// If the variant stores a T, return it.
		if (variant.userType() == qMetaTypeId<T>()) {
			return *reinterpret_cast<const T*>(variant.constData());
		}

		// Some value to return, if the variant does not store a T.
		static T defaultValue;
		// Return the default value. Or add an assertion here. Or throw
		// an exception etc.
		return defaultValue;
	}

	inline std::string ToJsonString(const QVariant& json) {
		QJsonDocument json_document;

		if (json.canConvert<QVariantMap>()) {
			QJsonObject json_object =
				QJsonObject::fromVariantMap(GetValueRef<QVariantMap>(json));
			json_document.setObject(json_object);
			return json_document.toJson().toStdString();
		} else if (json.canConvert<QVariantList>()) {
			QJsonArray json_array =
				QJsonArray::fromVariantList(GetValueRef<QVariantList>(json));
			json_document.setArray(json_array);
			return json_document.toJson().toStdString();
		}
		return std::string();
	}

    std::pair<double, bool> GetIntegralPart(double value) {
      double integral_part = 0.;
      double fractional_part = std::modf(value, &integral_part);
      if (fractional_part <= std::numeric_limits<double>::epsilon()) {
        return std::make_pair(integral_part, true);
      }
      return std::make_pair(integral_part, false);
    }

    bool IsInt(double value) {
      std::pair<double, bool> int_part_result = GetIntegralPart(value);
      if (!int_part_result.second) {
        return false;
      }
      if (int_part_result.first < 0) {
        return true;
      }
      if (int_part_result.first <= std::numeric_limits<Int>::max()) {
        return true;
      }
      return false;
    }

    bool IsUInt(double value) {
      std::pair<double, bool> int_part_result = GetIntegralPart(value);
      if (!int_part_result.second) {
        return false;
      }
      if (int_part_result.first < 0) {
        return false;
      }
      if (int_part_result.first <= std::numeric_limits<UInt>::max()) {
        return true;
      }
      return false;
    }
} // namespace

TEST(TestQJsonArray, ReadWriteArray) {
  using namespace utils::json;
  QVariant json_root = Parse(kTestJsonArrayString);
  ASSERT_TRUE(json_root.canConvert<QVariantList>());
  QVariantList& json_root_list = GetValueRef<QVariantList>(json_root);
  ASSERT_EQ(kTestJsonArraySize, json_root_list.size());
  QVariant& int_item = json_root_list[1];
  ASSERT_EQ(5, int_item.toInt());
  int_item = 88888;
  QVariant& int_item_new = json_root_list[1];
  ASSERT_EQ(88888, int_item_new.toInt());
  json_root_list.append("Appended value");
  ASSERT_EQ(kTestJsonArraySize + 1, json_root_list.size());
  ASSERT_EQ(QString("Appended value"), json_root_list[2].toString());
}

TEST(TestQJsonObject, ReadWriteObject) {
  using namespace utils::json;
  QVariant json_root = Parse(kTestJsonObjectString);
  ASSERT_TRUE(json_root.canConvert<QVariantMap>());
  // Test simple object
  QVariantMap& json_root_map = GetValueRef<QVariantMap>(json_root);
  ASSERT_EQ(kTestJsonObjectSize, json_root_map.size());
  ASSERT_EQ(QString("StrObjVal"), json_root_map["StrVal"].toString());
  QVariant& str_obj = json_root_map["StrVal"];
  str_obj = "SomeNewStringValue";
  ASSERT_EQ(QString("SomeNewStringValue"), json_root_map["StrVal"].toString());
  // Test complext object
  QVariantMap& obj = GetValueRef<QVariantMap>(json_root_map["ObjVal"]);
  for (QVariantMap::iterator itr = obj.begin(); itr != obj.end(); ++itr) {
    QVariant::Type item_type = itr.value().type();
    // Json stores int values as double
    if (item_type == QVariant::Double) {
      itr.value() = 777788;
    } else if (item_type == QVariant::String) {
      itr.value() = "SomeNewStringValue";
    }
  }
}

TEST(TestQJsonObject, DoubleIsNotIntegral) {
  QVariant variant(QVariant::Double);
  ASSERT_EQ(QVariant::Double, variant.type());
  variant.setValue(123.0000000001);
  ASSERT_FALSE(GetIntegralPart(variant.toDouble()).second);
  variant.setValue(double(13) * 2 / 3);
  ASSERT_FALSE(GetIntegralPart(variant.toDouble()).second);
  variant.setValue(12.1);
  ASSERT_FALSE(GetIntegralPart(variant.toDouble()).second);
  variant.setValue(12.9);
  ASSERT_FALSE(GetIntegralPart(variant.toDouble()).second);
  variant.setValue(double(1) / 3);
  ASSERT_FALSE(GetIntegralPart(variant.toDouble()).second);
}

TEST(TestQJsonObject, DoubleIsIntegral) {
  QVariant variant(QVariant::Double);
  ASSERT_EQ(QVariant::Double, variant.type());
  variant.setValue(123.);
  ASSERT_TRUE(GetIntegralPart(variant.toDouble()).second);
  variant.setValue(double(12) / 2);
  ASSERT_TRUE(GetIntegralPart(variant.toDouble()).second);
}

TEST(TestQJsonObject, DoubleIsInt) {
  QVariant variant(QVariant::Double);
  ASSERT_EQ(QVariant::Double, variant.type());
  variant.setValue(123.);
  ASSERT_TRUE(IsInt(variant.toDouble()));
  variant.setValue(-123.);
  ASSERT_TRUE(IsInt(variant.toDouble()));
  variant.setValue(double(12) / 2);
  ASSERT_TRUE(IsInt(variant.toDouble()));
  variant.setValue(double(std::numeric_limits<Int>::max()) + 5);
  ASSERT_FALSE(IsInt(variant.toDouble()));
}

TEST(TestQJsonObject, ZeroIsInt) {
  QVariant variant(QVariant::Double);
  ASSERT_EQ(QVariant::Double, variant.type());
  variant.setValue(0.);
  ASSERT_TRUE(IsInt(variant.toDouble()));
}

TEST(TestQJsonObject, DoubleIsUInt) {
  QVariant variant(QVariant::Double);
  ASSERT_EQ(QVariant::Double, variant.type());
  variant.setValue(-123.);
  ASSERT_FALSE(IsUInt(variant.toDouble()));
  variant.setValue(double(12) / 2);
  ASSERT_TRUE(IsUInt(variant.toDouble()));
  variant.setValue(double(std::numeric_limits<Int>::max() + 5));
  ASSERT_TRUE(IsUInt(variant.toDouble()));
}
#endif

}  // namespace utils_test
}  // namespace components
}  // namespace test
