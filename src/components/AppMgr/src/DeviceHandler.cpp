#include "AppMgr/DeviceHandler.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger DeviceHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DeviceHandler"));

    /**
     * \brief Default class constructor
     */
    DeviceHandler::DeviceHandler()
    {
    }

    /**
     * \brief Default class destructor
     */
    DeviceHandler::~DeviceHandler()
    {
        clear();
    }

    /**
     * \brief add a device to a mapping
     * \param sessionKey session/connection key
     * \param device device handler
     */
    void DeviceHandler::addDevice(const int &sessionKey, const NsConnectionHandler::tDeviceHandle &device)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding a device " << device << " with a session " << sessionKey );
        mDeviceMapping.insert(DeviceMapItem(sessionKey, device));
    }

    /**
     * \brief remove a device from a mapping
     * \param sessionKey session/connection key
     */
    void DeviceHandler::removeDevice(const int &sessionKey)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing a device  with a session " << sessionKey );
        mDeviceMapping.erase(sessionKey);
    }

    /**
     * \brief remove an application from a mapping
     * \param device device to remove all associated sessions from mapping
     */
    void DeviceHandler::removeSession(const NsConnectionHandler::tDeviceHandle &device)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing all sessions associated with device " << device );
        for(DeviceMap::iterator it = mDeviceMapping.begin(); it != mDeviceMapping.end(); it++)
        {
            if(it->second == device)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Removing session " << it->first );
                mDeviceMapping.erase(it->first);
            }
        }
    }

    /**
     * \brief find a device subscribed to sesssion
     * \param sessionKey session/connection key
     * \return device handler
     */
    NsConnectionHandler::tDeviceHandle DeviceHandler::findDeviceAssignedToSession(const int &sessionKey) const
    {
        DeviceMap::const_iterator it = mDeviceMapping.find(sessionKey);
        if(it != mDeviceMapping.end())
        {
            return it->second;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Session " << sessionKey << " not found in subscribed." );
        return 0;
    }

    /**
     * \brief cleans session mapping
     */
    void DeviceHandler::clear()
    {
        mDeviceMapping.clear();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t DeviceHandler::size() const
    {
        return mDeviceMapping.size();
    }

    /**
     * \brief Copy constructor
     */
    DeviceHandler::DeviceHandler(const DeviceHandler &)
    {
    }

}
