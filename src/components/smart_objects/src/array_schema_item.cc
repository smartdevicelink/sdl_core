// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "smart_objects/array_schema_item.h"
#include "smart_objects/smart_object.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {

std::shared_ptr<CArraySchemaItem> CArraySchemaItem::create(
    const ISchemaItemPtr ElementSchemaItem,
    const TSchemaItemParameter<size_t>& MinSize,
    const TSchemaItemParameter<size_t>& MaxSize) {
  return std::shared_ptr<CArraySchemaItem>(
      new CArraySchemaItem(ElementSchemaItem, MinSize, MaxSize));
}

errors::eType CArraySchemaItem::validate(
    const SmartObject& Object,
    rpc::ValidationReport* report__,
    const utils::SemanticVersion& MessageVersion,
    const bool allow_unknown_enums) {
  if (SmartType_Array != Object.getType()) {
    std::string validation_info = "Incorrect type, expected: " +
                                  SmartObject::typeToString(SmartType_Array) +
                                  ", got: " +
                                  SmartObject::typeToString(Object.getType());
    report__->set_validation_info(validation_info);
    return errors::INVALID_VALUE;
  }
  size_t sizeLimit;
  const size_t array_len = Object.length();

  if (mMinSize.getValue(sizeLimit) && (array_len < sizeLimit)) {
    std::stringstream stream;
    stream << "Got array of size: " << array_len
           << ", minimum allowed: " << sizeLimit;
    std::string validation_info = stream.str();
    report__->set_validation_info(validation_info);
    return errors::OUT_OF_RANGE;
  }
  if (mMaxSize.getValue(sizeLimit) && (array_len > sizeLimit)) {
    std::stringstream stream;
    stream << "Got array of size: " << array_len
           << ", maximum allowed: " << sizeLimit;
    std::string validation_info = stream.str();
    report__->set_validation_info(validation_info);
    return errors::OUT_OF_RANGE;
  }

  for (size_t i = 0u; i < array_len; ++i) {
    std::stringstream strVal;
    strVal << i;
    const errors::eType result =
        mElementSchemaItem->validate(Object.getElement(i),
                                     &report__->ReportSubobject(strVal.str()),
                                     MessageVersion,
                                     allow_unknown_enums);
    if (errors::OK != result) {
      return result;
    }
  }
  return errors::OK;
}

void CArraySchemaItem::applySchema(
    SmartObject& Object,
    const bool remove_unknown_parameters,
    const utils::SemanticVersion& MessageVersion) {
  if (SmartType_Array == Object.getType()) {
    for (size_t i = 0U; i < Object.length(); ++i) {
      mElementSchemaItem->applySchema(
          Object[i], remove_unknown_parameters, MessageVersion);
    }
  }
}

void CArraySchemaItem::unapplySchema(SmartObject& Object,
                                     const bool remove_unknown_parameters) {
  if (SmartType_Array == Object.getType()) {
    for (size_t i = 0U; i < Object.length(); ++i) {
      mElementSchemaItem->unapplySchema(Object[i], remove_unknown_parameters);
    }
  }
}

void CArraySchemaItem::BuildObjectBySchema(const SmartObject& pattern_object,
                                           SmartObject& result_object) {
  if (SmartType_Array == pattern_object.getType()) {
    const size_t array_len = pattern_object.length();
    if (array_len > 0) {
      for (size_t i = 0u; i < array_len; i++) {
        mElementSchemaItem->BuildObjectBySchema(pattern_object.getElement(i),
                                                result_object[i]);
      }
      return;
    }
  }
  // empty array
  result_object = SmartObject(SmartType_Array);
}

CArraySchemaItem::CArraySchemaItem(const ISchemaItemPtr ElementSchemaItem,
                                   const TSchemaItemParameter<size_t>& MinSize,
                                   const TSchemaItemParameter<size_t>& MaxSize)
    : mElementSchemaItem(ElementSchemaItem)
    , mMinSize(MinSize)
    , mMaxSize(MaxSize) {}

}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
