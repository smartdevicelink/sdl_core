/**
 * @file CSmartFactory.hpp
 * @brief CSmartFactory header file.
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

#ifndef __CSMARTFACTORY_HPP__
#define __CSMARTFACTORY_HPP__

#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"
#include <map>
#include <string>

namespace NsSmartDeviceLink
{
    namespace NsJSONHandler
    {
        /**
         * @brief String constants used by SmartFactory.
         */
        namespace strings
        {
            /**
             * @brief String constant for MSG_PARAMS.
             */
            extern const std::string S_MSG_PARAMS;

            /**
             * @brief String constant for PARAMS.
             */
            extern const std::string S_PARAMS;

            /**
             * @brief String constant for FUNCTION_ID.
             */
            extern const std::string S_FUNCTION_ID;

            /**
             * @brief String constant for MESSAGE_TYPE.
             */
            extern const std::string S_MESSAGE_TYPE;

            /**
             * @brief String constant for PROTOCOL_VERSION.
             */
            extern const std::string S_PROTOCOL_VERSION;

            /**
             * @brief String constant for PROTOCOL_TYPE.
             */
            extern const std::string S_PROTOCOL_TYPE;

            /**
             * @brief String constant for CORRELATION_ID.
             */
            extern const std::string S_CORRELATION_ID;

          /**
           * @brief String constant for "code" param name.
           */
          extern const std::string kCode;

          /**
           * @brief String constant for "message" param name.
           */
          extern const std::string kMessage;
        }

        /**
         * @brief Smart Schema key.
         *
         * @tparam FunctionIdEnum Type of function ID enum.
         * @tparam MessageTypeEnum Type of messageType enum.
         */
        template <class FunctionIdEnum, class MessageTypeEnum>
        struct SmartSchemaKey
        {
            /**
             * @brief Value of function ID for the key.
             */
            FunctionIdEnum functionId;

            /**
             * @brief Value of messageType for the key.
             */
            MessageTypeEnum messageType;

            /**
             * @brief Constructor.
             *
             * @param functionIdParam Value of function ID.
             * @param messageTypeParam Value of message type.
             */
            SmartSchemaKey(FunctionIdEnum functionIdParam, MessageTypeEnum messageTypeParam);
        };

        /**
         * @brief Smart Factory.
         *
         * This class is used as base class for generated factories.
         * Clients should use methods of this class to access all
         * SmartSchema validation features.
         *
         * @tparam FunctionIdEnum Type of function ID enum.
         * @tparam MessageTypeEnum Type of messageType enum.
         * @tparam StructIdEnum Type of StructId enum.
         */
        template <class FunctionIdEnum, class MessageTypeEnum, class StructIdEnum>
        class CSmartFactory
        {
        public:

            /**
             * @brief Constructor.
             */
            CSmartFactory(void);

            /**
             * @brief Attach schema to the function SmartObject.
             *
             * @param object SmartObject to attach schema for.
             *
             * @return True if operation was successful or false otherwise.
             */
            bool attachSchema(NsSmartDeviceLink::NsSmartObjects::SmartObject& object);

          /**
           * @brief Attach schema to the struct SmartObject.
           *
           * @param struct_id Identifier of the struct.
           * @param object SmartObject to attach schema for.
           *
           * @return True if operation was successful of false otherwise.
           */
          bool AttachSchema(const StructIdEnum struct_id,
              NsSmartDeviceLink::NsSmartObjects::SmartObject &object);


          /**
           * @brief Create new SmartObject with attached function SmartSchema.
           *
           * @param function_id FunctionID of the function.
           * @param message_type messageType of the function.
           *
           * @return If function succeeded it returns new SmartObject with
           *         map type and attached SmartSchema. Client can use such
           *         object to store specific function and perform validation.
           *         Otherwise (if SmartSchema was not attached to the
           *         SmartObject) function returns empty SmartObject with
           *         null type.
           */
          NsSmartDeviceLink::NsSmartObjects::SmartObject CreateSmartObject(
              const FunctionIdEnum function_id,
              const MessageTypeEnum message_type);

          /**
           * @brief Create new SmartObject with attached struct SmartSchema.
           *
           * @param struct_id Identifier of the struct.
           *
           * @return If function succeeded it returns new SmartObject with
           *         map type and attached SmartSchema. Client can use such
           *         object to store specific struct and perform validation.
           *         Otherwise (if SmartSchema was not attached to the
           *         SmartObject) function returns empty SmartObject with
           *         null type.
           */
          NsSmartDeviceLink::NsSmartObjects::SmartObject CreateSmartObject(
              const StructIdEnum struct_id);

          /**
           * @brief Get SmartSchema for specific function.
           *
           * @param function_id FunctionID of the function.
           * @param message_type messageType of the function.
           * @param[out] result This value will be copy of the desired
           *                    function SmartSchema if it found (this
           *                    function returns true) or unmodified if
           *                    SmartSchema is not found (this function
           *                    returns false).
           *
           * @return True if function schema for specified input parameters
           *         is found or false otherwise.
           */
           bool GetSchema(
              const FunctionIdEnum function_id,
              const MessageTypeEnum message_type,
              NsSmartDeviceLink::NsSmartObjects::CSmartSchema &result);

          /**
           * @brief Get SmartSchema for specific struct.
           *
           * @param struct_id Identifier of the struct.
           *
           * @param[out] result This value will be copy of the desired
           *                    struct SmartSchema if it found (this
           *                    function returns true) or unmodified if
           *                    SmartSchema is not found (this function
           *                    returns false).
           *
           * @return True if struct schema for specified input parameter is
           *         found or false otherwise.
           */
          bool GetSchema(
              const StructIdEnum struct_id,
              NsSmartDeviceLink::NsSmartObjects::CSmartSchema &result);

        protected:

          /**
           * @brief Defines map of SmartSchemaKeys to the SmartSchemes.
           *
           * This container type should be used to store SmartSchemes of
           * functions.
           */
          typedef std::map<SmartSchemaKey<FunctionIdEnum, MessageTypeEnum>,
            NsSmartDeviceLink::NsSmartObjects::CSmartSchema>
            FuncionsSchemesMap;

          /**
           * @brief Defines map of StructIdEnum to the SmartSchemes.
           *
           * This container type should be used to store SmartSchemes of
           * structs.
           */
          typedef std::map<StructIdEnum,
            NsSmartDeviceLink::NsSmartObjects::CSmartSchema>
            StructsSchemesMap;

          /**
           * @brief Map of all function schemes for this factory.
           */
          FuncionsSchemesMap functions_schemes_;

          /**
           * @brief Map of all struct shemes for this factory.
           */
          StructsSchemesMap structs_schemes_;
        };

        template <class FunctionIdEnum, class MessageTypeEnum, class StructIdEnum>
        CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::CSmartFactory(void)
        : functions_schemes_(),
          structs_schemes_()
        {
        }

        template <class FunctionIdEnum, class MessageTypeEnum, class StructIdEnum>
        bool CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::attachSchema(NsSmartDeviceLink::NsSmartObjects::SmartObject &object)
        {
            if(false == object.keyExists(strings::S_PARAMS)) return false;
            if(false == object[strings::S_PARAMS].keyExists(strings::S_MESSAGE_TYPE)) return false;
            if(false == object[strings::S_PARAMS].keyExists(strings::S_FUNCTION_ID)) return false;

            MessageTypeEnum msgtype((MessageTypeEnum)object[strings::S_PARAMS][strings::S_MESSAGE_TYPE].asInt());
            FunctionIdEnum fid((FunctionIdEnum)object[strings::S_PARAMS][strings::S_FUNCTION_ID].asInt());

            SmartSchemaKey<FunctionIdEnum, MessageTypeEnum> key(fid, msgtype);

            typename FuncionsSchemesMap::iterator schemaIterator = functions_schemes_.find(key);

            if(schemaIterator == functions_schemes_.end())
            {
                // Schema was not found
                return false;
            }

            object.setSchema(schemaIterator->second);
            schemaIterator->second.applySchema(object);

            return true;
        }

      template <class FunctionIdEnum,
          class MessageTypeEnum,
          class StructIdEnum>
      bool CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::
          AttachSchema(
              const StructIdEnum struct_id,
              NsSmartDeviceLink::NsSmartObjects::SmartObject &object) {
        typename StructsSchemesMap::iterator structs_iterator =
            structs_schemes_.find(struct_id);

        if (structs_iterator == structs_schemes_.end()) {
          return false;
        }

        object.setSchema(structs_iterator->second);
        structs_iterator->second.applySchema(object);

        return true;
      }

      template <class FunctionIdEnum,
          class MessageTypeEnum,
          class StructIdEnum>
      NsSmartDeviceLink::NsSmartObjects::SmartObject
          CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::
          CreateSmartObject(
              const FunctionIdEnum function_id,
              const MessageTypeEnum message_type) {
        SmartSchemaKey<FunctionIdEnum, MessageTypeEnum> key(
          function_id, message_type);

        typename FuncionsSchemesMap::iterator schema_iterator =
            functions_schemes_.find(key);

        if(schema_iterator != functions_schemes_.end()) {
          NsSmartDeviceLink::NsSmartObjects::SmartObject function_object(
              NsSmartDeviceLink::NsSmartObjects::SmartType_Map);
          function_object.setSchema(schema_iterator->second);
          schema_iterator->second.applySchema(function_object);
            return function_object;
        }

        return NsSmartDeviceLink::NsSmartObjects::SmartObject();
      }

      template <class FunctionIdEnum,
          class MessageTypeEnum,
          class StructIdEnum>
      NsSmartDeviceLink::NsSmartObjects::SmartObject
          CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::
          CreateSmartObject(const StructIdEnum struct_id) {
        NsSmartDeviceLink::NsSmartObjects::SmartObject struct_object(
            NsSmartDeviceLink::NsSmartObjects::SmartType_Map);
        if (AttachSchema(struct_id, struct_object)) {
          return struct_object;
        }

        return NsSmartDeviceLink::NsSmartObjects::SmartObject();
      }

      template <class FunctionIdEnum,
          class MessageTypeEnum,
          class StructIdEnum>
      bool CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::
      GetSchema(const FunctionIdEnum function_id,
                const MessageTypeEnum message_type,
                NsSmartDeviceLink::NsSmartObjects::CSmartSchema &result) {
        SmartSchemaKey<FunctionIdEnum, MessageTypeEnum> key(function_id,
                                                            message_type);

        typename FuncionsSchemesMap::iterator schema_iterator =
            functions_schemes_.find(key);

        if(schema_iterator != functions_schemes_.end()) {
          result = schema_iterator->second;
          return true;
        }

        return false;
      }

      template <class FunctionIdEnum,
          class MessageTypeEnum,
          class StructIdEnum>
      bool CSmartFactory<FunctionIdEnum, MessageTypeEnum, StructIdEnum>::
      GetSchema(const StructIdEnum struct_id,
                NsSmartDeviceLink::NsSmartObjects::CSmartSchema &result) {
        typename StructsSchemesMap::iterator structs_iterator =
            structs_schemes_.find(struct_id);

        if(structs_iterator != structs_schemes_.end()) {
          result = structs_iterator->second;
          return true;
        }

        return false;
      }

        template <class FunctionIdEnum, class MessageTypeEnum>
        SmartSchemaKey<FunctionIdEnum, MessageTypeEnum>::SmartSchemaKey(FunctionIdEnum functionIdParam, MessageTypeEnum messageTypeParam)
        : functionId(functionIdParam)
        , messageType(messageTypeParam)
        {

        }

        template <class FunctionIdEnum, class MessageTypeEnum>
        bool operator<(const SmartSchemaKey< FunctionIdEnum, MessageTypeEnum >& l, const SmartSchemaKey< FunctionIdEnum, MessageTypeEnum >& r)
        {
            if (l.functionId < r.functionId) return true;
            if (l.functionId > r.functionId) return false;

            if (l.messageType < r.messageType) return true;
            if (l.messageType > r.messageType) return false;

            return false;
        }
    }
}
#endif //__CSMARTFACTORY_HPP__
