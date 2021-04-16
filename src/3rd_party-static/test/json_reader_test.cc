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
#include "utils/jsoncpp_reader_wrapper.h"

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

  Json::Value json;
public:
  bool Parse(const std::string &str);
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

bool JSONReaderTest::Parse(const std::string &str)
{
  utils::JsonReader reader;
  return reader.parse(str, &json);
}

TEST_F(JSONReaderTest, Parse_NormalString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(normalString_));
}

TEST_F(JSONReaderTest, Parse_BackspaceString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(backspaceString_));
}

TEST_F(JSONReaderTest, Parse_FormFeedString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(formfeedString_));
}

TEST_F(JSONReaderTest, Parse_LineFeedString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(linefeedString_));
}

TEST_F(JSONReaderTest, ParseReturnString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(returnString_));
}

TEST_F(JSONReaderTest, Parse_TabString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(tabString_));
}

TEST_F(JSONReaderTest, Parse_BellString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(bellString_));
}

TEST_F(JSONReaderTest, Parse_QuoteString_ExpectUnsuccessfulParsing) {
  ASSERT_FALSE(Parse(quoteString_));
}

TEST_F(JSONReaderTest, Parse_SlashString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(slashString_));
}

TEST_F(JSONReaderTest, Parse_BackslashString_ExpectUnsuccessfulParsing) {
  ASSERT_FALSE(Parse(backslashString_));
}

TEST_F(JSONReaderTest, Parse_SinglequoteString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(singlequoteString_));
}

TEST_F(JSONReaderTest, Parse_ColonString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(colonString_));
}

TEST_F(JSONReaderTest, Parse_ComaString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(comaString_));
}

TEST_F(JSONReaderTest, Parse_OpeningbraceString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(openingbraceString_));
}

TEST_F(JSONReaderTest, Parse_ClosingbraceString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(closingbraceString_));
}

TEST_F(JSONReaderTest, Parse_OpeningbracketString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(openingbracketString_));
}

TEST_F(JSONReaderTest, Parse_ClosingbracketString_ExpectSuccessfulParsing) {
  ASSERT_TRUE(Parse(closingbracketString_));
}

}  //  namespace json_reader_test
}  //  namespace components
}  //  namespace test
