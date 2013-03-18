#ifndef __CFORMATTERJSONALRPCV2_HPP__
#define __CFORMATTERJSONALRPCV2_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

namespace NsAppLink { namespace NsJSONHandler {


    class CFormatterJsonALRPCv2
    {
    private:
        CFormatterJsonALRPCv2();
        CFormatterJsonALRPCv2(const CFormatterJsonALRPCv2&);

        static void objToJsonValue(NsAppLink::NsSmartObjects::CSmartObject &obj,
                Json::Value &value);


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

} } // namespace NsAppLink::NsJSONHandler


template<typename FunctionId, typename MessageType>
inline bool NsAppLink::NsJSONHandler::CFormatterJsonALRPCv2::fromString(
        const std::string& str,
        NsAppLink::NsSmartObjects::CSmartObject& out,
        FunctionId functionId,
        MessageType messageType)
{
    return true;
}

#endif // __CFORMATTERJSONALRPCV2_HPP__
