#ifndef __CSMARTOBJECT_VALIDATION_STRINGVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_STRINGVALIDATOR_HPP__

#include <stddef.h>

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
             * @brief Double validator.
             **/
            class CStringValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @param MaxLength Maximum allowed string length.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CStringValidator> create(TValidatorParameter<size_t> MaxLength = TValidatorParameter<size_t>());

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
                CStringValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CStringValidator(const CStringValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CStringValidator & operator =(const CStringValidator & Other);
            };
        }
    }
}

#endif
