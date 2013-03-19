#ifndef __SMARTOBJECT_ISCHEMA_ITEM_HPP__
#define __SMARTOBJECT_ISCHEMA_ITEM_HPP__

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
             * @return true if object is valid, false otherwise.
             **/
            virtual bool validate(const CSmartObject & Object);

            /**
             * @brief Set default value to an object.
             *
             * @param Object Object to set default value.
             *
             * @return true if default value was successfully set, false otherwise.
             **/
            virtual bool setDefaultValue(CSmartObject & Object);
        };
    }
}

#endif
