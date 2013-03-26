#include "SmartObjects/ISchemaItem.hpp"

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::ISchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return NsAppLink::NsSmartObjects::Errors::ERROR;
}

bool NsAppLink::NsSmartObjects::ISchemaItem::setDefaultValue(CSmartObject & Object)
{
    return false;
}

void NsAppLink::NsSmartObjects::ISchemaItem::applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
}

void NsAppLink::NsSmartObjects::ISchemaItem::unapplySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
}
