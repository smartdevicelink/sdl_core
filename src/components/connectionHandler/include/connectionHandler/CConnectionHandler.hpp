/**
 * \file CConnectionHandler.hpp
 * \brief Connection handler class.
 * \Observes TransportManager and ProtocolHandler, stores information regarding connections
 * \and sessions and provides it to AppManager.
 * \author AKara
 */

#ifndef CONNECTIONHANDLER_H 
#define CONNECTIONHANDLER_H 

#include "Logger.hpp"
#include "ITransportManagerDeviceListener.hpp"
#include "SDeviceInfo.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class CConnectionHandler
    * \brief AppLink ConnectionHandler main class
    */
    class CConnectionHandler: public NsAppLink::NsTransportManager::ITransportManagerDeviceListener
    {
        public:
        static CConnectionHandler* getInstance();

        ~CConnectionHandler();
        /**
         * @brief Available devices list updated.
         *
         * Called when device scanning initiated with scanForNewDevices
         * is completed.
         *
         * @param DeviceList New list of available devices.
         **/
        virtual void onDeviceListUpdated(const NsAppLink::NsTransportManager::tDeviceList & DeviceList);

        /**
         * @brief Application connected.
         *
         * @param ConnectedDevice DeviceInfo of connected device.
         * @param Connection Connection handle.
         **/
        virtual void onApplicationConnected(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection);

        /**
         * @brief Application disconnected.
         *
         * @param DisconnectedDevice DeviceInfo of disconnected device.
         * @param Connection Connection handle.
         **/
        virtual void onApplicationDisconnected(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection);

        private:
        /**
         * \brief Default class constructor
         */
        CConnectionHandler();

        /**
         * \brief Copy constructor
         */
        CConnectionHandler(const CConnectionHandler&);

        /**
          *\brief For logging.
        */
        static log4cplus::Logger mLogger;
    };
}/* namespace NsConnectionHandler */

#endif /* CONNECTIONHANDLER_H */