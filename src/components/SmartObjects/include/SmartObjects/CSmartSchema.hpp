#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

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
            CSmartSchema(NsAppLink::NsSmartObjects::Validation::IValidator validator);

            bool validate(const NsAppLink::NsSmartObjects::CSmartObject& object);
            void setValidator(NsAppLink::NsSmartObjects::Validation::IValidator validator);

        protected:
            NsAppLink::NsSmartObjects::Validation::IValidator mValidator;
            bool mValidatorAvailable;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__