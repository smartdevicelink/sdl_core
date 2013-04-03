#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

NsSmartDeviceLink::NsSmartObjects::CSmartSchema::CSmartSchema()
: mSchemaItem(NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem::create())
{
}

NsSmartDeviceLink::NsSmartObjects::CSmartSchema::CSmartSchema(NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem)
: mSchemaItem(SchemaItem)
{

}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CSmartSchema::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object)
{
    return mSchemaItem->validate(object);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::setSchemaItem(NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem)
{
    mSchemaItem = SchemaItem;
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    mSchemaItem->applySchema(Object);
}

void NsSmartDeviceLink::NsSmartObjects::CSmartSchema::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    mSchemaItem->unapplySchema(Object);
}
