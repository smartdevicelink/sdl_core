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

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem> NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                                                                                         const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue)
{
    return new NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem(MaxLength, DefaultValue);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_String == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;

        size_t maxLength;
        std::string value = Object;

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

bool NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
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

void NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::BuildObjectBySchema(
              NsSmartDeviceLink::NsSmartObjects::CSmartObject & object) {
  bool result = setDefaultValue(object);
  if (false ==result) {
    object = std::string("");
  }
}

NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::CStringSchemaItem(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue):
mMaxLength(MaxLength),
mDefaultValue(DefaultValue)
{
}
