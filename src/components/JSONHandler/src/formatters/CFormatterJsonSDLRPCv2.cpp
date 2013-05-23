/**
 * @file CFormatterJsonSDLRPCv2.cpp
 * @brief CFormatterJsonSDLRPCv2 source file.
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

#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"

namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

// ----------------------------------------------------------------------------

bool NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::toString(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    NsSmartDeviceLink::NsSmartObjects::CSmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);       // converts enums(as int) to strings

    objToJsonValue(formattedObj.getElement(NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS), root);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

Formatters::CFormatterJsonSDLRPCv2::tMetaFormatterErrorCode
  Formatters::CFormatterJsonSDLRPCv2::MetaFormatToString(
            const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object,
            NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema,
            std::string& outStr) {

    NsSmartDeviceLink::NsSmartObjects::CSmartObject tmp_object;

    tMetaFormatterErrorCode resultCode =
        CMetaFormatter::createObjectByPattern(object, schema, tmp_object);

    CFormatterJsonSDLRPCv2::toString(tmp_object, outStr);

    return resultCode;
}