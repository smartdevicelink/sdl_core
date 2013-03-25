#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CStringSchemaItem> NsAppLink::NsSmartObjects::CStringSchemaItem::create(const NsAppLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                                                                                         const NsAppLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue)
{
    return new NsAppLink::NsSmartObjects::CStringSchemaItem(MaxLength, DefaultValue);
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CStringSchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    NsAppLink::NsSmartObjects::Errors::eType result = NsAppLink::NsSmartObjects::Errors::ERROR;

    if (NsAppLink::NsSmartObjects::SmartType_String == Object.getType())
    {
        result = NsAppLink::NsSmartObjects::Errors::OK;

        size_t maxLength;
        std::string value = Object;

        if (true == mMaxLength.getValue(maxLength))
        {
            if (value.size() > maxLength)
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

bool NsAppLink::NsSmartObjects::CStringSchemaItem::setDefaultValue(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    bool result = false;
    std::string value;

    if (true == mDefaultValue.getValue(value))
    {
        Object = value;
        result = true;
    }

    return result;
}

NsAppLink::NsSmartObjects::CStringSchemaItem::CStringSchemaItem(const NsAppLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                const NsAppLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue):
mMaxLength(MaxLength),
mDefaultValue(DefaultValue)
{
}
