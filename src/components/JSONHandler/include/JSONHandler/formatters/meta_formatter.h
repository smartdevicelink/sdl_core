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

#ifndef __JSONHANDLER_CMETAFORMATTER_HPP__
#define __JSONHANDLER_CMETAFORMATTER_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CSmartSchema.hpp"

namespace NsSmartDeviceLink { namespace NsJSONHandler { namespace Formatters {
    
  namespace e_formatter_version {

    /**
     * @brief Type (version) of formatter
     */
    enum Type {

      /**
       * @brief Formatter of version SDLRPCv1
       */
      FORMATTER_SDLRPCv1 = 0,

      /**
       * @brief Formatter of version SDLRPCv2
       */
      FORMATTER_SDLRPCv2
    };
  }


  namespace meta_formatter_error_code {
    /**
      * @brief Error codes of MetaFormatter represented as bitmask
      **/
    typedef long tMetaFormatterErrorCode ;

    /**
      * @brief OK, no error
      */
    static const tMetaFormatterErrorCode ERROR_OK = 0x0;

    /**
      * @brief origin smart object is not function
      */
    static const tMetaFormatterErrorCode ERROR_OBJECT_IS_NOT_FUNCTION = 0x01;

    /**
      * @brief smart shema describes object which is not function
      */
    static const tMetaFormatterErrorCode ERROR_SCHEMA_IS_NOT_FUNCTION = 0x02;
  }

  /**
   * @brief Formats to string the smart object against given schema for given formatter
   *
   * Sample usage:
   *    CSmartFactory factory;
   *    CSmartObject object;
   *    CSmartSchmema schema;
   * 
   */
  class CMetaFormatter
  {
  public:
    
    /**
     * @brief Converts to string the smart object against given schema for given formatter version
     *
     * @param object Original smart object
     * @param schema Smart schema which describes 'fake' smart object to be formatted
     * @param formatter_version Version of formatter
     * @param result_string The resulting formatted string
     * @return formatting error code
     */
    static meta_formatter_error_code::tMetaFormatterErrorCode FormatToString(
            const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object,
            NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema,
            const e_formatter_version::Type formatter_version,
            std::string& result_string);

  private:

    /**
     * @brief Copy service parameters, i.e. parameters of smartObject["params"]
     *
     * @param in_object Smart object which service parameters be copied from
     * @param out_object Smart object which service parameters be copied to
     */
    static void CopyServiceParams(
        NsSmartDeviceLink::NsSmartObjects::CSmartObject in_object,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out_object);

     /**
     * @brief Fill out_object with values from in_object, if tree elements of both objects matched
     *
     * @param in_object Smart object which service parameters be read from
     * @param out_object Smart object which service parameters be filled to
     */
    static void FillOutputObject(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& in_object,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out_object);

  };
    
} } } // namespace NsSmartDeviceLink::NsJSONHandler::Formatters

#endif // __JSONHANDLER_CMETAFORMATTER_HPP__
