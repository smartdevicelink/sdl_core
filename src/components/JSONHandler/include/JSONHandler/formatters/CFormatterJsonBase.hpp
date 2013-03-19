#ifndef __CFORMATTERJSONBASE_HPP__
#define __CFORMATTERJSONBASE_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

namespace NsAppLink { namespace NsJSONHandler { namespace Formatters {

    /**
     * @brief The base class for all JSON based formatters.
     */
    class CFormatterJsonBase
    {
    private:
        CFormatterJsonBase();
        CFormatterJsonBase(const CFormatterJsonBase&);

    protected:

        /**
         * @brief The method constructs a JSON object from the input SmartObject
         *
         * @param obj Input SmartObject. Can contain a complex structure of objects.
         * @param value The resulting JSON object. It has the same structure as the input SmartObject.
         */
        static void objToJsonValue(NsAppLink::NsSmartObjects::CSmartObject &obj,
                Json::Value &value);

        /**
         * @brief The method constructs a SmartObject from the input JSON object
         *
         * @param value Input JSON object.
         * @param obj The resulting SmartObject.
         */
        static void jsonValueToObj(const Json::Value &value,
                NsAppLink::NsSmartObjects::CSmartObject &obj);
    };


} } } // namespace NsAppLink::NsJSONHandler::Formatters

#endif // __CFORMATTERJSONBASE_HPP__
