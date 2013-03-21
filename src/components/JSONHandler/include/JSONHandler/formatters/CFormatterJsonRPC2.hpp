#ifndef __CFORMATTERJSONRPC2_HPP__
#define __CFORMATTERJSONRPC2_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

#include "CFormatterJsonBase.hpp"

namespace NsAppLink { namespace NsJSONHandler { namespace Formatters {


    class CFormatterJsonRPC2 : public CFormatterJsonBase
    {
    private:
        CFormatterJsonRPC2();
        CFormatterJsonRPC2(const CFormatterJsonRPC2&);

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


#endif // __CFORMATTERJSONRPC2_HPP__
