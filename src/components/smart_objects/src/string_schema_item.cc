/**
 * @file CStringSchemaItem.cpp
 * @brief CStringSchemaItem source file.
 */
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

#include "smart_objects/smart_object.h"
#include "smart_objects/string_schema_item.h"

namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem> NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MinLength,
                                                                                                                                    const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                                                                                    const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue)
{
    return new NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem(MinLength, MaxLength, DefaultValue);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::SmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_String == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;

        size_t minLength;
        size_t maxLength;
        std::string value = Object;

        if (true == mMinLength.getValue(minLength))
        {
            if (value.size() < minLength)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
            }
        }

        if (true == mMaxLength.getValue(maxLength))
        {
            if (value.size() > maxLength)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

    }
    else
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

bool NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::SmartObject & Object)
{
    bool result = false;
    std::string value;

    if (true == mDefaultValue.getValue(value))
    {
        Object = value;
        result = true;
    }

    return result;
}

void smart_objects_ns::CStringSchemaItem::BuildObjectBySchema(
    const smart_objects_ns::SmartObject& pattern_object,
    smart_objects_ns::SmartObject& result_object) {

  if (smart_objects_ns::SmartType_String == pattern_object.getType()) {
    result_object = pattern_object;
  } else {
    bool result = setDefaultValue(result_object);
    if (false == result) {
      result_object = std::string("");
    }
  }


}

NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::CStringSchemaItem(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MinLength,
                                                                        const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                        const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue):
mMinLength(MinLength),
mMaxLength(MaxLength),
mDefaultValue(DefaultValue)
{
}
