/**
 * @file CFormatterJsonALRPCv1.cpp
 * @brief CFormatterJsonALRPCv1 source file.
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

#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"

namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace SmartObjects = NsSmartDeviceLink::NsSmartObjects;
namespace Strings = NsSmartDeviceLink::NsJSONHandler::strings;

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::S_REQUEST("request");
const std::string Formatters::CFormatterJsonALRPCv1::S_RESPONSE("response");
const std::string Formatters::CFormatterJsonALRPCv1::S_NOTIFICATION("notification");
const std::string Formatters::CFormatterJsonALRPCv1::S_PARAMETERS("parameters");
const std::string Formatters::CFormatterJsonALRPCv1::S_NAME("name");
const std::string Formatters::CFormatterJsonALRPCv1::S_CORRELATION_ID("correlationID");

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::getMessageType(
        const SmartObjects::CSmartObject& obj)
{
    return obj.getElement(Strings::S_PARAMS).getElement(Strings::S_MESSAGE_TYPE);
}

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::getMessageType(
        const Json::Value& root)
{
    std::string type;

    if (true == root.isMember(S_REQUEST))
    {
        type = S_REQUEST;
    }
    else if (true == root.isMember(S_RESPONSE))
    {
        type = S_RESPONSE;
    }
    else if (true == root.isMember(S_NOTIFICATION))
    {
        type = S_NOTIFICATION;
    }
    else
    {}

    return type;
}

// ----------------------------------------------------------------------------

bool Formatters::CFormatterJsonALRPCv1::toString(
        const SmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);
    Json::Value params(Json::objectValue);

    SmartObjects::CSmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);       // converts enums(as int) to strings

    objToJsonValue(formattedObj.getElement(Strings::S_MSG_PARAMS), params);

    std::string type = getMessageType(formattedObj);
    root[type] = Json::Value(Json::objectValue);
    root[type][S_PARAMETERS] = params;

    root[type][S_CORRELATION_ID] = static_cast<int>(
            formattedObj.getElement(Strings::S_PARAMS).getElement(Strings::S_CORRELATION_ID));

    root[type][S_NAME] = static_cast<std::string>(
            formattedObj.getElement(Strings::S_PARAMS).getElement(Strings::S_FUNCTION_ID));

    outStr = root.toStyledString();

    return true;
}


