#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

NsAppLink::NsSmartObjects::CAlwaysTrueSchemaItem::CAlwaysTrueSchemaItem(void)
{
}

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CAlwaysTrueSchemaItem> NsAppLink::NsSmartObjects::CAlwaysTrueSchemaItem::create(void)
{
    return new CAlwaysTrueSchemaItem();
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CAlwaysTrueSchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return NsAppLink::NsSmartObjects::Errors::OK;
}