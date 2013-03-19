#ifndef __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__
#define __SMARTOBJECT_TNUMBERSCHEMAITEM_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Number schema item.
         *
         * @tparam NumberType Number type.
         **/
        template <typename NumberType>
        class TNumberSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param MinValue Minimum allowed value.
             * @param MaxValue Maximum allowed value.
             * @param DefaultValue Default value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<TNumberSchemaItem> create(const TSchemaItemParameter<NumberType> & MinValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & MaxValue = TSchemaItemParameter<NumberType>(),
                                                        const TSchemaItemParameter<NumberType> & DefaultValue = TSchemaItemParameter<NumberType>());

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
            TNumberSchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            TNumberSchemaItem(const TNumberSchemaItem<NumberType> & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            TNumberSchemaItem & operator =(const TNumberSchemaItem<NumberType> & Other);
        };
    }
}

#endif
