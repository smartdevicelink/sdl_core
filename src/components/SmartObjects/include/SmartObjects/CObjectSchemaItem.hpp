#ifndef __SMARTOBJECT_COBJECTSCHEMAITEM_HPP__
#define __SMARTOBJECT_COBJECTSCHEMAITEM_HPP__

#include <map>
#include <string>

#include "TSharedPtr.hpp"
#include "ISchemaItem.hpp"
#include "TSchemaItemParameter.hpp"

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief Object schema item.
         **/
        class CObjectSchemaItem: public ISchemaItem
        {
        public:
            /**
             * @brief Object member.
             **/
            struct SMember
            {
                /**
                 * @brief Default constructor.
                 **/
                SMember(void);

                /**
                 * @brief Constructor.
                 *
                 * @param SchemaItem Member schema item.
                 * @param IsMandatory true if member is mandatory, false
                 *                    otherwise. Defaults to true.
                 **/
                SMember(const TSharedPtr<ISchemaItem> & SchemaItem,
                        const bool IsMandatory = true);

                /**
                 * @brief Member schema item.
                 **/
                TSharedPtr<ISchemaItem> mSchemaItem;

                /**
                 * @brief true if member is mandatory, false otherwise.
                 **/
                bool mIsMandatory;
            };

            /**
             * @brief Create a new schema item.
             *
             * @param Members Map of member name to SMember structure
             *                describing the object member.
             *
             * @return Shared pointer to a new schema item.
             **/
            static TSharedPtr<CObjectSchemaItem> create(const std::map<std::string, SMember> & Members);

            /**
             * @brief Validate smart object.
             *
             * @param Object Object to validate.
             *
             * @return NsSmartObjects::Errors::eType
             **/
            virtual Errors::eType validate(const NsAppLink::NsSmartObjects::CSmartObject & Object);

        private:
            /**
             * @brief Constructor.
             **/
            CObjectSchemaItem(void);

            /**
             * @brief Copy constructor.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             **/
            CObjectSchemaItem(const CObjectSchemaItem & Other);

            /**
             * @brief Assignment operator.
             *
             * Not implemented to prevent misuse.
             *
             * @param Other Other schema item.
             *
             * @return Not implemented.
             **/
            CObjectSchemaItem & operator =(const CObjectSchemaItem & Other);
        };
    }
}

#endif
