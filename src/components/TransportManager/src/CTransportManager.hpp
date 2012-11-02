#ifndef __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__

#include "TransportManager/ITransportManager.hpp"
#include "IDeviceAdapterListener.hpp"
#include "IHandleGenerator.hpp"
#include "Logger.hpp"

#include <pthread.h>

namespace NsAppLink
{
    namespace NsTransportManager
    {
        class IDeviceAdapter;

        /**
         * @brief Transport manager implementation.
         **/
        class CTransportManager: public ITransportManager,
                                 public IDeviceAdapterListener,
                                 public IHandleGenerator
        {
        public:
            /**
             * @brief Constructor.
             **/
            CTransportManager(void);

            /**
             * @brief Destructor.
             **/
            virtual ~CTransportManager(void);

            /**
             * @brief Run transport manager.
             *
             * Must be called from startup after all references to
             * external components are initialized to start trasport manager.
             **/
            virtual void run(void);

            /**
             * @brief Get list of available devices.
             *
             * @param DeviceList Reference to a variable that will
             *                   receive device list.
             **/
            virtual void getDeviceList(tDeviceList & DeviceList) const;

            /**
             * @brief Add listener to the data-related events
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDataListener(ITransportManagerDataListener * Listener);

            /**
             * @brief Remove listener to the data-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDataListener(ITransportManagerDataListener * Listener);

            /**
             * @brief Add listener to the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void addDeviceListener(ITransportManagerDeviceListener * Listener);

            /**
             * @brief Remove listenerto the device-related events.
             *
             * @param Listener Pointer to listener.
             **/
            virtual void removeDeviceListener(ITransportManagerDeviceListener * Listener);

            /**
             * @brief Send frame.
             *
             * @param ConnectionHandle Connection handle.
             * @param Data Frame payload data.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const Blob & Data);

            /**
             * @brief Generate new device handle.
             *
             * Internal method used for generation of unique device handle
             * for device adapters.
             *
             * @return New device handle.
             **/
            virtual tDeviceHandle generateNewDeviceHandle(void);

            /**
             * @brief Generate new connection handle.
             *
             * Internal method used for generation of unique connection handle
             * for device adapters.
             *
             * @return New connection handle.
             **/
            virtual tConnectionHandle generateNewConnectionHandle(void);

        protected:
            /**
             * @brief Device adapters.
             **/
            std::vector<IDeviceAdapter*> mDeviceAdapters;

            /**
             * @brief Logger.
             **/
            const log4cplus::Logger mLogger;

            /**
             * @brief Mutex restricting access to data listeners.
             **/
            mutable pthread_mutex_t mDataListenersMutex;

            /**
             * @brief Mutex restricting access to device listeners.
             **/
            mutable pthread_mutex_t mDeviceListenersMutex;

            /**
             * @brief Mutex restricting access to new device handle generation
             **/
            mutable pthread_mutex_t mDeviceHandleGenerationMutex;

            /**
             * @brief Mutex restricting access to new conenction handle generation
             **/
            mutable pthread_mutex_t mConnectionHandleGenerationMutex;

            /**
             * @brief Data listeners
             **/
            std::vector<ITransportManagerDataListener*> mDataListeners;

            /**
             * @brief Device listeners
             **/
            std::vector<ITransportManagerDeviceListener*> mDeviceListeners;

            /**
             * @brief Last used device handle
             *
             * Used during device handle generation
             **/
            tDeviceHandle mLastUsedDeviceHandle;

            /**
             * @brief Last used connection handle
             *
             * Used during connection handle generation
             **/
            tConnectionHandle mLastUsedConnectionHandle;
        };
    }
}

#endif
