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

#include <sstream>
#include "gmock/gmock.h"
#include "smart_objects/smart_object.h"

//#define NO_INCLUSIVE_MAPS
//#define COPY_SUB_OBJECTS_WORKAROUND

namespace test {
namespace components {
namespace SmartObjects {
namespace SmartObjectStressTest {

using namespace NsSmartDeviceLink::NsSmartObjects;

class StressTestHelper : public ::testing::Test {
 private:
  char get_random_char() const {
    return static_cast<char>('A' + (rand() % 52));
  }

  std::string to_string(const int value) const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
  }

  std::string to_string(const double value) const {
    // Content is the same as in SmartObject::convert_double_to_string
    std::stringstream ss;
    ss << std::fixed << std::setprecision(10) << value;  //convert double to string w fixed notation, hi precision
    std::string s = ss.str();                            //output to std::string
    s.erase(s.find_last_not_of('0') + 1, std::string::npos);  //remove trailing 000s    (123.1200 => 123.12,  123.000 => 123.)
    if (s[s.size() - 1] == '.') {
      s.erase(s.end() - 1);              //remove dangling decimal (123. => 123)
    }
    return s;
  }

  std::string to_string(const char ch) const {
    char buff[2];
    sprintf(buff, "%c", ch);
    return std::string(buff);
  }

  std::string to_string(const bool b) const {
    return std::string((b) ? "true" : "false");
  }

 protected:
  typedef std::map<std::string, std::string> VerificationMap;
  VerificationMap mVerifyMap;

  std::vector<std::string> split(const std::string &s, char delim) const {
    std::vector < std::string > elems;

    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }

    return elems;
  }

  std::string generate_key(const char *pPrefix, const int index) const {
    char buff[32];
    sprintf(buff, "%s%d", pPrefix, index);
    return std::string(buff);
  }

  void makeRandomObject(SmartObject &obj, const int size,
                        std::string key_path) {
    int type_id = rand() % 8;

    switch (type_id) {
      case 0:     // int
      {
        int iVal = rand();
        obj = iVal;
        mVerifyMap[key_path] = to_string(iVal);
        //std::cout << "Created int, value: " << iVal << std::endl;
        break;
      }
      case 1:     // bool
      {
        bool bVal = static_cast<bool>(rand() % 2);
        obj = bVal;
        mVerifyMap[key_path] = to_string(bVal);
        //std::cout << "Created bool, value: " << to_string(bVal) << std::endl;
        break;
      }
      case 2:     // double
      {
        double dVal = 100.0 / (rand() % 200);
        obj = dVal;
        mVerifyMap[key_path] = to_string(dVal);
        //std::cout << "Created double, value: " << dVal << std::endl;
        break;
      }
      case 3:     // char
      {
        char cVal = get_random_char();
        obj = cVal;
        mVerifyMap[key_path] = to_string(cVal);
        //std::cout << "Created char, value: " << cVal << std::endl;
        break;
      }
      case 4:     // string
      {
        std::string strVal(rand() % 200, get_random_char());
        obj = strVal;   // string with random char filled random size
        mVerifyMap[key_path] = strVal;
        //std::cout << "Created string, value: " << strVal << std::endl;
        break;
      }
      case 5:     // map
        if (size <= 0)
          break;

        //std::cout << "Creating a map with size: " << size << std::endl;
        mVerifyMap[key_path] = "map";
        for (int i = 0; i < size; i++) {
          std::string key = generate_key("M", i);
#ifdef NO_INCLUSIVE_MAPS
          obj[key] = key;
#else
          obj[key] = SmartObject();
          makeRandomObject(obj[key], size - 1, key_path + key + ' ');  // recursion
#endif // MAP_WORKAROUND
        }
        break;
      case 6:     // array
        if (size <= 0)
          break;

        //std::cout << "Creating an array with size: " << size << std::endl;
        mVerifyMap[key_path] = "array";
        for (int i = 0; i < size; i++) {
          obj[i] = SmartObject();      // just init it as an array
          makeRandomObject(obj[i], size - 1,
                           key_path + generate_key("A", i) + ' ');  // recursion
        }
        break;
      case 7:     // binary
        int dataSize = rand() % 200;
        char randomChar = get_random_char();
        std::string strDataVal(dataSize, randomChar);
        std::string strVal("c:");
        strVal += strDataVal;

        NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryVal(dataSize,
                                                                 randomChar);

        obj = binaryVal;   // string with binary data filled with random chars
        mVerifyMap[key_path] = strVal;
        //std::cout << "Created string, value: " << strVal << std::endl;
        break;
    }
  }

  SmartObject get_object(SmartObject &rootObj, const std::string &path) const {
    std::vector < std::string > obj_tokens;
    SmartObject lastObj = rootObj;

    obj_tokens = split(path, ' ');

    for (size_t i = 0; i < obj_tokens.size(); i++) {
      if (obj_tokens[i][0] == 'A')            // array
          {
        int index = atoi(&(obj_tokens[i].c_str()[1]));  // get integer skipping first char
#ifdef COPY_SUB_OBJECTS_WORKAROUND
            lastObj = SmartObject(lastObj[index]);
#else
        lastObj = lastObj[index];       // go to the child object
#endif
      } else if (obj_tokens[i][0] == 'M')       // map
          {
#ifdef COPY_SUB_OBJECTS_WORKAROUND
        lastObj = SmartObject(lastObj[obj_tokens[i]]);
#else
        lastObj = lastObj[obj_tokens[i]];       // go to the child object
#endif
      } else {
        //FAIL();
        EXPECT_TRUE(false);
      }
    }
    return lastObj;
  }
};

/*
 * The test creates the initial SmartObject and use it as an array for the next SmartObjects.
 * Each next SmartObject is randomly assigned to some type.
 * If one of the object happens to be a container it fills it with SmartObject of random type. The amount of these
 * objects is the size of the parent container -1.
 * The iteration continues until all nodes are simple SmartObjects (not arrays or maps)
 */
TEST_F(StressTestHelper, StressTest) {
  SmartObject objects;

  const int size = 11;

  for (int i = 0; i < size; i++) {
    SmartObject obj;

    makeRandomObject(obj, size - 1, generate_key("A", i) + ' ');

    objects[i] = obj;
  }

  for (VerificationMap::const_iterator it = mVerifyMap.begin();
      it != mVerifyMap.end(); it++) {
    std::string value(it->second);
    SmartObject obj = get_object(objects, it->first);

    // Binary data check
    if (!value.compare(0, 2, "c:")) {
      std::string etalonData = value.substr(2);

      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::SmartType_Binary,
                obj.getType());

      NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData =
          obj.asBinary();
      ASSERT_EQ(etalonData.size(), binaryData.size());

      for (size_t i = 0; i < etalonData.size(); ++i) {
        {
          std::string etalonData = value.substr(2);

          ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::SmartType_Binary,
                    obj.getType());

          NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData = obj
              .asBinary();
          ASSERT_EQ(etalonData.size(), binaryData.size());

          for (size_t i = 0; i < etalonData.size(); ++i) {
            ASSERT_EQ(etalonData.at(i), binaryData.at(i));
          }
          continue;
        }

        ASSERT_EQ(etalonData.at(i), binaryData.at(i));
      }
      continue;
    }

#ifdef NO_INCLUSIVE_MAPS
    if (!value.compare("map"))
    {
      std::vector<std::string> path = split(it->first, ' ');

      std::string map_value = path[path.size()-1];
      ASSERT_EQ(map_value, static_cast<std::string>(obj));
      continue;
    }
#endif
    if (value.compare("map") && value.compare("array")) {
      //std::cout << "Verification key: " << it->first << " Value: " << value << std::endl;
      //std::cout << "Object Value: " << static_cast<std::string>(obj) << std::endl;

      if (!value.compare("true")) {
        ASSERT_TRUE(obj.asBool());
      } else if (!value.compare("false")) {
        ASSERT_FALSE(obj.asBool());
      } else {
        ASSERT_EQ(value, obj.asString())<< "Object value is not correct. Object path: " << it->first;
      }
    }
  }
}

TEST_F(StressTestHelper, ExtraManualDebugTest) {
  SmartObject obj;

  obj[0] = false;
  obj[1] = 0.869495;
  obj[2] = true;
  obj[3] = 'Q';
  obj[4] = true;
  obj[5] = 3.704;
  obj[6] = SmartObject();
  obj[6][0] =
      std::string(
          "ttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt");
  obj[6][1] = 'K';
  obj[6][2] = 0.735294;
  obj[6][3] = 'I';
  obj[6][4] = SmartObject();
  obj[6][4]["M0"] = 0.59432;
  SmartObject & refObj = obj[6][4];
  refObj["M1"]["M0"]["M0"][0] = true;

  // FIXME: Figure out why there's a trailing zero while converting from double to string
  ASSERT_EQ("0.59432", get_object(obj, "A6 A4 M0").asString());
  ASSERT_TRUE(get_object(obj, "A6 A4 M1 M0 M0 A0").asBool());
}

}
}
}
}
