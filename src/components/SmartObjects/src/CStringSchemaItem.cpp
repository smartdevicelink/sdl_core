#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem> NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                                                                                         const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue)
{
    return new NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem(MaxLength, DefaultValue);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_String == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;

        size_t maxLength;
        std::string value = Object;

        if (true == mMaxLength.getValue(maxLength))
        {
            if (value.size() > maxLength)
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

bool NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
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

NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::CStringSchemaItem(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxLength,
                                                                const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string> & DefaultValue):
mMaxLength(MaxLength),
mDefaultValue(DefaultValue)
{
}
