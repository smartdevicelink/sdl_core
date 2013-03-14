#ifndef __CSMARTOBJECT_VALIDATION_VALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_VALIDATOR_HPP__

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        namespace Validation
        {
            class IValidator
            {
            public:
                virtual bool validate(const CSmartObject& object);
            };
        }
    }
}

#endif //__CSMARTOBJECT_VALIDATION_VALIDATOR_HPP__