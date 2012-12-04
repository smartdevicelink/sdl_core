#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "ConnectionHandler/CDevice.hpp"

#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief session-to-device map
     */
    typedef std::map<int, NsConnectionHandler::tDeviceHandle> DeviceMap;

    /**
     * \brief session-to-device map
     */
    typedef std::pair<int, NsConnectionHandler::tDeviceHandle> DeviceMapItem;

    class DeviceHandler
    {
    public:

        /**
         * \brief Default class constructor
         */
        DeviceHandler();

        /**
         * \brief Default class destructor
         */
        ~DeviceHandler();

        /**
         * \brief add a device to a mapping
         * \param sessionKey session/connection key
         * \param device device handler
         */
        void addDevice( const int& sessionKey, const NsConnectionHandler::tDeviceHandle& device );

        /**
         * \brief remove a device from a mapping
         * \param sessionKey session/connection key
         */
        void removeDevice(const int& sessionKey);

        /**
         * \brief remove an application from a mapping
         * \param device device to remove all associated sessions from mapping
         */
        void removeSession( const NsConnectionHandler::tDeviceHandle& device );

        /**
         * \brief find a device subscribed to sesssion
         * \param sessionKey session/connection key
         * \return device handler
         */
        NsConnectionHandler::tDeviceHandle findDeviceAssignedToSession(const int& sessionKey) const;

        /**
         * \brief cleans session mapping
         */
        void clear( );

        /**
         * \brief get count of items
         * \return items count
         */
        size_t size() const;

    private:

        /**
         * \brief Copy constructor
         */
        DeviceHandler(const DeviceHandler&);

        DeviceMap    mDeviceMapping;
        static log4cplus::Logger mLogger;
    };

}

#endif // DEVICEHANDLER_H
