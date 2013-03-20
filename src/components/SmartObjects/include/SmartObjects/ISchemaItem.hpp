#ifndef __SMARTOBJECT_ISCHEMA_ITEM_HPP__
#define __SMARTOBJECT_ISCHEMA_ITEM_HPP__

#include "Errors.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        class CSmartObject;

        /**
         * @brief Base schema item.
         **/
        class ISchemaItem
        {
        public:
            /**
             * @brief Validate object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const CSmartObject & Object);

            /**
             * @brief Set default value to an object.
             *
             * @param Object Object to set default value.
             *
             * @return true if default value was successfully set, false otherwise.
             **/
            virtual bool setDefaultValue(CSmartObject & Object);

            /**
             * @brief Apply schema.
             *
             * @param Object Object to apply schema.
             **/
            virtual void applySchema(NsAppLink::NsSmartObjects::CSmartObject & Object);
        };
    }
}

#endif
