#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema()
: mSchemaItem(NsAppLink::NsSmartObjects::CAlwaysTrueSchemaItem::create())
{
}

NsAppLink::NsSmartObjects::CSmartSchema::CSmartSchema(NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem)
: mSchemaItem(SchemaItem)
{

}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CSmartSchema::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return mSchemaItem->validate(object);
}

void NsAppLink::NsSmartObjects::CSmartSchema::setSchemaItem(NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem)
{
    mSchemaItem = SchemaItem;
}

void NsAppLink::NsSmartObjects::CSmartSchema::applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    mSchemaItem->applySchema(Object);
}