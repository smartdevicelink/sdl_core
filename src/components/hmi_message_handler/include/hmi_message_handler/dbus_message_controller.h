/**
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

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_CONTROLLER_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_CONTROLLER_H_

#include <string>
#include <map>
#include "json/json.h"
#include "hmi_message_handler/dbus_adapter.h"

namespace hmi_message_handler {

class DBusAdapter;

class DBusMessageController : public DBusAdapter {
 public:
  DBusMessageController(const std::string& serviceName, const std::string& path);

  virtual ~DBusMessageController();

  /**
   * \brief subscribes controller to the property changing.
   * \param property property name in format ComponentName.PropertyName.
   */
  void subscribeTo(std::string property);

  /**
   * \brief send Json message.
   * \param message JSON message.
   */
  void SendJsonMessage(const Json::Value& message);

  /**
   * \brief checks is message notification or not.
   * \param root JSON message.
   * \return true if notification.
   */
  bool isNotification(const Json::Value& root);

  /**
   * \brief checks is message response or not.
   * \param root JSON message.
   * \return true if response.
   */
  bool isResponse(const Json::Value& root);

  /**
   * \brief Send data.
   * \param data data to send
   */
  void Send(const std::string& data);

  /**
   * \brief Receive data from the network.
   * \param data if data is received it will put in this reference
   * \note This method will blocked until data comes.
   */
  void Recv(std::string& data);

  /**
   * \brief pure virtual method to process response.
   * \param method method name which has been called.
   * \param root JSON message.
   */
  virtual void processResponse(std::string method, Json::Value& root) = 0;

  /**
   * \brief pure virtual method to process request.
   * \param root JSON message.
   */
  virtual void processRequest(Json::Value& root) = 0;

  /**
   * \brief Process notification message.
   * \brief Notify subscribers about property change.
   * expected notification format example:
   * \code
   * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
   * \endcode
   * \param root JSON message.
   */
  virtual void processNotification(Json::Value& root) = 0;

  /**
  * \brief Method for receiving thread.
  */
  void* MethodForReceiverThread(void * arg);

 private:
  /**
   * \brief Start value of id's diapason.
   */
  int mControllersIdStart;

  /**
  * \brief Already sent messages Methods to recognize esponses: MessageId:MethodName.
  */
  std::map<std::string, std::string> mWaitResponseQueue;

  /**
   * \brief JSON reader.
   */
  Json::Reader m_reader;

  /**
   * \brief JSON writer.
   */
  Json::FastWriter m_writer;

  /**
   * \brief JSON writer.
   */
  Json::FastWriter m_receiverWriter;

  /**
   * \brief Method for receiving messages without tcp packeting.
   * \param message received data
   */
  void onMessageReceived(Json::Value message);

  /**
   * \brief Checks message.
   * \param root JSON message.
   * \param error JSON message to fill in case of any errors.
   * \return true if message is good.
   */
  bool checkMessage(Json::Value& root, Json::Value& error);

  /**
   * \brief searches Method by id in mWaitResponseQueue.
   * \param id id of incoming JSON message.
   * \return string method name or "" in case not found.
   */
  std::string findMethodById(std::string id);
};

}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_CONTROLLER_H_
