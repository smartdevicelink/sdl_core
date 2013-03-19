#ifndef __CFORMATTERJSONALRPCV2_HPP__
#define __CFORMATTERJSONALRPCV2_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

namespace NsAppLink { namespace NsJSONHandler { namespace Formatters {


    class CFormatterJsonALRPCv2
    {
    private:
        CFormatterJsonALRPCv2();
        CFormatterJsonALRPCv2(const CFormatterJsonALRPCv2&);

        static void objToJsonValue(NsAppLink::NsSmartObjects::CSmartObject &obj,
                Json::Value &value);

        static void jsonValueToObj(const Json::Value &value,
                NsAppLink::NsSmartObjects::CSmartObject &obj);


    public:

        // TODO: Make const input params
        /**
         * @brief Creates a JSON string from a SmartObject.
         *
         * @param obj input SmartObject
         * @param outStr resulting JSON string
         * @return true if success, false otherwise
         */
        static bool toString(NsAppLink::NsSmartObjects::CSmartObject &obj,
                std::string& outStr);


        /**
         * @brief Creates a SmartObject from a JSON string.
         */
        template<typename FunctionId, typename MessageType>
        static bool fromString(const std::string &str,
                NsAppLink::NsSmartObjects::CSmartObject &out,
                FunctionId functionId,
                MessageType messageType);
    };

} } } // namespace NsAppLink::NsJSONHandler::Formatters


template<typename FunctionId, typename MessageType>
inline bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::fromString(
        const std::string& str,
        NsAppLink::NsSmartObjects::CSmartObject& out,
        FunctionId functionId,
        MessageType messageType)
{
    Json::Value root;
    Json::Reader reader;

    // TODO: Check correct keys and values
    out["params"]["MessageType"] = messageType;
    out["params"]["FunctionId"] = functionId;

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
        jsonValueToObj(root, out["msg_params"]);
    }

    return parsingSuccessful;
}

#endif // __CFORMATTERJSONALRPCV2_HPP__
