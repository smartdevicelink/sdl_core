#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CSmartObject.hpp"

NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem> NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::create(const NsSmartDeviceLink::NsSmartObjects::TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> & ElementSchemaItem,
                                                                                                                                       const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MinSize,
                                                                                                                                       const NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t> & MaxSize)
{
    return new NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem(ElementSchemaItem, MinSize, MaxSize);
}

NsSmartDeviceLink::NsSmartObjects::Errors::eType NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    NsSmartDeviceLink::NsSmartObjects::Errors::eType result = NsSmartDeviceLink::NsSmartObjects::Errors::ERROR;

    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::OK;
        size_t sizeLimit;

        if (true == mMinSize.getValue(sizeLimit))
        {
            if (Object.length() < sizeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if ((NsSmartDeviceLink::NsSmartObjects::Errors::OK == result) &&
            (true == mMaxSize.getValue(sizeLimit)))
        {
            if (Object.length() > sizeLimit)
            {
                result = NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE;
            }
        }

        if (NsSmartDeviceLink::NsSmartObjects::Errors::OK == result)
        {
            for (size_t i = 0U; i < Object.length(); ++i)
            {
                result = mElementSchemaItem->validate(Object.getElement(i));

                if (NsSmartDeviceLink::NsSmartObjects::Errors::OK != result)
                {
                    break;
                }
            }
        }
    }
    else
    {
        result = NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE;
    }

    return result;
}

void NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->applySchema(Object[i]);
        }
    }
}

void NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == Object.getType())
    {
        for (size_t i = 0U; i < Object.length(); ++i)
        {
            mElementSchemaItem->unapplySchema(Object[i]);
        }
    }
}

NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem::CArraySchemaItem(const TSharedPtr<ISchemaItem> & ElementSchemaItem,
                                                              const TSchemaItemParameter<size_t> & MinSize,
                                                              const TSchemaItemParameter<size_t> & MaxSize):
mElementSchemaItem(ElementSchemaItem),
mMinSize(MinSize),
mMaxSize(MaxSize)
{
}
