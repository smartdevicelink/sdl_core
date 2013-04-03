#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue)
{
    return new CBoolSchemaItem(DefaultValue);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    return (true == NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean == Object.getType()) ? NsSmartDeviceLink::NsSmartObjects::Errors::OK : NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
}

bool NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::setDefaultValue(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    bool result = false;
    bool value = false;

    if (true == mDefaultValue.getValue(value))
    {
        Object = value;
        result = true;
    }

    return result;
}

NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::CBoolSchemaItem(const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue):
mDefaultValue(DefaultValue)
{
}
