/**
 * @file CSmartSchema.hpp
 * @brief CSmartSchema header file.
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

#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

#include "utils/shared_ptr.h"
#include "ISchemaItem.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        /**
         * @brief Smart Schema.
         */
        class CSmartSchema
        {
        public:

            /**
             * @brief Constructor.
             *
             * Default constructor. Creates empty schema.
             */
            CSmartSchema();

            /**
             * @brief Constructor.
             *
             * Creates schema with given root schema item.
             *
             * @param SchemaItem Root schema item.
             */
            CSmartSchema(utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Validate smart object.
             *
             * @param Object SmartObject to validate.
             *
             * @return Result of validation.
             */
            Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& Object);

            /**
             * @brief Set new root schema item.
             *
             * @param SchemaItem Root schema item.
             */
            void setSchemaItem(utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief The reverse SmartObject conversion using schema.
             *
             * @param object Object to convert.
             */
            virtual void unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject& object);

            /**
             * @brief Build smart object by smart schema having copied matched
             *        parameters from pattern smart object
             *
             * @param pattern_object pattern object
             * @param result_object object to build
             */
            virtual void BuildObjectBySchema(
              const NsSmartDeviceLink::NsSmartObjects::CSmartObject& pattern_object,
              NsSmartDeviceLink::NsSmartObjects::CSmartObject& result_object) const;

        protected:

            /**
             * @brief Root schema item.
             */
            utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> mSchemaItem;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__
