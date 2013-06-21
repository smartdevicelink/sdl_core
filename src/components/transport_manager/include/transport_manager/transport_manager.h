/**
 * \file transport_manager.h
 * \brief Class transport_manager header.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER

#include "device_adapter.h"


namespace transport_manager
{
/**
 * @brief type for
 *
 * @see @ref components_transportmanager_client_connection_management
 **/
typedef int SessionID;

/**
 * @brief type for
 *
 * @see @ref components_transportmanager_client_connection_management
 **/
typedef int EventType;

/**
 * @brief Interface of transport manager.
 * @interface TransportManager
 **/
class TransportManager
{
public:
	/**
	 * @brief Destructor.
	 **/
	virtual ~TransportManager(void);

	/**
	 * @brief Start scanning for new devices.
	 *
	 *
	 * @see @ref components_transportmanager_client_device_management
	 **/
	virtual void searchDevices(void) const = 0;

	/**
	 * @brief Connect to all applications discovered on device.
	 *
	 * @param DeviceHandle Handle of device to connect to.
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void connectDevice(const SessionID session_id) = 0;

	/**
	 * @brief Disconnect from all applications connected on device.
	 *
	 * @param DeviceHandle Handle of device to disconnect from.
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void disconnectDevice(const SessionID session_id) = 0;

	/**
	 * @brief post new mesage into TM's queue
	 *
	 * @param new message container
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void postMessage(const application_manager::Message message) = 0;

	/**
	 * @brief adds new call back function for specified event type
	 *
	 * @param event type, function address
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void addEventListener(const EventType event_type, const int *(Callback)(int *Data)) = 0;

	/**
	 * @brief set new module that will process errors
	 *
	 * @param error handler
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void set_error_handler(const int ErrorHandler) = 0;

	/**
	 * @brief set new module that will handle messages
	 *
	 * @param message container
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void set_message_container(const int MessageContainer) = 0;

	/**
	 * @brief set new module that will exchange data in thread safe way
	 *
	 * @param data transmitter
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void set_data_transmitter(const int DataTransmitter) = 0;

	/**
	 * @brief add new device adapter
	 *
	 * @param device adapter
	 *
	 * @see @ref components_transportmanager_client_connection_management
	 **/
	virtual void addDeviceAdapter(DeviceAdapter *device_adapter) = 0;

};
}

#endif
