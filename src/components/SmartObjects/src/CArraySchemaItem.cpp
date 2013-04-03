/**
 * @file CArraySchemaItem.cpp
 * @brief CArraySchemaItem source file.
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

#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem> NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> & ElementSchemaItem,
                                                                                                                                       const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MinSize,
                                                                                                                                       const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxSize)
{
    return new NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem(ElementSchemaItem, MinSize, MaxSize);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;
        size_t sizeLimit;

        if (true == mMinSize.getValue(sizeLimit))
        {
            if (Object.length() < sizeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if ((NsSmartDeviceLink::NsSmartObjects::Errors::OK == result) &&
            (true == mMaxSize.getValue(sizeLimit)))
        {
            if (Object.length() > sizeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if (NsSmartDeviceLink::NsSmartObjects::Errors::OK == result)
        {
            for (size_t i = 0U; i < Object.length(); ++i)
            {
                result = mElementSchemaItem->validate(Object.getElement(i));

                if (NsSmartDeviceLink::NsSmartObjects::Errors::OK != result)
                {
                    break;
                }
            }
        }
    }
    else
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

void NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->applySchema(Object[i]);
        }
    }
}

void NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->unapplySchema(Object[i]);
        }
    }
}

NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::CArraySchemaItem(const TSharedPtr<ISchemaItem> & ElementSchemaItem,
                                                              const TSchemaItemParameter<size_t> & MinSize,
                                                              const TSchemaItemParameter<size_t> & MaxSize):
mElementSchemaItem(ElementSchemaItem),
mMinSize(MinSize),
mMaxSize(MaxSize)
{
}
