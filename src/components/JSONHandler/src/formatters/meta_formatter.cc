/**
 * @file meta_fromatter.cc
 * @brief implementation of class CMetaFormatter which is designed to format
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
// POSSIBILITY OF SUCH DAMAGE.auto

#include <set>
#include <string>

#include "JSONHandler/formatters/meta_formatter.h"
#include "JSONHandler/CSmartFactory.hpp"
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv1.hpp"
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv2.hpp"

namespace formatter_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;
namespace jsonhandler_ns = NsSmartDeviceLink::NsJSONHandler;

//---------------------------------------------------------------

bool formatter_ns::CMetaFormatter::CreateObjectByPattern(
            const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object,
            const NsSmartDeviceLink::NsSmartObjects::CSmartSchema& schema,
            NsSmartDeviceLink::NsSmartObjects::CSmartObject& result_object) {

  if (smart_objects_ns::SmartType_Invalid == result_object.getType()) {
    return false;
  }

  // build temporary object by schema filled with default values
  schema.BuildObjectBySchema(result_object);

  // fill output object
  CMetaFormatter::FillOutputObject(object, result_object);

  return true;
}

//---------------------------------------------------------------

void formatter_ns::CMetaFormatter::FillOutputObject(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& in_object,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out_object) {
  
  if (in_object.getType() == smart_objects_ns::SmartType_Invalid
      || in_object.getType() == smart_objects_ns::SmartType_Null
      || in_object.getType() != out_object.getType()) {
    return;
  }

  if (in_object.getType() == smart_objects_ns::SmartType_Map) {
    
    const std::set<std::string> in_object_keys = in_object.enumerate();
    for(std::set<std::string>::const_iterator i = in_object_keys.begin();
        i != in_object_keys.end(); ++i) {
      if (true == out_object.keyExists(*i)) {
        CMetaFormatter::FillOutputObject(in_object.getElement(*i),
                                          out_object[*i]);
      }      
    }      
  } else if (in_object.getType() == smart_objects_ns::SmartType_Array) {
    
    for(int i = 0; i < in_object.length(); i++) {
      CMetaFormatter::FillOutputObject(in_object.getElement(i), out_object[i]);      
    }    
  } else {
    out_object = in_object;
  }
}  
  
