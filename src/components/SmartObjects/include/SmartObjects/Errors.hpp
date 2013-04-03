#ifndef __CSMARTOBJECT_VALIDATION_ERRORS_HPP__
#define __CSMARTOBJECT_VALIDATION_ERRORS_HPP__

namespace NsSmartDeviceLink
{
    namespace NsSmartObjects
    {
        namespace Errors
        {
            /**
             * @brief Enumeration that provides information about SmartObject validation errors
             **/
            enum eType {
                /**
                 * @brief Success validation
                 **/
                OK = 0,

                /**
                 * @brief Invalid value.
                 **/
                INVALID_VALUE,

                /**
                 * @brief Value is out of allowed range.
                 **/
                OUT_OF_RANGE,

                /**
                 * @brief Mandatory parameter is missing.
                 **/
                MISSING_MANDATORY_PARAMETER,

                /**
                 * @brief Unexpected parameter.
                 **/
                UNEXPECTED_PARAMETER,

                /**
                 * @brief General validation error
                 **/
                ERROR
            };
        }
    }
}

#endif //__CSMARTOBJECT_VALIDATION_ERRORS_HPP__