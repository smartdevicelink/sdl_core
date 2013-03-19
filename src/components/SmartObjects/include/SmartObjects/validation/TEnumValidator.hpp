#ifndef __CSMARTOBJECT_VALIDATION_ENUMVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_ENUMVALIDATOR_HPP__

#include <set>

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
             * @brief Enumeration validator.
             *
             * @tparam EnumType Enumeration type.
             **/
            template <typename EnumType>
            class TEnumValidator: public IValidator
            {
            public:
                /**
                 * @brief Create a new validator.
                 *
                 * @param AllowedElements Set of allowed enumeration elements.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<TEnumValidator> create(const std::set<EnumType> & AllowedElements);

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
                TEnumValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                TEnumValidator(const TEnumValidator<EnumType> & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                TEnumValidator & operator =(const TEnumValidator<EnumType> & Other);
            };
        }
    }
}

#endif
