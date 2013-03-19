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
             * @return true if Object is valid, false otherwise.
             **/
            virtual bool validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             **/
            CStringSchemaItem(void);

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
        };
    }
}

#endif
