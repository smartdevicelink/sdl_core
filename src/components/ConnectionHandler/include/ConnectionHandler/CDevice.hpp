/**
 * \file CDevice.hpp
 * \brief CDevice class.
 * Stores device information
 * \author AKara
 */

#ifndef DEVICE_H 
#define DEVICE_H 

#include "Logger.hpp"
#include <string>
#include <map>

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{

    /**
     * \brief Type for DeviceHandle
     */
    typedef int tDeviceHandle;

    /**
    * \class CDevice
    * \brief Connection class
    */
    class CDevice
    {
    public:
        /**
         * \brief Class constructor
         */
        CDevice(tDeviceHandle aDeviceHandle, std::string aUserFriendlyName);

        /**
         * \brief Destructor
         */
        ~CDevice();

        /**
         * \brief Returns device handle
         */
        tDeviceHandle getDeviceHandle() const;

        /**
         * \brief Returns user frendly device name
         */
        std::string getUserFriendlyName() const;

    private:
        /**
        * \brief Uniq device handle.
        **/
        tDeviceHandle mDeviceHandle;

        /**
        * \brief User-friendly device name.
        **/
        std::string mUserFriendlyName;

        /**
          *\brief For logging.
        */
        static log4cplus::Logger mLogger;
    };

    /**
     * \brief Type for Devices map
     */
    typedef std::map<int, CDevice> tDeviceList;

    /**
     * \brief Type for Devices map iterator
     */
    typedef std::map<int, CDevice>::iterator tDeviceListIterator;

}/* namespace NsConnectionHandler */

#endif /* DEVICE_H */