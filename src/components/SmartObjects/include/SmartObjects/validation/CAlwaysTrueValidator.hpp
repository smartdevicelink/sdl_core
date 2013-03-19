#ifndef __CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__

#include "SmartObjects/TSharedPtr.hpp"
#include "IValidator.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        namespace Validation
        {
            /**
             * @brief Always true validator.
             **/
            class CAlwaysTrueValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CAlwaysTrueValidator> create(void);

                /**
                 * @brief Validate smart object.
                 *
                 * @param Object Object to validate.
                 *
                 * @return true if Object is valid, false otherwise.
                 **/
                virtual bool validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

            private:
                /**
                 * @brief Constructor.
                 **/
                CAlwaysTrueValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CAlwaysTrueValidator(const CAlwaysTrueValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CAlwaysTrueValidator & operator =(const CAlwaysTrueValidator & Other);
            };
        }
    }
}

#endif //__CSMARTOBJECT_VALIDATION_ALWAYSTRUEVALIDATOR_HPP__
