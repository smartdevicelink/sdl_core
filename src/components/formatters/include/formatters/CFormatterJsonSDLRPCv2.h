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

#include "json/json.h"

#include "smart_objects/smart_object.h"

#include "CFormatterJsonBase.h"
#include "formatters/CSmartFactory.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

/**
 * @brief Class is used to convert SmartObjects to JSON string and vice versa.
 *
 * JSON strings is in SDLRPCv2 format.
 */
class CFormatterJsonSDLRPCv2 : public CFormatterJsonBase {
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

  typedef NsSmartDeviceLink::NsJSONHandler::Formatters::meta_formatter_error_code::tMetaFormatterErrorCode tMetaFormatterErrorCode;

  /**
   * @brief Creates a JSON string from a SmartObject.
   *
   * @param obj input SmartObject
   * @param outStr resulting JSON string
   * @return true if success, false otherwise
   */
  static bool toString(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject &obj,
      std::string& outStr);

  /**
   * @brief Creates a SmartObject from a JSON string.
   *
   * @param str Input JSON string in SDLRPCv2 format
   * @param out Output SmartObject
   * @param functionId The corresponding field in SmartObject is filled with this param.
   * @param messageType The corresponding field in SmartObject is filled with this param.
   * @return true if success, otherwise - false
   */
  template<typename FunctionId, typename MessageType>
  static bool fromString(const std::string &str,
                         NsSmartDeviceLink::NsSmartObjects::SmartObject &out,
                         FunctionId functionId, MessageType messageType);

  /**
   * @brief Creates a SmartObject from a JSON string.
   *
   * Version with CorrelationID.
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
                         NsSmartDeviceLink::NsSmartObjects::SmartObject &out,
                         FunctionId functionId, MessageType messageType,
                         int32_t correlationId);

  /**
   * @brief Converts to string the smart object against the given schema
   *
   * @param object Original smart object
   * @param schema Smart schema which describes 'fake' smart object to be formatted
   * @param outStr Resulting JSON string
   * @return formatting error code
   */
  static tMetaFormatterErrorCode MetaFormatToString(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& object,
      const NsSmartDeviceLink::NsSmartObjects::CSmartSchema& schema,
      std::string& outStr);
};

template<typename FunctionId, typename MessageType>
inline bool CFormatterJsonSDLRPCv2::fromString(
    const std::string& str, NsSmartDeviceLink::NsSmartObjects::SmartObject& out,
    FunctionId functionId, MessageType messageType) {
  bool result = true;

  try {
    Json::Value root;
    Json::Reader reader;

    namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;
    bool result = reader.parse(str, root);

    if (true == result) {
      out[strings::S_PARAMS][strings::S_MESSAGE_TYPE] = messageType;
      out[strings::S_PARAMS][strings::S_FUNCTION_ID] = functionId;
      out[strings::S_PARAMS][strings::S_PROTOCOL_TYPE] = 0;
      out[strings::S_PARAMS][strings::S_PROTOCOL_VERSION] = 2;

      jsonValueToObj(root, out[strings::S_MSG_PARAMS]);
    }
  } catch (...) {
    result = false;
  }

  return result;
}

template<typename FunctionId, typename MessageType>
inline bool CFormatterJsonSDLRPCv2::fromString(
    const std::string& str, NsSmartDeviceLink::NsSmartObjects::SmartObject& out,
    FunctionId functionId, MessageType messageType, int32_t correlationId) {

  bool result = fromString(str, out, functionId, messageType);
  namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;

  if (true == result) {
    out[strings::S_PARAMS][strings::S_CORRELATION_ID] = correlationId;
  }

  return result;
}

}
}
}  // namespace NsSmartDeviceLink::NsJSONHandler::Formatters

#endif // __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS__CFORMATTERJSONSDLRPCV2_HPP__
