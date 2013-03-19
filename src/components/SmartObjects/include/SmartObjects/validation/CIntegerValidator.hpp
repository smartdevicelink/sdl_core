#ifndef __CSMARTOBJECT_VALIDATION_INTEGERVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_INTEGERVALIDATOR_HPP__

#include "SmartObjects/TSharedPtr.hpp"
#include "IValidator.hpp"
#include "TValidatorParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        namespace Validation
        {
            /**
             * @brief Integer validator.
             **/
            class CIntegerValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @param MinValue Minimum allowed value.
                 * @param MaxValue Maximum allowed value.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CIntegerValidator> create(TValidatorParameter<int> MinValue = TValidatorParameter<int>(),
                                                            TValidatorParameter<int> MaxValue = TValidatorParameter<int>());

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
                CIntegerValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CIntegerValidator(const CIntegerValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CIntegerValidator & operator =(const CIntegerValidator & Other);
            };
        }
    }
}

#endif
