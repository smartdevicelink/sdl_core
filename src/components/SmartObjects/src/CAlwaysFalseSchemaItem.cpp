#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::CAlwaysFalseSchemaItem(void)
{
}

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem> NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::create(void)
{
    return new CAlwaysFalseSchemaItem();
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CAlwaysFalseSchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object)
{
    return NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;
}
