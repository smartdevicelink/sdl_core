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
