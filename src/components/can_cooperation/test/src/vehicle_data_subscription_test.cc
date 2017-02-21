/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "json/json.h"

namespace can_cooperation {

TEST(VehicleDataSubscription, CompareSuccess) {
  std::string json_str1 =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  std::string json_str2 =
      "{\"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1}, \"moduleType\": \"CLIMATE\" }},\
                         \"jsonrpc\": \"2.0\"}";

  Json::Value json1;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_str1, json1, false));
  Json::Value json2;
  ASSERT_TRUE(reader.parse(json_str1, json2, false));

  ASSERT_FALSE(json1 < json2 || json2 < json1);

  ASSERT_TRUE(reader.parse(json_str2, json2, false));

  ASSERT_FALSE(json1 < json2 || json2 < json1);

  std::set<Json::Value> subscribed;
  subscribed.insert(json1["params"]["moduleData"]);
  subscribed.insert(json2["params"]["moduleData"]);

  ASSERT_FALSE(subscribed.end() ==
               subscribed.find(json2["params"]["moduleData"]));
}

TEST(VehicleDataSubscription, CompareNull) {
  std::string json_str1 =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  Json::Value json1;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_str1, json1, false));
  Json::Value json2(Json::ValueType::nullValue);

  ASSERT_FALSE(json1 < json2);
  //  -- Pay attention: null value is less then object:
  //  doesn't work: ASSERT_FALSE(json2 < json1);
}

TEST(VehicleDataSubscription, CompareDifferentTypes) {
  std::string json_str1 =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  std::string json_str2 = "\"method\": \"RC.OnInteriorVehicleData\"";

  Json::Value json1;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_str1, json1, false));
  ASSERT_TRUE(json1.type() == Json::ValueType::objectValue);
  Json::Value json2;
  ASSERT_TRUE(reader.parse(json_str2, json2, false));
  ASSERT_TRUE(json2.type() == Json::ValueType::stringValue);

  ASSERT_FALSE(json1 < json2);
  //  -- Pay attention: string type is less than object
  // doesn't work: ASSERT_FALSE(json2 < json1);
}

TEST(VehicleDataSubscription, CompareDifferentNesting) {
  std::string json_str1 =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  std::string json_str2 =
      "{\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}";

  Json::Value json1;
  Json::Reader reader;
  ASSERT_TRUE(reader.parse(json_str1, json1, false));
  Json::Value json2;
  ASSERT_TRUE(reader.parse(json_str2, json2, false));

  ASSERT_TRUE(json2 < json1);
}

TEST(VehicleDataSubscription, CompareModuleDesciption) {
  Json::Value json1;
  json1["moduleDescription"] = Json::Value(Json::ValueType::objectValue);
  json1["moduleDescription"]["moduleType"] = "RADIO";
  json1["moduleDescription"]["moduleZone"] =
      Json::Value(Json::ValueType::objectValue);
  json1["moduleDescription"]["moduleZone"]["col"] = 0;
  json1["moduleDescription"]["moduleZone"]["row"] = 0;
  json1["moduleDescription"]["moduleZone"]["level"] = 0;
  json1["moduleDescription"]["moduleZone"]["colspan"] = 2;
  json1["moduleDescription"]["moduleZone"]["rowspan"] = 2;
  json1["moduleDescription"]["moduleZone"]["levelspan"] = 1;

  Json::Value json2;
  json2["moduleData"] = Json::Value(Json::ValueType::objectValue);
  json2["moduleData"]["radioControlData"] =
      Json::Value(Json::ValueType::objectValue);
  json2["moduleData"]["moduleZone"] = Json::Value(Json::ValueType::objectValue);
  json2["moduleData"]["moduleZone"]["colspan"] = 2;
  json2["moduleData"]["moduleZone"]["rowspan"] = 2;
  json2["moduleData"]["moduleZone"]["levelspan"] = 1;
  json2["moduleData"]["moduleZone"]["col"] = 0;
  json2["moduleData"]["moduleZone"]["row"] = 0;
  json2["moduleData"]["moduleZone"]["level"] = 0;
  json2["moduleData"]["moduleType"] = "RADIO";

  ASSERT_FALSE(json1["moduleDescription"] == json2["moduleData"]);

  Json::Value json3;
  json3["moduleZone"] = json2["moduleData"]["moduleZone"];
  json3["moduleType"] = json2["moduleData"]["moduleType"];

  ASSERT_TRUE(json1["moduleDescription"] == json3);

  std::set<Json::Value> subscribed;
  subscribed.insert(json1["moduleDescription"]);
  Json::Value null_val(Json::ValueType::nullValue);
  subscribed.insert(null_val);
  ASSERT_FALSE(subscribed.end() == subscribed.find(json3));

  ASSERT_FALSE(subscribed.end() == subscribed.find(Json::Value()));
}

}  // namespace can_cooperation
