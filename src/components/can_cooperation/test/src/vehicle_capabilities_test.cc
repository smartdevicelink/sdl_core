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
#include "can_cooperation/vehicle_capabilities.h"

namespace can_cooperation {

TEST(VehicleCapabilities, Load) {
  VehicleCapabilities caps;
  ASSERT_EQ(Json::ValueType::arrayValue, caps.capabilities().type());
}

TEST(VehicleCapabilities, GetCapabilities) {
  VehicleCapabilities caps;
  ASSERT_EQ(Json::ValueType::arrayValue, caps.capabilities().type());
  ASSERT_TRUE(6 == caps.capabilities().size());
}

TEST(VehicleCapabilities, GetSpecificCapabilities) {
  VehicleCapabilities caps;
  Json::Value value;
  value["col"] = 0;
  value["row"] = 0;
  value["level"] = 0;
  value["colspan"] = 2;
  value["rowspan"] = 2;
  value["levelspan"] = 1;
  ASSERT_EQ(Json::ValueType::arrayValue, caps.capabilities(value).type());
  ASSERT_TRUE(2 == caps.capabilities(value).size());

  value["row"] = 1;
  ASSERT_EQ(Json::ValueType::arrayValue, caps.capabilities(value).type());
  ASSERT_TRUE(1 == caps.capabilities(value).size());
}

}  // namespace can_cooperation
