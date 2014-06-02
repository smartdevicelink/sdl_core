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

namespace utils {

template<typename Key, typename Record>
class Dictionary {
 private:
  typedef std::map<Key, Record> ItemContainer;
  typedef std::map<Key, Dictionary> SubitemContainer;

 public:
/**
 * @brief Typedef for subitems iterator
 */
  typedef typename SubitemContainer::iterator iterator;
/**
  * @brief Typedef for subitems const iterator
  */
  typedef typename SubitemContainer::const_iterator const_iterator;

/**
  * @brief Typedef for items iterator
  */
  typedef typename ItemContainer::iterator rec_iterator;
/**
  * @brief Typedef for items const iterator
  */
  typedef typename ItemContainer::const_iterator rec_const_iterator;

/**
 * @brief add an item to the dictionary
 * @param key - item key
 * @param record - item value
 */
  void AddItem(const Key& key, const Record& record);
/**
 * @brief add a subitem to the dictionary
 * @param key - subitem key
 * @param subitem - subitem dictionary
 */
  void AddSubitem(const Key& key, const Dictionary& subitem);
/**
 * @brief item under specified key
 * @param key - item key
 * @return item with specified key of default-constructed item
 */
  Record& ItemAt(const Key& key);
/**
 * @brief item under specified key
 * @param key - item key
 * @return item with specified key of default-constructed item
   */
  const Record& ItemAt(const Key& key) const;
/**
 * @brief subitem under specified key
 * @param key - subitem key
 * @return subitem with specified key of default-constructed dictionary
 */
  Dictionary& SubitemAt(const Key& key);
/**
 * @brief subitem under specified key
 * @param key - subitem key
 * @return subitem with specified key of default-constructed dictionary
 */
  const Dictionary& SubitemAt(const Key& key) const;
/**
 * @brief first subitem
 * @return iterator pointing to the first subitem
 */
  iterator begin();

/**
 * @brief first subitem
 * @return const iterator pointing to the first subitem
 */
  const_iterator begin() const;

/**
  * @brief first item
  * @return iterator pointing to the first item
  */
  rec_iterator rec_begin();

/**
  * @brief first item
  * @return iterator pointing to the first item
  */
  rec_const_iterator rec_begin() const;

/**
 * @brief end of subitem list
 * @return iterator pointing to the end of subitem list
 */
  iterator end();
/**
 * @brief end of subitem list
 * @return const iterator pointing to the end of subitem list
 */
  const_iterator end() const;

  /**
  * @brief end of item list
  * @return iterator pointing to the end of item list
  */
  rec_iterator rec_end();

  /**
  * @brief end of item list
  * @return const iterator pointing to the end of item list
  */
  rec_const_iterator rec_end() const;

private:
 ItemContainer items_;
 SubitemContainer subitems_;
};

template<typename Key, typename Record>
void Dictionary<Key, Record>::AddItem(const Key& key, const Record& record) {
  items_.insert(std::make_pair(key, record));
}

template<typename Key, typename Record>
void Dictionary<Key, Record>::AddSubitem(const Key& key, const Dictionary& subitem) {
  subitems_.insert(std::make_pair(key, subitem));
}

template<typename Key, typename Record>
Record& Dictionary<Key, Record>::ItemAt(const Key& key) {
  return items_.at(key);
}

template<typename Key, typename Record>
const Record& Dictionary<Key, Record>::ItemAt(const Key& key) const {
  return items_.at(key);
}

template<typename Key, typename Record>
Dictionary<Key, Record>& Dictionary<Key, Record>::SubitemAt(const Key& key) {
  return subitems_.at(key);
}

template<typename Key, typename Record>
const Dictionary<Key, Record>& Dictionary<Key, Record>::SubitemAt(const Key& key) const {
  return subitems_.at(key);
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::iterator Dictionary<Key, Record>::begin() {
  return subitems_.begin();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::const_iterator Dictionary<Key, Record>::begin() const {
  return subitems_.begin();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::rec_iterator Dictionary<Key, Record>::rec_begin() {
  return items_.begin();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::rec_const_iterator Dictionary<Key, Record>::rec_begin() const {
  return items_.begin();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::iterator Dictionary<Key, Record>::end() {
  return subitems_.end();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::const_iterator Dictionary<Key, Record>::end() const {
  return subitems_.end();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::rec_iterator Dictionary<Key, Record>::rec_end() {
  return items_.end();
}

template<typename Key, typename Record>
typename Dictionary<Key, Record>::rec_const_iterator Dictionary<Key, Record>::rec_end() const {
  return items_.end();
}

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_H_
