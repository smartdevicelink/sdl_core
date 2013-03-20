#ifndef __CSMARTOBJECT_VALIDATION_ERRORS_HPP__
#define __CSMARTOBJECT_VALIDATION_ERRORS_HPP__

namespace NsAppLink
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
                 * @brief General validation error
                 **/
                ERROR
            };
        }
    }
}

#endif //__CSMARTOBJECT_VALIDATION_ERRORS_HPP__