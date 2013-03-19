#ifndef __SMARTOBJECT_VALIDATION_TVALIDATORPARAMETER_HPP__
#define __SMARTOBJECT_VALIDATION_TVALIDATORPARAMETER_HPP__

namespace NsAppLink
{
    namespace NsSmartObjects
    {
        /**
         * @brief SchemaItem parameter.
         *
         * Container for schema item parameter such as min/max value, size,
         * etc. which may be unspecified.
         *
         * @tparam ParameterType Type of a parameter.
         */
        template <typename ParameterType>
        class TSchemaItemParameter
        {
        public:
            /**
             * @brief Constructor.
             *
             * Initialize unspecified parameter.
             **/
            TSchemaItemParameter(void);

            /**
             * @brief Constructor.
             *
             * Initialize parameter with specified value.
             *
             * @param ParameterValue Value of a parameter.
             **/
            TSchemaItemParameter(ParameterType ParameterValue);

            /**
             * @brief Get parameter value.
             *
             * Get the value of the parameter if it is specified.
             *
             * @param Value Reference to variable that must receive the
             *              value of the parameter. If parameter is not
             *              specified this variable is not modified.
             *
             * @return true if the parameter is specified and its value
             *         has been stored in Value variable, false otherwise.
             **/
            bool getValue(ParameterType & Value) const;
        };
    }
}

#endif
