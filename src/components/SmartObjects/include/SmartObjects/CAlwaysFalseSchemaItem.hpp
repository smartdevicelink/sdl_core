#ifndef __SMARTOBJECT_CALWAYSFALSESCHEMAITEM_HPP__
#define __SMARTOBJECT_CALWAYSFALSESCHEMAITEM_HPP__

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Always false schema item.
         **/
        class CAlwaysFalseSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Create a new schema item.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CAlwaysFalseSchemaItem> create(void);

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartDeviceLink::NsSmartObjects::Errors::ERROR
             **/
            virtual Errors::eType validate(const NsSmartDeviceLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             **/
            CAlwaysFalseSchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CAlwaysFalseSchemaItem(const CAlwaysFalseSchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CAlwaysFalseSchemaItem & operator =(const CAlwaysFalseSchemaItem & Other);
        };
    }
}

#endif
