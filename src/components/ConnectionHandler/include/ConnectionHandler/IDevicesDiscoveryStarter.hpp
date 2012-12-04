/**
 * \file IDevicesDiscoveryStarter.hpp
 * \brief Starter of devices discovering process.
 * \author AKara
 */

#ifndef IDEVICESDISCOVERYSTARTER_H 
#define IDEVICESDISCOVERYSTARTER_H 

/**
 * \namespace NsConnectionHandler
 * \brief AppLink ConnectionHandler namespace.
 */ 
namespace NsConnectionHandler
{
    /**
    * \class IDevicesDiscoveryStarter
    * \brief Starter of devices discovering process class
    */
    class IDevicesDiscoveryStarter
    {
    public:
        /**
         * \brief Method which should start devices discoveryng
         */
        virtual void startDevicesDiscovery()=0;

    protected:
        /**
         * \brief Destructor
         */
        virtual ~IDevicesDiscoveryStarter() {};
    };
}/* namespace NsConnectionHandler */

#endif /* IDEVICESDISCOVERYSTARTER_H */