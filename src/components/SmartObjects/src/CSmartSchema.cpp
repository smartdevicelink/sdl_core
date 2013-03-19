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

bool NsAppLink::NsSmartObjects::CSmartSchema::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return mSchemaItem->validate(object);
}

void NsAppLink::NsSmartObjects::CSmartSchema::setSchemaItem(NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem)
{
    mSchemaItem = SchemaItem;
}
