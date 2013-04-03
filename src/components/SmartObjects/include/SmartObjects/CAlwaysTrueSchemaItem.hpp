#ifndef __SMARTOBJECT_CALWAYSTRUEVALIDATOR_HPP__
#define __SMARTOBJECT_CALWAYSTRUEVALIDATOR_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Always true schema item.
         **/
        class CAlwaysTrueSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CAlwaysTrueSchemaItem> create(void);

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             **/
            CAlwaysTrueSchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CAlwaysTrueSchemaItem(const CAlwaysTrueSchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CAlwaysTrueSchemaItem & operator =(const CAlwaysTrueSchemaItem & Other);
        };
    }
}

#endif
