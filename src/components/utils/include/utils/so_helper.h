/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SO_HELPER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SO_HELPER_H_

#include "smart_objects/smart_object.h"

/**
 * @brief Structure for representing of TTS SO for checks below
 **/
struct TTS {
  explicit TTS(const smart_objects::SmartObject *tts)
  :params_(tts)
  {};

  const smart_objects::SmartObject *params_;
};

/**
 * @brief Structure for representing of VR synonyms SO for checks below
 **/
struct VR {
  explicit VR(const smart_objects::SmartObject *vr)
  :params_(vr)
  {};

  const smart_objects::SmartObject *params_;
};

/*
 * @brief Check for coincidence of parameters in VR synonyms, TTS and application name
 *
 * return TRUE if no coincidence of app. name, TTS and VR syn, otherwise FALSE
*/
template < typename T, typename U>
bool checkCoincidence(const T &knownItem, const U &newItem);

template<>
bool checkCoincidence<std::string, TTS>(const std::string &knownItem, const TTS &newItem) {
  if (NULL == newItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    if (knownItem == newItem.params_->getElement(i)[application_manager::strings::text].asString()) {
      return true;
    }
  }

  return false;
};

template<>
bool checkCoincidence<std::string, VR>(const std::string &knownItem, const VR &newItem) {
  if (NULL == newItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    if (knownItem == newItem.params_->getElement(i).asString()) {
      return true;
    }
  }

  return false;
};

template<>
bool checkCoincidence<TTS, VR>(const TTS &knownItem, const VR &newItem) {
  if (    NULL == newItem.params_
      ||  NULL == knownItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    for(size_t j = 0; j != knownItem.params_->length(); ++j) {
      if (knownItem.params_->getElement(i)[application_manager::strings::text]
             == newItem.params_->getElement(j)) {

        return true;
      }
    }
  }

  return false;
};

template<>
bool checkCoincidence<VR, TTS>(const VR &knownItem, const TTS &newItem) {
  if (    NULL == newItem.params_
      ||  NULL == knownItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    for(size_t j = 0; j != knownItem.params_->length(); ++j) {
      if (knownItem.params_->getElement(i)
            == newItem.params_->getElement(j)[application_manager::strings::text]) {

        return true;
      }
    }
  }

  return false;
}

template<>
bool checkCoincidence<TTS, TTS>(const TTS &knownItem, const TTS &newItem) {
  if (    NULL == newItem.params_
      ||  NULL == knownItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    for(size_t j = 0; j != knownItem.params_->length(); ++j) {
      if (knownItem.params_->getElement(i)[application_manager::strings::text]
             == newItem.params_->getElement(j)[application_manager::strings::text]) {

        return true;
      }
    }
  }

  return false;
}

template<>
bool checkCoincidence<VR, VR>(const VR &knownItem, const VR &newItem) {
  if (    NULL == newItem.params_
      ||  NULL == knownItem.params_) {
    return false;
  }
  for (size_t i = 0; i != newItem.params_->length(); ++i) {
    for(size_t j = 0; j != knownItem.params_->length(); ++j) {
      if (knownItem.params_->getElement(i) == newItem.params_->getElement(j)) {

        return true;
      }
    }
  }

  return false;
}

#endif /* SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SO_HELPER_H_ */
