/**
 * \file ConnectionHandler.hpp
 * \brief Connection handler interface class.
 *
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

#ifndef SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_ICONNECTIONHANDLER_H_
#define SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_ICONNECTIONHANDLER_H_

#include "TransportManager/ITransportManagerDeviceListener.hpp"
#include "TransportManager/SDeviceInfo.hpp"
#include "ProtocolHandler/ISessionObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"
#include "ConnectionHandler/CDevice.hpp"
#include "ConnectionHandler/CConnection.hpp"
#include "ConnectionHandler/IDevicesDiscoveryStarter.hpp"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {
/**
 * \class ConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler interface class
 */
class ConnectionHandler {
 public:
  /**
   * \brief Sets observer pointer for ConnectionHandler.
   * \param observer Pointer to observer object.
   **/
  virtual void set_connection_handler_observer(
      ConnectionHandlerObserver * observer)=0;

  /**
   * \brief Sets pointer to TransportManager.
   * \param transportManager Pointer to TransportManager object.
   **/
  virtual void set_transport_manager(
      NsSmartDeviceLink::NsTransportManager::ITransportManager * transport_manager)=0;

  virtual void StartTransportManager() = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~ConnectionHandler() {
  }
  ;

};
}/* namespace connection_handler */

#endif /* SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_ICONNECTIONHANDLER_H_ */
