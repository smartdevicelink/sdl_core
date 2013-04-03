/**
 * @file CSmartObject.hpp
 * @brief CSmartObject header file.
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

#ifndef __SMARTOBJECT_ISCHEMA_ITEM_HPP__
#define __SMARTOBJECT_ISCHEMA_ITEM_HPP__

#include "Errors.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        /**
         * @brief Base schema item.
         **/
        class ISchemaItem
        {
        public:
            /**
             * @brief Validate object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const CSmartObject & Object);

            /**
             * @brief Set default value to an object.
             *
             * @param Object Object to set default value.
             *
             * @return true if default value was successfully set, false otherwise.
             **/
            virtual bool setDefaultValue(CSmartObject & Object);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief Unapply schema.
             *
             * @param Object Object to unapply schema.
             **/
            virtual void unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);
        };
    }
}

#endif
