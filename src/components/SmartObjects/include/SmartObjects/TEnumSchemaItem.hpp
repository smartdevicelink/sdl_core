#ifndef __SMARTOBJECT_TENUMSCHEMAITEM_HPP__
#define __SMARTOBJECT_TENUMSCHEMAITEM_HPP__

#include <map>
#include <set>
#include <string>

#include "CSmartObject.hpp"
#include "TSharedPtr.hpp"
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
            static TSharedPtr<TEnumSchemaItem> create(const std::set<EnumType> & AllowedElements,
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
NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType> > NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<EnumType>::create(const std::set<EnumType> & AllowedElements,
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

#endif
