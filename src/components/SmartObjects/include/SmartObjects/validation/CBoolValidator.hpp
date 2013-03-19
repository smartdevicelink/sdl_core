#ifndef __SMARTOBJECT_VALIDATION_BOOLVALIDATOR_HPP__
#define __SMARTOBJECT_VALIDATION_BOOLVALIDATOR_HPP__

#include "SmartObjects/TSharedPtr.hpp"
#include "IValidator.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        namespace Validation
        {
            /**
             * @brief Bool validator.
             **/
            class CBoolValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CBoolValidator> create(void);

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
                CBoolValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CBoolValidator(const CBoolValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CBoolValidator & operator =(const CBoolValidator & Other);
            };
        }
    }
}

#endif
