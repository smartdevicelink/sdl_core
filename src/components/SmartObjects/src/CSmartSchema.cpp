/**
 * @file CSmartSchema.cpp
 * @brief CSmartSchema source file.
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

#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::CSmartSchema::CSmartSchema()
: mSchemaItem(NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem::create())
{
}

NsSmartDeviceLink::NsSmartObjects::CSmartSchema::CSmartSchema(utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem)
: mSchemaItem(SchemaItem)
{

}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CSmartSchema::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object)
{
    return mSchemaItem->validate(object);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::setSchemaItem(utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem)
{
    mSchemaItem = SchemaItem;
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    mSchemaItem->applySchema(Object);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    mSchemaItem->unapplySchema(Object);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::BuildObjectBySchema(
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject& pattern_object,
    NsSmartDeviceLink::NsSmartObjects::CSmartObject& result_object) const  {
  
  mSchemaItem->BuildObjectBySchema(pattern_object, result_object);
}