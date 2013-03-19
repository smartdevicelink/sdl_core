#ifndef __SMARTOBJECT_TENUMSCHEMAITEM_HPP__
#define __SMARTOBJECT_TENUMSCHEMAITEM_HPP__

#include <set>

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Enumeration schema item.
         *
         * @tparam EnumType Enumeration type.
         **/
        template <typename EnumType>
        class TEnumSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param AllowedElements Set of allowed enumeration elements.
             * @param DefaultValue Default value.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<TEnumSchemaItem> create(const std::set<EnumType> & AllowedElements,
                                                      const TSchemaItemParameter<EnumType> & DefaultValue = TSchemaItemParameter<EnumType>());

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
            TEnumSchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            TEnumSchemaItem(const TEnumSchemaItem<EnumType> & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            TEnumSchemaItem & operator =(const TEnumSchemaItem<EnumType> & Other);
        };
    }
}

#endif
