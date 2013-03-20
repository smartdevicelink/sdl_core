#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"

NsAppLink::NsSmartObjects::CAlwaysFalseSchemaItem::CAlwaysFalseSchemaItem(void)
{
}

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CAlwaysFalseSchemaItem> NsAppLink::NsSmartObjects::CAlwaysFalseSchemaItem::create(void)
{
    return new CAlwaysFalseSchemaItem();
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CAlwaysFalseSchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject& object)
{
    return NsAppLink::NsSmartObjects::Errors::ERROR;
}
