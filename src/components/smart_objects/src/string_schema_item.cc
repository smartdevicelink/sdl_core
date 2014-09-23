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
#include "smart_objects/smart_object.h"
#include "smart_objects/string_schema_item.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

utils::SharedPtr<CStringSchemaItem> CStringSchemaItem::create(
    const TSchemaItemParameter<size_t>& MinLength,
    const TSchemaItemParameter<size_t>& MaxLength,
    const TSchemaItemParameter<std::string>& DefaultValue) {
  return new CStringSchemaItem(MinLength, MaxLength, DefaultValue);
}

Errors::eType CStringSchemaItem::validate(const SmartObject& Object) {
  if (SmartType_String != Object.getType()) {
    return Errors::INVALID_VALUE;
  }

  const std::string value = Object.asString();
  size_t length;

  if (mMinLength.getValue(length) && (value.size() < length)) {
    return Errors::OUT_OF_RANGE;
  }
  if (mMaxLength.getValue(length) && (value.size() > length)) {
    return Errors::OUT_OF_RANGE;
  }
  return Errors::OK;
}

SmartType CStringSchemaItem::getSmartType() const {
  return SmartType_String;
}

std::string CStringSchemaItem::getDefaultValue() const {
  return std::string("");
}

CStringSchemaItem::CStringSchemaItem(
    const TSchemaItemParameter<size_t>& MinLength,
    const TSchemaItemParameter<size_t>& MaxLength,
    const TSchemaItemParameter<std::string>& DefaultValue)
    : CDefaultSchemaItem<std::string>(DefaultValue),
      mMinLength(MinLength),
      mMaxLength(MaxLength) {
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
