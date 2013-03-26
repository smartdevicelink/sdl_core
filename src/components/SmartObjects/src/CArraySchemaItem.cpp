#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::CArraySchemaItem> NsAppLink::NsSmartObjects::CArraySchemaItem::create(const NsAppLink::NsSmartObjects::TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> & ElementSchemaItem,
                                                                                                                                       const NsAppLink::NsSmartObjects::TSchemaItemParameter<size_t> & MinSize,
                                                                                                                                       const NsAppLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxSize)
{
    return new NsAppLink::NsSmartObjects::CArraySchemaItem(ElementSchemaItem, MinSize, MaxSize);
}

NsAppLink::NsSmartObjects::Errors::eType NsAppLink::NsSmartObjects::CArraySchemaItem::validate(const NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    NsAppLink::NsSmartObjects::Errors::eType result = NsAppLink::NsSmartObjects::Errors::ERROR;

    if (NsAppLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        result = NsAppLink::NsSmartObjects::Errors::OK;
        size_t sizeLimit;

        if (true == mMinSize.getValue(sizeLimit))
        {
            if (Object.length() < sizeLimit)
            {
                result = NsAppLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if ((NsAppLink::NsSmartObjects::Errors::OK == result) &&
            (true == mMaxSize.getValue(sizeLimit)))
        {
            if (Object.length() > sizeLimit)
            {
                result = NsAppLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if (NsAppLink::NsSmartObjects::Errors::OK == result)
        {
            for (size_t i = 0U; i < Object.length(); ++i)
            {
                result = mElementSchemaItem->validate(Object.getElement(i));

                if (NsAppLink::NsSmartObjects::Errors::OK != result)
                {
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

void NsAppLink::NsSmartObjects::CArraySchemaItem::applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->applySchema(Object[i]);
        }
    }
}

void NsAppLink::NsSmartObjects::CArraySchemaItem::unapplySchema(NsAppLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->unapplySchema(Object[i]);
        }
    }
}

NsAppLink::NsSmartObjects::CArraySchemaItem::CArraySchemaItem(const TSharedPtr<ISchemaItem> & ElementSchemaItem,
                                                              const TSchemaItemParameter<size_t> & MinSize,
                                                              const TSchemaItemParameter<size_t> & MaxSize):
mElementSchemaItem(ElementSchemaItem),
mMinSize(MinSize),
mMaxSize(MaxSize)
{
}
