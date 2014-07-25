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

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

utils::SharedPtr<CArraySchemaItem> CArraySchemaItem::create(
    const ISchemaItemPtr ElementSchemaItem,
    const TSchemaItemParameter<size_t>& MinSize,
    const TSchemaItemParameter<size_t>& MaxSize) {
  return new CArraySchemaItem(ElementSchemaItem, MinSize, MaxSize);
}

Errors::eType CArraySchemaItem::validate(const SmartObject& Object) {
  if (SmartType_Array != Object.getType()) {
    return Errors::INVALID_VALUE;
  }
  size_t sizeLimit;
  const size_t array_len = Object.length();

  if (mMinSize.getValue(sizeLimit) && (array_len < sizeLimit)) {
    return Errors::OUT_OF_RANGE;
  }
  if (mMaxSize.getValue(sizeLimit) && (array_len > sizeLimit)) {
    return Errors::OUT_OF_RANGE;
  }

  for (size_t i = 0u; i < array_len; ++i) {
    const Errors::eType result =
      mElementSchemaItem->validate(Object.getElement(i));
    if (Errors::OK != result) {
      return result;
    }
  }
  return Errors::OK;
}

void CArraySchemaItem::applySchema(SmartObject& Object) {
  if (SmartType_Array == Object.getType()) {
    for (size_t i = 0U; i < Object.length(); ++i) {
      mElementSchemaItem->applySchema(Object[i]);
    }
  }
}

void CArraySchemaItem::unapplySchema(SmartObject& Object) {
  if (SmartType_Array == Object.getType()) {
    for (size_t i = 0U; i < Object.length(); ++i) {
      mElementSchemaItem->unapplySchema(Object[i]);
    }
  }
}

void CArraySchemaItem::BuildObjectBySchema(
    const SmartObject& pattern_object, SmartObject& result_object) {
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
  : mElementSchemaItem(ElementSchemaItem),
    mMinSize(MinSize),
    mMaxSize(MaxSize) {
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
