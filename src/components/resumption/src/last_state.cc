/*
 * Copyright (c) 2013, Ford Motor Company
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

#include "resumption/last_state.h"
#include "utils/file_system.h"

namespace resumption {


Json::Value LastState::toJson(const Dictionary& dict) {
  Json::Value val;
  Dictionary::const_iterator it;
  Dictionary::rec_const_iterator rec_it;

  it = dict.begin();
  rec_it = dict.rec_begin();

  for ( ; rec_it != dict.rec_end() ;++rec_it) {
    std::string key = rec_it->first;
    std::string value = rec_it->second;
    val[key] = value;
  }
  for ( ; it != dict.end() ;++it) {
    std::string key = it->first;
    const Dictionary& value = it->second;
    val[key] = toJson(value);
  }

  return val;
}

LastState::Dictionary LastState::fromJson(const Json::Value& json_val) {
  Dictionary result;

  Json::Value::Members members = json_val.getMemberNames();
  Json::Value::Members::const_iterator it = members.begin();
  for ( ; it != members.end(); ++it) {
    std::string key = (*it);
    const Json::Value& value = json_val[key];
    if (value.isString()) {
      result.AddItem(key,value.asString());
    } else if(value.isObject()) {
      Dictionary sub_item = fromJson(value);
      result.AddSubitem(key, sub_item);
    } else {
      NOTREACHED();
    }
  }

  return result;
}

void LastState::SaveToFileSystem() {
  const Json::Value& val = toJson(dictionary);
  const std::string& str = val.toStyledString();
  const std::vector<uint8_t> char_vector_pdata(
    str.begin(), str.end());
  DCHECK(file_system::Write(filename, char_vector_pdata));
}

void LastState::LoadFromFileSystem() {
  std::string buffer;
  bool result = file_system::ReadFile(filename,buffer);
  if (result) {
    Json::Reader m_reader;
    Json::Value val;
    DCHECK(m_reader.parse(buffer,val));
    dictionary = fromJson(val);
  } else {
    // Error
  }
}

LastState::LastState() {
  LoadFromFileSystem();
}

LastState::~LastState() {
  SaveToFileSystem();
}

const std::string LastState::filename = "TM.dat";
}
