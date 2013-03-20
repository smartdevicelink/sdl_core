#ifndef __SMARTOBJECT_CBOOLSCHEMAITEM_HPP__
#define __SMARTOBJECT_CBOOLSCHEMAITEM_HPP__

#include "TSchemaItemParameter.hpp"
#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Bool schema item.
         **/
        class CBoolSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @param DefaultValue Default value of a parameter.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CBoolSchemaItem> create(const TSchemaItemParameter<bool> & DefaultValue = TSchemaItemParameter<bool>());

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
             * @param DefaultValue Default value of a parameter.
             **/
            CBoolSchemaItem(const TSchemaItemParameter<bool> & DefaultValue);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CBoolSchemaItem(const CBoolSchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CBoolSchemaItem & operator =(const CBoolSchemaItem & Other);

            /**
             * @param DefaultValue Default value of a parameter.
             */
            const TSchemaItemParameter<bool> mDefaultValue;
        };
    }
}

#endif
