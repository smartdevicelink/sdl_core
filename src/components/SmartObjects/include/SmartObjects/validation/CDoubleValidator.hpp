#ifndef __CSMARTOBJECT_VALIDATION_DOUBLEVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_DOUBLEVALIDATOR_HPP__

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
            class CDoubleValidator: public IValidator
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
                static TSharedPtr<CDoubleValidator> create(TValidatorParameter<double> MinValue = TValidatorParameter<double>(),
                                                           TValidatorParameter<double> MaxValue = TValidatorParameter<double>());

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
                CDoubleValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CDoubleValidator(const CDoubleValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CDoubleValidator & operator =(const CDoubleValidator & Other);
            };
        }
    }
}

#endif
