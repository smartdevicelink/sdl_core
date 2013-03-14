#ifndef __CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__

#include "IValidator.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        namespace Validation
        {
            class CAlwaysTrueValidator: public IValidator
            {
            public:
                virtual bool validate(const NsAppLink::NsSmartObjects::CSmartObject& object);
            };
        }
    }
}

#endif //__CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__