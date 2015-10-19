/**
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
#include "json/reader.h"

namespace test {
namespace third_party_libs {
namespace json_reader_test {


class JSONReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp();
  virtual void TearDown() {}

  std::string normalString_;
  std::string backspaceString_;
  std::string formfeedString_;
  std::string linefeedString_;
  std::string returnString_;
  std::string tabString_;
  std::string bellString_;
  std::string quoteString_;
  std::string slashString_;
  std::string backslashString_;
  std::string singlequoteString_;
  std::string openingbraceString_;
  std::string closingbraceString_;
  std::string openingbracketString_;
  std::string closingbracketString_;
  std::string comaString_;
  std::string colonString_;

  Json::Reader reader;
  Json::Value json;

};

void JSONReaderTest::SetUp()
{
  normalString_ = "{\"field\" : \"value\" }";
  backspaceString_ = "{\"field\" : \"va\blue\" }";
  formfeedString_ = "{\"field\" : \"va\flue\" }";
  linefeedString_ = "{\"field\" : \"va\nlue\" }";
  returnString_ = "{\"field\" : \"va\rlue\" }";
  tabString_ = "{\"field\" : \"va\tlue\" }";
  bellString_ = "{\"field\" : \"va\alue\" }";
  quoteString_ = "{\"field\" : \"va\"lue\" }";
  slashString_ = "{\"field\" : \"va/lue\" }";
  backslashString_ = "{\"field\" : \"va\\lue\" }";
  singlequoteString_ = "{\"field\" : \"va\'lue\" }";
  openingbraceString_ = "{\"field\" : \"va{lue\" }";
  closingbraceString_ = "{\"field\" : \"va}lue\" }";
  openingbracketString_ = "{\"field\" : \"va[lue\" }";
  closingbracketString_ = "{\"field\" : \"va]lue\" }";
  comaString_ = "{\"field\" : \"va,lue\" }";
  colonString_ = "{\"field\" : \"va:lue\" }";
}

TEST_F(JSONReaderTest, ParseNormalString) {
  ASSERT_TRUE(reader.parse(normalString_, json));
}

TEST_F(JSONReaderTest, ParseBackspaceString) {
  ASSERT_TRUE(reader.parse(backspaceString_, json));
}

TEST_F(JSONReaderTest, ParseFormFeedString) {
  ASSERT_TRUE(reader.parse(formfeedString_, json));

}

TEST_F(JSONReaderTest, ParseLineFeedString) {
  ASSERT_TRUE(reader.parse(linefeedString_, json));
}

TEST_F(JSONReaderTest, ParseReturnString) {
  ASSERT_TRUE(reader.parse(returnString_, json));
}

TEST_F(JSONReaderTest, ParseTabString) {
  ASSERT_TRUE(reader.parse(tabString_, json));
}

TEST_F(JSONReaderTest, ParseBellString) {
  ASSERT_TRUE(reader.parse(bellString_, json));
}

TEST_F(JSONReaderTest, ParseQuoteString) {
  ASSERT_FALSE(reader.parse(quoteString_, json));
}

TEST_F(JSONReaderTest, ParseSlashString) {
  ASSERT_TRUE(reader.parse(slashString_, json));
}

TEST_F(JSONReaderTest, ParseBackslashString) {
  ASSERT_FALSE(reader.parse(backslashString_, json));
}

TEST_F(JSONReaderTest, ParseSinglequoteString) {
  ASSERT_TRUE(reader.parse(singlequoteString_, json));
}

TEST_F(JSONReaderTest, ParseColonString) {
  ASSERT_TRUE(reader.parse(colonString_, json));
}

TEST_F(JSONReaderTest, ParseComaString) {
  ASSERT_TRUE(reader.parse(comaString_, json));
}

TEST_F(JSONReaderTest, ParseOpeningbraceString) {
  ASSERT_TRUE(reader.parse(openingbraceString_, json));
}

TEST_F(JSONReaderTest, ParseClosingbraceString) {
  ASSERT_TRUE(reader.parse(closingbraceString_, json));
}

TEST_F(JSONReaderTest, ParseOpeningbracketString) {
  ASSERT_TRUE(reader.parse(openingbracketString_, json));
}
TEST_F(JSONReaderTest, ParseClosingbracketString) {
  ASSERT_TRUE(reader.parse(closingbracketString_, json));
}

}  //  namespace json_reader_test
}  //  namespace components
}  //  namespace test
