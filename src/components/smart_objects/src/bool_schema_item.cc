/**
 * @file CBoolSchemaItem.cpp
 * @brief CBoolSchemaItem source file.
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

#include "smart_objects/bool_schema_item.h"
#include "smart_objects/smart_object.h"

namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue)
{
    return new CBoolSchemaItem(DefaultValue);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::SmartObject & Object)
{
    return (true == NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean == Object.getType()) ? NsSmartDeviceLink::NsSmartObjects::Errors::OK : NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
}

bool NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::SmartObject & Object)
{
    bool result = false;
    bool value = false;

    if (true == mDefaultValue.getValue(value))
    {
        Object = value;
        result = true;
    }

    return result;
}


void smart_objects_ns::CBoolSchemaItem::BuildObjectBySchema(
    const smart_objects_ns::SmartObject& pattern_object,
    smart_objects_ns::SmartObject& result_object) {

  if (smart_objects_ns::SmartType_Boolean == pattern_object.getType()) {
    result_object = pattern_object;
  } else {
    bool result = setDefaultValue(result_object);
    if (false == result) {
      result_object = false;
    }
  }


}

NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::CBoolSchemaItem(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue):
mDefaultValue(DefaultValue)
{
}

