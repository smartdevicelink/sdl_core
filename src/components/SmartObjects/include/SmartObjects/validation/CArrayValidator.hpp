#ifndef __CSMARTOBJECT_VALIDATION_ARRAYVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_ARRAYVALIDATOR_HPP__

#include <stddef.h>

#include "SmartObjects/TSharedPtr.hpp"
#include "IValidator.hpp"
#include "CAlwaysTrueValidator.hpp"
#include "TValidatorParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        namespace Validation
        {
            /**
             * @brief Array validator.
             **/
            class CArrayValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @param ElementValidator Validator for array elements.
                 * @param MinSize Minimum allowed value.
                 * @param MaxSize Maximum allowed value.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CArrayValidator> create(const TSharedPtr<IValidator> & ElementValidator = CAlwaysTrueValidator::create(),
                                                          TValidatorParameter<size_t> MinSize = TValidatorParameter<size_t>(),
                                                          TValidatorParameter<size_t> MaxSize = TValidatorParameter<size_t>());

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
                CArrayValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CArrayValidator(const CArrayValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CArrayValidator & operator =(const CArrayValidator & Other);
            };
        }
    }
}

#endif
