#ifndef __TRANSPORTMANAGER_IHANDLEGENERATOR_HPP__
#define __TRANSPORTMANAGER_IHANDLEGENERATOR_HPP__

#include "TransportManager/ITransportManager.hpp"

namespace NsAppLink
{
    namespace NsTransportManager
    {
        /**
         * @brief Interface for handle generator.
         **/
        class IHandleGenerator
        {
        public:
            /**
             * @brief Destructor.
             **/
            virtual ~IHandleGenerator(void);

            /**
             * @brief Generate new device handle.
             *
             * Internal method used for generation of unique device handle
             * for device adapters.
             *
             * @return New device handle.
             **/
            virtual tDeviceHandle generateNewDeviceHandle(void) = 0;

            /**
             * @brief Generate new connection handle.
             *
             * Internal method used for generation of unique connection handle
             * for device adapters.
             *
             * @return New connection handle.
             **/
            virtual tConnectionHandle generateNewConnectionHandle(void) = 0;
        };
    }
}

#endif
