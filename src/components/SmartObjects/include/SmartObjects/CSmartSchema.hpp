#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

#include "SmartObjects/TSharedPtr.hpp"
#include "SmartObjects/validation/IValidator.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        class CSmartSchema
        {
        public:

            CSmartSchema();
            CSmartSchema(TSharedPtr<NsAppLink::NsSmartObjects::Validation::IValidator> validator);

            bool validate(const NsAppLink::NsSmartObjects::CSmartObject& object);
            void setValidator(TSharedPtr<NsAppLink::NsSmartObjects::Validation::IValidator> validator);

        protected:
            TSharedPtr<NsAppLink::NsSmartObjects::Validation::IValidator> mValidator;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__