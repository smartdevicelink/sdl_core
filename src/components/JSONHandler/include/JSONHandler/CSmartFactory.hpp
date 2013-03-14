#ifndef __CSMARTFACTORY_HPP__
#define __CSMARTFACTORY_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CSmartSchema.hpp"
#include <map>

namespace NsAppLink
{
    namespace NsJSONHandler
    {
        template <class FunctionIdEnum, class MessageTypeEnum>
        struct SmartSchemaKey
        {
            FunctionIdEnum functionId;
            MessageTypeEnum messageType;

            SmartSchemaKey(FunctionIdEnum functionIdParam, MessageTypeEnum messageTypeParam);
        };


        template <class FunctionIdEnum, class MessageTypeEnum>
        class CSmartFactory
        {
        public:
            CSmartFactory();
            bool attachSchema(NsAppLink::NsSmartObjects::CSmartObject& object);

        protected:

            typedef std::map<SmartSchemaKey<FunctionIdEnum, MessageTypeEnum>, NsAppLink::NsSmartObjects::CSmartSchema> SchemaMap;
            SchemaMap mSchemas;
        };

        template <class FunctionIdEnum, class MessageTypeEnum>
        CSmartFactory<FunctionIdEnum, MessageTypeEnum>::CSmartFactory()
        : mSchemas()
        {
        }

        template <class FunctionIdEnum, class MessageTypeEnum>
        bool CSmartFactory<FunctionIdEnum, MessageTypeEnum>::attachSchema(NsAppLink::NsSmartObjects::CSmartObject& object)
        {
            if(false == object.keyExists("params")) return false;
            if(false == object["params"].keyExists("msg_type")) return false;
            if(false == object["params"].keyExists("function_id")) return false;

            MessageTypeEnum msgtype((MessageTypeEnum)(int)object["params"]["msg_type"]);
            FunctionIdEnum fid((FunctionIdEnum)(int)object["params"]["function_id"]);

            SmartSchemaKey<FunctionIdEnum, MessageTypeEnum> key(fid, msgtype);

            typename SchemaMap::iterator schemaIterator = mSchemas.find(key);

            if(schemaIterator == mSchemas.end())
            {
                // Schema was not found
                return false;
            }

            object.setSchema(schemaIterator->second);

            return true;
        }

        template <class FunctionIdEnum, class MessageTypeEnum>
        SmartSchemaKey<FunctionIdEnum, MessageTypeEnum>::SmartSchemaKey(FunctionIdEnum functionIdParam, MessageTypeEnum messageTypeParam)
        : functionId(functionIdParam)
        , messageType(messageTypeParam)
        {

        }


        template <class FunctionIdEnum, class MessageTypeEnum>
        bool operator<(const SmartSchemaKey< FunctionIdEnum, MessageTypeEnum >& l, const SmartSchemaKey< FunctionIdEnum, MessageTypeEnum >& r)
        {
            if (l.functionId < r.functionId) return true;
            if (l.functionId > r.functionId) return false;

            if (l.messageType < r.messageType) return true;
            if (l.messageType > r.messageType) return false;

            return false;
        }
    }
}
#endif //__CSMARTFACTORY_HPP__
