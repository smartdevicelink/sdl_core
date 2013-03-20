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
            TSchemaItemParameter(const ParameterType & ParameterValue);

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

        private:
            /**
             * @brief true if parameter is specified, false otherwise.
             **/
            const bool mIsSpecified;

            /**
             * @brief Parameter value.
             **/
            const ParameterType mValue;
        };
    }
}

template <typename ParameterType>
NsAppLink::NsSmartObjects::TSchemaItemParameter<ParameterType>::TSchemaItemParameter(void):
mIsSpecified(false),
mValue()
{
}

template <typename ParameterType>
NsAppLink::NsSmartObjects::TSchemaItemParameter<ParameterType>::TSchemaItemParameter(const ParameterType & ParameterValue):
mIsSpecified(true),
mValue(ParameterValue)
{
}

template <typename ParameterType>
bool NsAppLink::NsSmartObjects::TSchemaItemParameter<ParameterType>::getValue(ParameterType & Value) const
{
    if (true == mIsSpecified)
    {
        Value = mValue;
    }

    return mIsSpecified;
}

#endif
