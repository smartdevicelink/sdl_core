/**
 * @file CFormatterJsonSDLRPCv2.hpp
 * @brief CFormatterJsonSDLRPCv2 header file.
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

#ifndef __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS__CFORMATTERJSONSDLRPCV2_HPP__
#define __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS__CFORMATTERJSONSDLRPCV2_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

#include "CFormatterJsonBase.hpp"
#include "JSONHandler/CSmartFactory.hpp"

namespace NsSmartDeviceLink { namespace NsJSONHandler { namespace Formatters {


    /**
     * @brief Class is used to convert SmartObjects to JSON string and vice versa.
     *
     * JSON strings is in SDLRPCv2 format.
     */
    class CFormatterJsonSDLRPCv2 : public CFormatterJsonBase
    {
    private:

        /**
         * @brief Hidden constructor.
         *
         * The class contains only static methods. Should not be instantiated.
         */
        CFormatterJsonSDLRPCv2();

        /**
         * @brief Hidden copy constructor.
         *
         * The class contains only static methods. Should not be instantiated.
         */
        CFormatterJsonSDLRPCv2(const CFormatterJsonSDLRPCv2&);

    public:

        typedef NsSmartDeviceLink::NsJSONHandler::Formatters::
        meta_formatter_error_code::tMetaFormatterErrorCode tMetaFormatterErrorCode;

        /**
         * @brief Creates a JSON string from a SmartObject.
         *
         * @param obj input SmartObject
         * @param outStr resulting JSON string
         * @return true if success, false otherwise
         */
        static bool toString(const NsSmartDeviceLink::NsSmartObjects::CSmartObject &obj,
                std::string& outStr);


        /**
         * @brief Creates a SmartObject from a JSON string.
         *
         * @param str Input JSON string in SDLRPCv2 format
         * @param out Output SmartObject
         * @param functionId The corresponding field in SmartObject is filled with this param.
         * @param messageType The corresponding field in SmartObject is filled with this param.
         * @param correlatioId It's like sequence number. The corresponding field in SmartObject
         *  is filled with this param.
         * @return true if success, otherwise - false
         */
        template<typename FunctionId, typename MessageType>
        static bool fromString(const std::string &str,
                NsSmartDeviceLink::NsSmartObjects::CSmartObject &out,
                FunctionId functionId,
                MessageType messageType,
                int correlationId);

      /**
        * @brief Converts to string the smart object against the given schema
        *
        * @param object Original smart object
        * @param schema Smart schema which describes 'fake' smart object to be formatted
        * @param outStr Resulting JSON string
        * @return formatting error code
        */
      static tMetaFormatterErrorCode MetaFormatToString(
                const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object,
                NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema,
                std::string& outStr);
    };

} } } // namespace NsSmartDeviceLink::NsJSONHandler::Formatters


template<typename FunctionId, typename MessageType>
inline bool NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::fromString(
        const std::string& str,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out,
        FunctionId functionId,
        MessageType messageType,
        int correlationId)
{
    Json::Value root;
    Json::Reader reader;

    namespace S = NsSmartDeviceLink::NsJSONHandler::strings;

    out[S::S_PARAMS][S::S_MESSAGE_TYPE] = messageType;
    out[S::S_PARAMS][S::S_FUNCTION_ID] = functionId;
    out[S::S_PARAMS][S::S_CORRELATION_ID] = correlationId;
    out[S::S_PARAMS][S::S_PROTOCOL_TYPE] = 0;
    out[S::S_PARAMS][S::S_PROTOCOL_VERSION] = 2;

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
        jsonValueToObj(root, out[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS]);
    }

    return parsingSuccessful;
}

#endif // __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS__CFORMATTERJSONSDLRPCV2_HPP__
