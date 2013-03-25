#include "SmartObjects/CAlwaysFalseSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember::SMember(void):
mSchemaItem(NsAppLink::NsSmartObjects::CAlwaysFalseSchemaItem::create()),
mIsMandatory(true)
{
}

NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember::SMember(const NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CObjectSchemaItem::ISchemaItem> & SchemaItem,
                                                               const bool IsMandatory):
mSchemaItem(SchemaItem),
mIsMandatory(IsMandatory)
{
}

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CObjectSchemaItem> NsAppLink::NsSmartObjects::CObjectSchemaItem::create(const std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember> & Members)
{
    return new NsAppLink::NsSmartObjects::CObjectSchemaItem(Members);
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CObjectSchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    NsAppLink::NsSmartObjects::Errors::eType result = NsAppLink::NsSmartObjects::Errors::ERROR;

    if (NsAppLink::NsSmartObjects::SmartType_Map == Object.getType())
    {
        result = NsAppLink::NsSmartObjects::Errors::OK;
        const std::set<std::string> objectKeys = Object.enumerate();

        for (std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember>::const_iterator i = mMembers.begin(); i != mMembers.end(); ++i)
        {
            if (objectKeys.end() != objectKeys.find(i->first))
            {
                result = i->second.mSchemaItem->validate(Object.getElement(i->first));
            }
            else
            {
                if (true == i->second.mIsMandatory)
                {
                    result = NsAppLink::NsSmartObjects::Errors::MISSING_MANDATORY_PARAMETER;
                }
            }

            if (NsAppLink::NsSmartObjects::Errors::OK != result)
            {
                break;
            }
        }

        if (NsAppLink::NsSmartObjects::Errors::OK == result)
        {
            for (std::set<std::string>::const_iterator k = objectKeys.begin(); k != objectKeys.end(); ++k)
            {
                if (mMembers.end() == mMembers.find(*k))
                {
                    result = NsAppLink::NsSmartObjects::Errors::UNEXPECTED_PARAMETER;
                    break;
                }
            }
        }
    }
    else
    {
        result = NsAppLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

void NsAppLink::NsSmartObjects::CObjectSchemaItem::applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsAppLink::NsSmartObjects::SmartType_Map == Object.getType())
    {
        for (std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember>::const_iterator i = mMembers.begin(); i != mMembers.end(); ++i)
        {
            if (true == Object.keyExists(i->first))
            {
                i->second.mSchemaItem->applySchema(Object[i->first]);
            }
        }
    }
}

NsAppLink::NsSmartObjects::CObjectSchemaItem::CObjectSchemaItem(const std::map<std::string, NsAppLink::NsSmartObjects::CObjectSchemaItem::SMember> & Members):
mMembers(Members)
{
}
