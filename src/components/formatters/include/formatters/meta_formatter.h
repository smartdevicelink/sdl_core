/**
 * @file meta_fromatter.h
 * @brief file describes class CMetaFormatter which is designed to format
 *        the smart object against given schema for given formatter
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

#ifndef __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTER_H__
#define __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTER_H__

#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

/**
  * @brief Formats to string the smart object against given schema for given formatter
  *
  * Sample usage:
  *    CSmartFactory factory;
  *    SmartObject object;
  *    CSmartSchmema schema;
  *
  */
class CMetaFormatter {
 public:

  /**
    * @brief Creates smart object by the given schema having copied
    *        matched tree elements from original object.
    *
    * @param object Original smart object which macthed tree elements
    *        will be copied from
    * @param schema Smart schema which describes result smart object
    * @param result_object createdsmart object
    * @return true if successful, false - otherwise
    */
  static bool CreateObjectByPattern(
          const NsSmartDeviceLink::NsSmartObjects::SmartObject& object,
          const NsSmartDeviceLink::NsSmartObjects::CSmartSchema& schema,
          NsSmartDeviceLink::NsSmartObjects::SmartObject& result_object);
};

}// namespace NsSmartDeviceLink
}// namespace NsSmartDeviceLink::NsJSONHandler
}// namespace NsSmartDeviceLink::NsJSONHandler::Formatters

#endif // __SMARTDEVICELINKCORE_JSONHANDLER_FORMATTERS_METAFORMATTER_H__
