#ifndef __CFORMATTERJSONALRPCV2_HPP__
#define __CFORMATTERJSONALRPCV2_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

#include "CFormatterJsonBase.hpp"
#include "JSONHandler/CSmartFactory.hpp"

namespace NsAppLink { namespace NsJSONHandler { namespace Formatters {


    /**
     * @brief Class is used to convert SmartObjects to JSON string and vice versa.
     *
     * JSON strings is in ALPCv2 format.
     */
    class CFormatterJsonALRPCv2 : public CFormatterJsonBase
    {
    private:

        /**
         * @brief Hidden constructor.
         *
         * The class contains only static methods. Should not be instantiated.
         */
        CFormatterJsonALRPCv2();

        /**
         * @brief Hidden copy constructor.
         *
         * The class contains only static methods. Should not be instantiated.
         */
        CFormatterJsonALRPCv2(const CFormatterJsonALRPCv2&);

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
         * @param str Input JSON string in ALRPCv2 format
         * @param out Output SmartObject
         * @param functionId The corresponding field in SmartObject is filled with this param.
         * @param messageType The corresponding field in SmartObject is filled with this param.
         * @param correlatioId It's like sequence number. The corresponding field in SmartObject
         *  is filled with this param.
         * @return true if success, otherwise - false
         */
        template<typename FunctionId, typename MessageType>
        static bool fromString(const std::string &str,
                NsAppLink::NsSmartObjects::CSmartObject &out,
                FunctionId functionId,
                MessageType messageType,
                int correlationId);
    };

} } } // namespace NsAppLink::NsJSONHandler::Formatters


template<typename FunctionId, typename MessageType>
inline bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::fromString(
        const std::string& str,
        NsAppLink::NsSmartObjects::CSmartObject& out,
        FunctionId functionId,
        MessageType messageType,
        int correlationId)
{
    Json::Value root;
    Json::Reader reader;

    namespace S = NsAppLink::NsJSONHandler::strings;

    out[S::S_PARAMS][S::S_MESSAGE_TYPE] = messageType;
    out[S::S_PARAMS][S::S_FUNCTION_ID] = functionId;
    out[S::S_PARAMS][S::S_CORRELATION_ID] = correlationId;
    out[S::S_PARAMS][S::S_PROTOCOL_TYPE] = 0;
    out[S::S_PARAMS][S::S_PROTOCOL_VERSION] = 2;

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
        jsonValueToObj(root, out[NsAppLink::NsJSONHandler::strings::S_MSG_PARAMS]);
    }

    return parsingSuccessful;
}

#endif // __CFORMATTERJSONALRPCV2_HPP__
