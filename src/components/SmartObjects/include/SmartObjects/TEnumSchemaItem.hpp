/**
 * @file TEnumSchemaItem.hpp
 * @brief TEnumSchemaItem header file.
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

#ifndef __SMARTOBJECT_TENUMSCHEMAITEM_HPP__
#define __SMARTOBJECT_TENUMSCHEMAITEM_HPP__

#include <map>
#include <set>
#include <string>

#include "utils/shared_ptr.h"

#include "CSmartObject.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Enumeration schema item.
         *
         * @tparam EnumType Enumeration type.
         **/
        template <typename EnumType>
        class TEnumSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param AllowedElements Set of allowed enumeration elements.
             * @param DefaultValue Default value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static utils::SharedPtr<TEnumSchemaItem> create(const std::set<EnumType> & AllowedElements,
                                                      const TSchemaItemParameter<EnumType> & DefaultValue = TSchemaItemParameter<EnumType>());

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

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
             * This implementation checks if enumeration is represented as string
             * and tries to convert it to integer according to element-to-string
             * map provided by getEnumElementsStringRepresentation().
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(CSmartObject & Object);

            /**
             * @brief Unapply schema.
             *
             * @param Object Object to unapply schema.
             **/
            virtual void unapplySchema(CSmartObject & Object);

            /**
             * @brief The method converts a string into the value of enum EnumType
             *
             * @param str String to convert
             * @param value the resulting enum value
             * @return true if the string is converted successfully
             */
            static bool stringToEnum(const std::string& str, EnumType &value);

        private:
            /**
             * @brief Constructor.
             *
             * @param AllowedElements Set of allowed enumeration elements.
             * @param DefaultValue Default value.
             **/
            TEnumSchemaItem(const std::set<EnumType> & AllowedElements,
                            const TSchemaItemParameter<EnumType> & DefaultValue);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            TEnumSchemaItem(const TEnumSchemaItem<EnumType> & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            TEnumSchemaItem & operator =(const TEnumSchemaItem<EnumType> & Other);

            /**
             * @brief Get string representation of enumeration elements.
             *
             * @return Map of enum element to its string representation.
             **/
            static const std::map<EnumType, std::string> & getEnumElementsStringRepresentation(void);

            /**
             * @brief Set of allowed enumeration elements.
             **/
            const std::set<EnumType> mAllowedElements;

            /**
             * @brief Default value.
             **/
            const TSchemaItemParameter<EnumType> mDefaultValue;
        };
    }
}

template <typename EnumType>
utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType> > NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::create(const std::set<EnumType> & AllowedElements,
                                                                                                                                                          const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<EnumType> & DefaultValue)
{
    return new NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>(AllowedElements, DefaultValue);
}

template <typename EnumType>
NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Integer == Object.getType())
    {
        if (mAllowedElements.end() != mAllowedElements.find(static_cast<EnumType>((int)Object)))
        {
            result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;
        }
        else
        {
            result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
        }
    }
    else
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

template <typename EnumType>
bool NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    bool result = false;
    EnumType value;

    if (true == mDefaultValue.getValue(value))
    {
        Object = static_cast<int>(value);
        result = true;
    }

    return result;
}

template <typename EnumType>
void NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_String == Object.getType())
    {
        std::string stringValue = Object;
        const std::map<EnumType, std::string> elementsStringRepresentation = getEnumElementsStringRepresentation();

        for (typename std::map<EnumType, std::string>::const_iterator i = elementsStringRepresentation.begin(); i != elementsStringRepresentation.end(); ++i)
        {
            if (i->second == stringValue)
            {
                Object = static_cast<int>(i->first);
                break;
            }
        }
    }
}

template <typename EnumType>
void NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Integer == Object.getType())
    {
        int integerValue = Object;
        const std::map<EnumType, std::string> elementsStringRepresentation = getEnumElementsStringRepresentation();
        typename std::map<EnumType, std::string>::const_iterator i = elementsStringRepresentation.find(static_cast<EnumType>(integerValue));

        if (i != elementsStringRepresentation.end())
        {
            Object = i->second;
        }
    }
}

template <typename EnumType>
NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::TEnumSchemaItem(const std::set<EnumType> & AllowedElements,
                                                                      const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<EnumType> & DefaultValue):
mAllowedElements(AllowedElements),
mDefaultValue(DefaultValue)
{
}

template<typename EnumType>
bool NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::stringToEnum(const std::string& str, EnumType &value)
{
    bool result = false;
    std::map<EnumType, std::string> enumMap =
            NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::getEnumElementsStringRepresentation();

    for (typename std::map<EnumType, std::string>::const_iterator it = enumMap.begin(); it != enumMap.end(); ++it)
    {
        if (str == it->second)
        {
            value = it->first;
            result = true;
            break;
        }
    }

    return result;
}

#endif
