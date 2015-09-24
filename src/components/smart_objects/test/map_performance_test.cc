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
#include <vector>
#include <map>

#include "gmock/gmock.h"

typedef std::string Key;
typedef std::vector<std::string> Value;
typedef std::map<Key, Value> Map;

namespace {
void MakeMapObject(Map &obj, const int size) {
  char i_key[8], j_key[8];

  Value array;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      sprintf(j_key, "j_%d", j);
      array.push_back(j_key);
    }

    sprintf(i_key, "i_%d", i);
    obj[i_key] = array;
  }
}
}

TEST(SmartObjectPerformanceTest, SmartObjectMapPerformance) {
  Map object;
  MakeMapObject(object, 100);

  for (Map::const_iterator i = object.begin(); i != object.end(); ++i) {
    printf("%s - ", i->first.c_str());
    const Value& value = i->second;

    Value::const_iterator item = std::find(value.begin(), value.end(), "j_9");
    if (item != value.end()) {
      printf("%s\n", item->c_str());
    } else {
      printf("none...\n");
    }
  }
}
