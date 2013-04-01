#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        /**
         * @brief Smart Schema.
         */ 
        class CSmartSchema
        {
        public:

            /**
             * @brief Constructor.
             *
             * Default constructor. Creates empty schema.
             */
            CSmartSchema();

            /**
             * @brief Constructor.
             *
             * Creates schema with given root schema item.
             *
             * @param SchemaItem Root schema item.
             */
            CSmartSchema(TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Validate smart object.
             *
             * @param Object SmartObject to validate.
             *
             * @return Result of validation.
             */ 
            Errors::eType validate(const NsAppLink::NsSmartObjects::CSmartObject& Object);

            /**
             * @brief Set new root schema item.
             *
             * @param SchemaItem Root schema item.
             */
            void setSchemaItem(TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief The reverse SmartObject conversion using schema.
             *
             * @param object Object to convert.
             */
            virtual void unapplySchema(NsAppLink::NsSmartObjects::CSmartObject& object);

        protected:

            /**
             * @brief Root schema item.
             */
            TSharedPtr<NsAppLink::NsSmartObjects::ISchemaItem> mSchemaItem;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__
