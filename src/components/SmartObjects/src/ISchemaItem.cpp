#include "SmartObjects/ISchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::ISchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object)
{
    return NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;
}

bool NsSmartDeviceLink::NsSmartObjects::ISchemaItem::setDefaultValue(CSmartObject & Object)
{
    return false;
}

void NsSmartDeviceLink::NsSmartObjects::ISchemaItem::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
}

void NsSmartDeviceLink::NsSmartObjects::ISchemaItem::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
}
