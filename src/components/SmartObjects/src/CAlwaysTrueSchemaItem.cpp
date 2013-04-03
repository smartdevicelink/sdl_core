#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem::CAlwaysTrueSchemaItem(void)
{
}

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem> NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem::create(void)
{
    return new CAlwaysTrueSchemaItem();
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object)
{
    return NsSmartDeviceLink::NsSmartObjects::Errors::OK;
}