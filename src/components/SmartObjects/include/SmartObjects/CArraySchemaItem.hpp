#ifndef __SMARTOBJECT_CARRAYSCHEMAITEM_HPP__
#define __SMARTOBJECT_CARRAYSCHEMAITEM_HPP__

#include <stddef.h>

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "CAlwaysTrueSchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Array schema item.
         **/
        class CArraySchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param ElementSchemaItem SchemaItem for array elements.
             * @param MinSize Minimum allowed value.
             * @param MaxSize Maximum allowed value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CArraySchemaItem> create(const TSharedPtr<ISchemaItem> & ElementSchemaItem = CAlwaysTrueSchemaItem::create(),
                                                       const TSchemaItemParameter<size_t> & MinSize = TSchemaItemParameter<size_t>(),
                                                       const TSchemaItemParameter<size_t> & MaxSize = TSchemaItemParameter<size_t>());

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             **/
            CArraySchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CArraySchemaItem(const CArraySchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CArraySchemaItem & operator =(const CArraySchemaItem & Other);
        };
    }
}

#endif
