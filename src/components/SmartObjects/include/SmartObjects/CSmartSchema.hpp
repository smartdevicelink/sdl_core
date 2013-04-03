#ifndef __CSMARTSCHEMA_HPP__
#define __CSMARTSCHEMA_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsSmartDeviceLink
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
            CSmartSchema(TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Validate smart object.
             *
             * @param Object SmartObject to validate.
             *
             * @return Result of validation.
             */
            Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& Object);

            /**
             * @brief Set new root schema item.
             *
             * @param SchemaItem Root schema item.
             */
            void setSchemaItem(TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> SchemaItem);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

            /**
             * @brief The reverse SmartObject conversion using schema.
             *
             * @param object Object to convert.
             */
            virtual void unapplySchema(NsSmartDeviceLink::NsSmartObjects::CSmartObject& object);

        protected:

            /**
             * @brief Root schema item.
             */
            TSharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> mSchemaItem;
        };
    }
}

#endif //__CSMARTSCHEMA_HPP__
