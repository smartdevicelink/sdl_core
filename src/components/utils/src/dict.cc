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

#include "utils/dict.h"

namespace utils {

void Dictionary::AddItem(const Key& key, const Record& record) {
  items_.insert(std::make_pair(key, record));
}

void Dictionary::AddSubitem(const Key& key, const Dictionary& subitem) {
  subitems_.insert(std::make_pair(key, subitem));
}

Dictionary::Record& Dictionary::ItemAt(const Key& key) {
  return items_.at(key);
}

const Dictionary::Record& Dictionary::ItemAt(const Key& key) const {
  return items_.at(key);
}

Dictionary& Dictionary::SubitemAt(const Key& key) {
  return subitems_.at(key);
}

const Dictionary& Dictionary::SubitemAt(const Key& key) const {
  return subitems_.at(key);
}

Dictionary::iterator Dictionary::begin() {
  return subitems_.begin();
}

Dictionary::const_iterator Dictionary::begin() const {
  return subitems_.begin();
}

Dictionary::iterator Dictionary::end() {
  return subitems_.end();
}

Dictionary::const_iterator Dictionary::end() const {
  return subitems_.end();
}

}  // namespace utils
