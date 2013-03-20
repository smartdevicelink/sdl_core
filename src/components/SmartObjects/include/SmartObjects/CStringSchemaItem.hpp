#ifndef __SMARTOBJECT_CSTRINGSCHEMAITEM_HPP__
#define __SMARTOBJECT_CSTRINGSCHEMAITEM_HPP__

#include <stddef.h>
#include <string>

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief String schema item.
         **/
        class CStringSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param MaxLength Maximum allowed string length.
             * @param DefaultValue Default value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CStringSchemaItem> create(const TSchemaItemParameter<size_t> & MaxLength = TSchemaItemParameter<size_t>(),
                                                        const TSchemaItemParameter<std::string> & DefaultValue = TSchemaItemParameter<std::string>());

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief Set default value to an object.
             *
             * @param Object Object to set default value.
             *
             * @return true if default value was successfully set, false otherwise.
             **/
            virtual bool setDefaultValue(CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             *
             * @param MaxLength Maximum allowed string length.
             * @param DefaultValue Default value.
             **/
            CStringSchemaItem(const TSchemaItemParameter<size_t> & MaxLength,
                              const TSchemaItemParameter<std::string> & DefaultValue);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CStringSchemaItem(const CStringSchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CStringSchemaItem & operator =(const CStringSchemaItem & Other);

            /**
             * @brief Maximum allowed string length.
             **/
            const TSchemaItemParameter<size_t> mMaxLength;

            /**
             * @brief Default value.
             **/
            const TSchemaItemParameter<std::string> mDefaultValue;
        };
    }
}

#endif
