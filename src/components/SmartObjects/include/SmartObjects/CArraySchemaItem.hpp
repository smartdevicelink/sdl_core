#ifndef __SMARTOBJECT_CARRAYSCHEMAITEM_HPP__
#define __SMARTOBJECT_CARRAYSCHEMAITEM_HPP__

#include <stddef.h>

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "CAlwaysTrueSchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsSmartDeviceLink
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
             * @param MinSize Minimum allowed size.
             * @param MaxSize Maximum allowed size.
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
            virtual Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief Unapply schema.
             *
             * @param Object Object to unapply schema.
             **/
            virtual void unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             *
             * @param ElementSchemaItem SchemaItem for array elements.
             * @param MinSize Minimum allowed size.
             * @param MaxSize Maximum allowed size.
             **/
            CArraySchemaItem(const TSharedPtr<ISchemaItem> & ElementSchemaItem,
                             const TSchemaItemParameter<size_t> & MinSize,
                             const TSchemaItemParameter<size_t> & MaxSize);

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

            /**
             * @brief SchemaItem for array elements.
             **/
            const TSharedPtr<ISchemaItem> mElementSchemaItem;

            /**
             * @brief Minimum allowed size.
             **/
            const TSchemaItemParameter<size_t> mMinSize;

            /**
             * @brief Maximum allowed size.
             **/
            const TSchemaItemParameter<size_t> mMaxSize;
        };
    }
}

#endif
