#ifndef __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__
#define __TRANSPORTMANAGER_CTRANSPORTMANAGER_HPP__

#include "TransportManager/ITransportManager.hpp"
#include "IDeviceAdapterListener.hpp"
#include "IHandleGenerator.hpp"
#include "Logger.hpp"

#include <pthread.h>
#include <vector>

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
             * @brief Start scanning for new devices.
             *
             * List of new devices will be supplied in onDeviceListUpdated callback.
             **/
            virtual void scanForNewDevices(void);

            /**
             * @brief Connect to all applications discovered on device.
             *
             * @param DeviceHandle Handle of device to connect to.
             **/
            virtual void connectDevice(const tDeviceHandle DeviceHandle);

            /**
             * @brief Disconnect from all applications connected on device.
             *
             * @param DeviceHandle Handle of device to disconnect from.
             **/
            virtual void disconnectDevice(const tDeviceHandle DeviceHandle);

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
             * @param DataSize Size of data in bytes.
             *
             * @return Frame sequence number. May be used to identify
             *         this frame when send result callback is received.
             **/
            virtual int sendFrame(tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

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

            // Handling Device Adapters callbacks

            /**
             * @brief Available devices list updated callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param DeviceList New list of available devices.
             **/
            virtual void onDeviceListUpdated(IDeviceAdapter * DeviceAdapter, const tInternalDeviceList & DeviceList);

            /**
             * @brief Application connected callback.
             *
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationConnected(const SDeviceInfo & ConnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Application disconnected callback.
             *
             * @param ConnectedDevice DeviceInfo of device with connected application.
             * @param Connection Connection handle.
             **/
            virtual void onApplicationDisconnected(const SDeviceInfo & DisconnectedDevice, const tConnectionHandle ConnectionHandle);

            /**
             * @brief Frame received callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param Data Received frame data.
             * @param DataSize Size of received data in bytes.
             *
             * @note  %Data pointed by parameter Data is valid only until
             *        this callback returns control to the caller. Callback
             *        handler is responsible to copy this data to its own
             *        location if it needs the data after return.
             **/
            virtual void onFrameReceived(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize);

            /**
             * @brief Frame send completed callback.
             *
             * @param DeviceAdapter Calling device adapter.
             * @param ConnectionHandle Connection handle.
             * @param FrameSequenceNumber Sequence numbere of frame that was previously returned by ITransportManager::sendFrame.
             * @param SendStatus Result status.
             **/
            virtual void onFrameSendCompleted(IDeviceAdapter * DeviceAdapter, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus);

        protected:

            /**
             * @brief Enumeration that represents type of the device listener callback
             *        that must be called
             **/
            enum EDeviceListenerCallbackType
            {
                DeviceListenerCallbackType_DeviceListUpdated = 0,
                DeviceListenerCallbackType_ApplicationConnected = 1,
                DeviceListenerCallbackType_ApplicationDisconnected = 2
            };

            /**
             * @brief Enumeration that represents type of the data listener callback
             *        that must be called
             **/
            enum EDataListenerCallbackType
            {
                DataListenerCallbackType_FrameReceived = 0,
                DataListenerCallbackType_FrameSendCompleted = 1
            };

            /**
             * @brief Device listener callback information
             *
             * Used to store data for calling device listeners
             **/
            struct SDeviceListenerCallback
            {
                /**
                 * @brief Constructor for creating DeviceListUpdated callback.
                 *
                 * @param CallbackType Type of the callback
                 * @param DeviceList Device list
                 **/
                SDeviceListenerCallback(EDeviceListenerCallbackType CallbackType, const tDeviceList & DeviceList);

                /**
                 * @brief Constructor for creating ApplicationConnected and ApplicationDisconnected callbacks
                 *
                 * @param CallbackType Type of the callback
                 * @param DeviceInfo Device information
                 * @param ConnectionHandle Connection handle
                 **/
                SDeviceListenerCallback(EDeviceListenerCallbackType CallbackType, const SDeviceInfo & DeviceInfo, const tConnectionHandle & ConnectionHandle);

                /**
                 * @brief Destructor.
                 *
                 **/
                ~SDeviceListenerCallback(void);

                /**
                 * @brief Type of the callback that must be called
                 **/
                EDeviceListenerCallbackType mCallbackType;

                /**
                 * @brief Device list (will be sent in the DeviceListUpdated callback)
                 **/
                tDeviceList mDeviceList;

                /**
                 * @brief Device information (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                SDeviceInfo mDeviceInfo;

                /**
                 * @brief Connection handle (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                tConnectionHandle mConnectionHandle;
            };

            /**
             * @brief Data listener callback information
             *
             * Used to store data for calling data listeners
             **/
            struct SDataListenerCallback
            {
                /**
                 * @brief Constructor for creating FrameReceived callback.
                 *
                 * @param CallbackType Type of the callback
                 * @param ConnectionHandle Connection handle
                 * @param Data Data
                 * @param DataSize Size of the data
                 **/
                SDataListenerCallback(NsAppLink::NsTransportManager::CTransportManager::EDataListenerCallbackType CallbackType, NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, uint8_t* Data, size_t DataSize);

                /**
                 * @brief Constructor for creating FrameSendCompleted callback
                 *
                 * @param CallbackType Type of the callback
                 * @param ConnectionHandle Connection handle
                 * @param FrameSequenceNumber Sequence Number
                 * @param SendStatus Status of operation
                 **/
                SDataListenerCallback(EDataListenerCallbackType CallbackType, tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus);

                /**
                 * @brief Destructor.
                 *
                 **/
                ~SDataListenerCallback(void);

                /**
                 * @brief Type of the callback that must be called
                 **/
                EDataListenerCallbackType mCallbackType;

                /**
                 * @brief Connection handle (will be sent in the ApplicationConnected and ApplicationDisconnected callbacks)
                 **/
                tConnectionHandle mConnectionHandle;

                /**
                 * @brief Data to send
                 **/
                uint8_t * mData;

                /**
                 * @brief Size of the data
                 **/
                size_t mDataSize;

                /**
                 * @brief Sequence number
                 **/
                int mFrameSequenceNumber;

                /**
                 * @brief Operation status
                 **/
                ESendStatus mSendStatus;
            };

            /**
             * @brief Struct that contains startup params for data-threads
             **/
            struct SDataThreadStartupParams
            {
                /**
                 * @brief Constructor
                 *
                 * @param TransportManager Pointer to the Transport Manager
                 * @param ConnectionHandle Connection Handle
                 **/
                SDataThreadStartupParams(NsAppLink::NsTransportManager::CTransportManager* TransportManager, NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle);

                /**
                 * @brief Pointer to Transport Manager
                 **/
                CTransportManager * mTransportManager;

                /**
                 * @brief Connection Handle
                 **/
                tConnectionHandle mConnectionHandle;
            };

            /**
             * @brief Map for storing Data Callbacks threads.
             **/
            typedef std::map<tConnectionHandle, pthread_t> tDataCallbacksThreads;

            /**
             * @brief Vector for storing Data Callbacks.
             **/
            typedef std::vector<SDataListenerCallback> tDataCallbacksVector;

            /**
             * @brief Map for storing callbacks for each connection handle
             **/
            typedef std::map<tConnectionHandle, tDataCallbacksVector*> tDataCallbacks;

            /**
             * @brief Map for storing condition variables for each connection handle
             **/
            typedef std::map<tConnectionHandle, pthread_cond_t*> tDataCallbacksConditionVariables;

            /**
             * @brief Start routine for Application-related callbacks.
             *
             * @param Data Must be pointer to CTransportManager instance.
             *
             * @return Thread return value.
             **/
            static void * applicationCallbacksThreadStartRoutine(void * Data);

            /**
             * @brief Application-related callbacks thread.
             *
             * From this thread all application-related callbacks are called
             *
             **/
            void applicationCallbacksThread();

            /**
             * @brief Start routine for Data-related callbacks.
             *
             * @param Data Must be pointer to CTransportManager instance.
             *
             * @return Thread return value.
             **/
            static void * dataCallbacksThreadStartRoutine(void * Data);

            /**
             * @brief Data-related callbacks thread.
             *
             * From this thread all data-related callbacks related to given connection handle are called
             *
             * @param ConnectionHandle Connection handle
             **/
            void dataCallbacksThread(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Starts thread for application-related callbacks
             *
             * @return bool Thread start status
             **/
            bool startApplicationCallbacksThread();

            /**
             * @brief Starts thread for application-related callbacks
             *
             * @param ConnectionHandle Connection Handle
             * @return bool
             **/
            bool startDataCallbacksThread(const tConnectionHandle ConnectionHandle);

            /**
             * @brief Check thread existence for given ConnectionHandle
             *
             * @param ConnectionHandle Connection Handle
             * @return bool
             *
             * @attention This function is not thread safe
             **/
            bool isThreadForConnectionHandleExist(const tConnectionHandle ConnectionHandle);

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

            /**
             * @brief ID of thread, which sends Application-related callbacks
             **/
            pthread_t mApplicationCallbacksThread;

            /**
             * @brief Condition variable used for device listeners callbacks synchronization
             **/
            pthread_cond_t mDeviceListenersConditionVar;

            /**
             * @brief Vector of the device listeners callbacks to call
             **/
            std::vector<SDeviceListenerCallback> mDeviceListenersCallbacks;

            /**
             * @brief Terminate flag.
             *
             * This flag is set to notify threads that they must be terminated.
             **/
            bool mTerminateFlag;

            /**
             * @brief Data callbacks for each connection handle
             **/
            tDataCallbacks mDataListenersCallbacks;

            /**
             * @brief Threads for sending data callbacks for each connection handle
             **/
            tDataCallbacksThreads mDataCallbacksThreads;

            /**
             * @brief Condition variables for each connection handle
             **/
            tDataCallbacksConditionVariables mDataCallbacksConditionVars;
        };
    }
}

#endif
