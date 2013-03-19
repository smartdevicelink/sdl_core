#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        class CSmartSchema
        {
        public:

            CSmartSchema();
            CSmartSchema(TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem);

            bool validate(const NsAppLink::NsSmartObjects::CSmartObject& object);
            void setSchemaItem(TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem);

        protected:
            TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> mSchemaItem;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__