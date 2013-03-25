#ifndef __CFORMATTERJSONALRPCV1_HPP__
#define __CFORMATTERJSONALRPCV1_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

#include "CFormatterJsonBase.hpp"

namespace NsAppLink { namespace NsJSONHandler { namespace Formatters {


    class CFormatterJsonALRPCv1 : public CFormatterJsonBase
    {
    private:
        CFormatterJsonALRPCv1();
        CFormatterJsonALRPCv1(const CFormatterJsonALRPCv1&);

        /**
         * @brief Extracts a message type from the SmartObject
         */
        static const std::string getMessageType(NsAppLink::NsSmartObjects::CSmartObject& obj);

        /**
         * @brief Extracts a message type from the root JSON object.
         *
         * @return Type or empty string if there's no type in the JSON object.
         */
        static const std::string getMessageType(const Json::Value& root);

        // ALRPCv1 string consts
        static const std::string S_REQUEST;
        static const std::string S_RESPONSE;
        static const std::string S_PARAMETERS;
        static const std::string S_NAME;
        static const std::string S_CORRELATION_ID;

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
         *
         * @param str input JSON string
         * @param out The resulting SmartObject
         */
        static bool fromString(const std::string &str,
                NsAppLink::NsSmartObjects::CSmartObject &out);
    };

} } } // namespace NsAppLink::NsJSONHandler::Formatters


#endif // __CFORMATTERJSONALRPCV1_HPP__
