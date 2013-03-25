#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CBoolSchemaItem> NsAppLink::NsSmartObjects::CBoolSchemaItem::create(const NsAppLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue)
{
    return new CBoolSchemaItem(DefaultValue);
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CBoolSchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    return (true == NsAppLink::NsSmartObjects::SmartType_Boolean == Object.getType()) ? NsAppLink::NsSmartObjects::Errors::OK : NsAppLink::NsSmartObjects::Errors::INVALID_VALUE;
}

bool NsAppLink::NsSmartObjects::CBoolSchemaItem::setDefaultValue(NsAppLink::NsSmartObjects::CSmartObject & Object)
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

NsAppLink::NsSmartObjects::CBoolSchemaItem::CBoolSchemaItem(const NsAppLink::NsSmartObjects::TSchemaItemParameter<bool> & DefaultValue):
mDefaultValue(DefaultValue)
{
}
