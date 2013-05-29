/**
 * @file TNumberSchemaItem.hpp
 * @brief TNumberSchemaItem header file.
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

#ifndef __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__
#define __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__

#include "utils/shared_ptr.h"

#include "CSmartObject.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Number schema item.
         *
         * @tparam NumberType Number type.
         **/
        template <typename NumberType>
        class TNumberSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param MinValue Minimum allowed value.
             * @param MaxValue Maximum allowed value.
             * @param DefaultValue Default value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static utils::SharedPtr<TNumberSchemaItem> create(const TSchemaItemParameter<NumberType> & MinValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & MaxValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & DefaultValue = TSchemaItemParameter<NumberType>());

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
             * @brief Build smart object by smart schema having copied matched
             *        parameters from pattern smart object
             *
             * @param pattern_object pattern object
             * @param result_object object to build
             */
            virtual void BuildObjectBySchema(
              const NsSmartDeviceLink::NsSmartObjects::CSmartObject& pattern_object,
              NsSmartDeviceLink::NsSmartObjects::CSmartObject& result_object);

        private:            
            
            /**
             * @brief Constructor.
             *
             * @param MinValue Minimum allowed value.
             * @param MaxValue Maximum allowed value.
             * @param DefaultValue Default value.
             **/
            TNumberSchemaItem(const TSchemaItemParameter<NumberType> & MinValue,
                              const TSchemaItemParameter<NumberType> & MaxValue,
                              const TSchemaItemParameter<NumberType> & DefaultValue);

            /**
             * @brief Get smart type for this NumberType.
             *
             * @return Smart type for this NumberType.
             **/
            static SmartType getSmartType(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            TNumberSchemaItem(const TNumberSchemaItem<NumberType> & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            TNumberSchemaItem & operator =(const TNumberSchemaItem<NumberType> & Other);

            /**
             * @brief Minimum allowed value.
             **/
            const TSchemaItemParameter<NumberType> mMinValue;

            /**
             * @brief Maximum allowed value.
             **/
            const TSchemaItemParameter<NumberType> mMaxValue;

            /**
             * @brief Default value.
             **/
            const TSchemaItemParameter<NumberType> mDefaultValue;
        };

        /**
         * @brief Specialization of getSmartType for int.
         *
         * @return SmartType_Integer.
         **/
        template <>
        SmartType TNumberSchemaItem<int>::getSmartType(void);

        /**
         * @brief Specialization of getSmartType for long.
         *
         * @return SmartType_Integer.
         **/
        template <>
        SmartType TNumberSchemaItem<long>::getSmartType(void);

        /**
         * @brief Specialization of getSmartType for double.
         *
         * @return SmartType_Double.
         **/
        template <>
        SmartType TNumberSchemaItem<double>::getSmartType(void);
    }
}

template <typename NumberType>
utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType> > NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<NumberType> & MinValue,
                                                                                                                                                                  const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<NumberType> & MaxValue,
                                                                                                                                                                  const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<NumberType> & DefaultValue)
{
    return new NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>(MinValue, MaxValue, DefaultValue);
}

template <typename NumberType>
NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (getSmartType() == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;
        NumberType value = Object;
        NumberType rangeLimit;

        if (true == mMinValue.getValue(rangeLimit))
        {
            if (value < rangeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if ((NsSmartDeviceLink::NsSmartObjects::Errors::OK == result) &&
            (true == mMaxValue.getValue(rangeLimit)))
        {
            if (value > rangeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }
    }
    else
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

template <typename NumberType>
bool NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>::setDefaultValue(CSmartObject & Object)
{
    bool result = false;
    NumberType value;

    if (true == mDefaultValue.getValue(value))
    {
        Object = value;
        result = true;
    }

    return result;
}


template <typename NumberType>
void NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>::BuildObjectBySchema(
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject& pattern_object,
    NsSmartDeviceLink::NsSmartObjects::CSmartObject& result_object) {

  if (getSmartType() == pattern_object.getType()) {
    result_object = static_cast<NumberType>(pattern_object);
  }else {
    bool result = setDefaultValue(result_object);
    if (false == result)  {
      result_object = static_cast<NumberType>(0);
    }
  }
}

template <typename NumberType>
NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<NumberType>::TNumberSchemaItem(const TSchemaItemParameter<NumberType> & MinValue,
                                                                            const TSchemaItemParameter<NumberType> & MaxValue,
                                                                            const TSchemaItemParameter<NumberType> & DefaultValue):
mMinValue(MinValue),
mMaxValue(MaxValue),
mDefaultValue(DefaultValue)
{
}

template <>
NsSmartDeviceLink::NsSmartObjects::SmartType NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<int>::getSmartType(void)
{
    return NsSmartDeviceLink::NsSmartObjects::SmartType_Integer;
}

template <>
NsSmartDeviceLink::NsSmartObjects::SmartType NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<long>::getSmartType(void)
{
    return NsSmartDeviceLink::NsSmartObjects::SmartType_Integer;
}

template <>
NsSmartDeviceLink::NsSmartObjects::SmartType NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<double>::getSmartType(void)
{
    return NsSmartDeviceLink::NsSmartObjects::SmartType_Double;
}

#endif
