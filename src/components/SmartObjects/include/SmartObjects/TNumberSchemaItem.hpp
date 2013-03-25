#ifndef __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__
#define __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__

#include "CSmartObject.hpp"
#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
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
            static TSharedPtr<TNumberSchemaItem> create(const TSchemaItemParameter<NumberType> & MinValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & MaxValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & DefaultValue = TSchemaItemParameter<NumberType>());

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief Set default value to an object.
             *
             * @param Object Object to set default value.
             *
             * @return true if default value was successfully set, false otherwise.
             **/
            virtual bool setDefaultValue(CSmartObject & Object);

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
         * @brief Specialization of getSmartType for double.
         *
         * @return SmartType_Double.
         **/
        template <>
        SmartType TNumberSchemaItem<double>::getSmartType(void);
    }
}

template <typename NumberType>
NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType> > NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType>::create(const NsAppLink::NsSmartObjects::TSchemaItemParameter<NumberType> & MinValue,
                                                                                                                                                                  const NsAppLink::NsSmartObjects::TSchemaItemParameter<NumberType> & MaxValue,
                                                                                                                                                                  const NsAppLink::NsSmartObjects::TSchemaItemParameter<NumberType> & DefaultValue)
{
    return new NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType>(MinValue, MaxValue, DefaultValue);
}

template <typename NumberType>
NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType>::validate(const NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    NsAppLink::NsSmartObjects::Errors::eType result = NsAppLink::NsSmartObjects::Errors::ERROR;

    if (getSmartType() == Object.getType())
    {
        result = NsAppLink::NsSmartObjects::Errors::OK;
        NumberType value = Object;
        NumberType rangeLimit;

        if (true == mMinValue.getValue(rangeLimit))
        {
            if (value < rangeLimit)
            {
                result = NsAppLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if ((NsAppLink::NsSmartObjects::Errors::OK == result) &&
            (true == mMaxValue.getValue(rangeLimit)))
        {
            if (value > rangeLimit)
            {
                result = NsAppLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }
    }
    else
    {
        result = NsAppLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

template <typename NumberType>
bool NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType>::setDefaultValue(CSmartObject & Object)
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
NsAppLink::NsSmartObjects::TNumberSchemaItem<NumberType>::TNumberSchemaItem(const TSchemaItemParameter<NumberType> & MinValue,
                                                                            const TSchemaItemParameter<NumberType> & MaxValue,
                                                                            const TSchemaItemParameter<NumberType> & DefaultValue):
mMinValue(MinValue),
mMaxValue(MaxValue),
mDefaultValue(DefaultValue)
{
}

template <>
NsAppLink::NsSmartObjects::SmartType NsAppLink::NsSmartObjects::TNumberSchemaItem<int>::getSmartType(void)
{
    return NsAppLink::NsSmartObjects::SmartType_Integer;
}

template <>
NsAppLink::NsSmartObjects::SmartType NsAppLink::NsSmartObjects::TNumberSchemaItem<double>::getSmartType(void)
{
    return NsAppLink::NsSmartObjects::SmartType_Double;
}

#endif
