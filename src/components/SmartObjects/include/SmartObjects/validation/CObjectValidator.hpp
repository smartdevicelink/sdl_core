#ifndef __CSMARTOBJECT_VALIDATION_OBJECTVALIDATOR_HPP__
#define __CSMARTOBJECT_VALIDATION_OBJECTVALIDATOR_HPP__

#include <map>
#include <string>

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
             * @brief Array validator.
             **/
            class CObjectValidator: public IValidator
            {
            public:
                /**
                 * @brief Object member.
                 **/
                struct SMember
                {
                    /**
                     * @brief Default constructor.
                     **/
                    SMember(void);

                    /**
                     * @brief Constructor.
                     *
                     * @param Validator Member validator.
                     * @param IsMandatory true if member is mandatory, false
                     *                    otherwise. Defaults to true.
                     **/
                    SMember(const TSharedPtr<IValidator> & Validator,
                            const bool IsMandatory = true);

                    /**
                     * @brief Member validator.
                     **/
                    TSharedPtr<IValidator> mValidator;

                    /**
                     * @brief true if member is mandatory, false otherwise.
                     **/
                    bool mIsMandatory;
                };

                /**
                 * @brief Create a new validator.
                 *
                 * @param Members Map of member name to SMember structure
                 *                describing the object member.
                 *
                 * @return Shared pointer to a new validator.
                 **/
                static TSharedPtr<CObjectValidator> create(const std::map<std::string, SMember> & Members);

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
                CObjectValidator(void);

                /**
                 * @brief Copy constructor.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 **/
                CObjectValidator(const CObjectValidator & Other);

                /**
                 * @brief Assignment operator.
                 *
                 * Not implemented to prevent misuse.
                 *
                 * @param Other Other validator.
                 *
                 * @return Not implemented.
                 **/
                CObjectValidator & operator =(const CObjectValidator & Other);
            };
        }
    }
}

#endif
