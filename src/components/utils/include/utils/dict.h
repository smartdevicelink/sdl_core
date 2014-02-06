/**
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_H_

#include <map>
#include <string>

namespace utils {

class Dictionary {
 public:
  typedef std::string Key;
  typedef std::string Record;
 private:
  typedef std::map<Key, Record> ItemContainer;
  typedef std::map<Key, Dictionary> SubitemContainer;
 public:
  typedef SubitemContainer::iterator iterator;
  typedef SubitemContainer::const_iterator const_iterator;
 private:
  ItemContainer items_;
  SubitemContainer subitems_;
 public:
  void AddItem(const Key& key, const Record& record);
  void AddSubitem(const Key& key, const Dictionary& subitem);

  Record& ItemAt(const Key& key);
  const Record& ItemAt(const Key& key) const;

  Dictionary& SubitemAt(const Key& key);
  const Dictionary& SubitemAt(const Key& key) const;

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;
};

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_H_
