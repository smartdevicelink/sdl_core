/**
 * \file mb_controller.cpp
 * \brief MessageBroker Controller.
 * \author AKara
 */

#include "mb_controller.hpp"

#include "MBDebugHelper.h" 
#include "CMessageBroker.hpp"

namespace NsMessageBroker
{
   CMessageBrokerController::CMessageBrokerController(const std::string& address, uint16_t port, std::string name):
   TcpClient(address, port),
   m_receivingBuffer(""),
   mControllersIdStart(-1),
   mControllersIdCurrent(0)
   {
      mControllersName = name;
   }


   std::string CMessageBrokerController::getControllersName()
   {
      return mControllersName;
   }

   CMessageBrokerController::~CMessageBrokerController()
   {
   }

   ssize_t CMessageBrokerController::Recv(std::string& data)
   {
      DBG_MSG(("CMessageBrokerController::Recv()\n"));
      ssize_t recv = TcpClient::Recv(data);
      DBG_MSG(("Received message: %s\n", data.c_str()));
      m_receivingBuffer += data;
      while (!stop)
      {
         Json::Value root;
         if (!m_reader.parse(m_receivingBuffer, root))
         {
            DBG_MSG(("Received not JSON string! %s\n", m_receivingBuffer.c_str()));
            return recv;
         }
         std::string wmes = m_receiverWriter.write(root);
         DBG_MSG(("Parsed JSON string:%s; length: %d\n", wmes.c_str(), wmes.length()));
         DBG_MSG(("Buffer is:%s\n", m_receivingBuffer.c_str()));
         ssize_t beginpos = m_receivingBuffer.find(wmes);
         if (-1 != beginpos)
         {
            m_receivingBuffer.erase(0, beginpos + wmes.length());
            DBG_MSG(("Buffer after cut is:%s\n", m_receivingBuffer.c_str()));
         } else
         {
            m_receivingBuffer.clear();
         }
         onMessageReceived(root);
      }
      return recv;
   }

   void CMessageBrokerController::onMessageReceived(Json::Value message)
   {
      // Determine message type and process...
      Json::Value error;
      if (checkMessage(message, error))
      {
         if (isNotification(message))
         {
            DBG_MSG(("Message is notification!\n"));
            processNotification(message);
         } else if (isResponse(message))
         {
            std::string id = message["id"].asString();
            std::string method = findMethodById(id);
            DBG_MSG(("Message is response on: %s\n", method.c_str()));
            if ("" != method)
            {
               if ("MB.registerComponent" == method)
               { // initialize mControllersIdStart
                  if (message.isMember("result") && message["result"].isInt())
                  {
                     mControllersIdStart = message["result"].asInt();
                  } else
                  {
                     DBG_MSG_ERROR(("Not possible to initialize mControllersIdStart!\n"));
                  }
               } else if ("MB.subscribeTo" == method || "MB.unregisterComponent" == method || "MB.unsubscribeFrom" == method)
               {
                  //nothing to do for now
               } else
               {
                  processResponse(method, message);
               }
            } else
            {
               DBG_MSG_ERROR(("Request with id %s has not been found!\n", id.c_str()));
            }
         } else
         {
            DBG_MSG(("Message is request!\n"));
            processRequest(message);
         }
      } else
      {
         DBG_MSG_ERROR(("Message contains wrong data!\n"));
      }
   }

   ssize_t CMessageBrokerController::Send(const std::string& data)
   {
      return TcpClient::Send(data);
   }

   void CMessageBrokerController::sendJsonMessage(Json::Value& message)
   {
      DBG_MSG(("CMessageBrokerController::sendJsonMessage()\n"));
      sync_primitives::AutoLock auto_lock(queue_lock_);
      std::string mes = m_writer.write(message);
      if (!isNotification(message) && !isResponse(message))
      {// not notification, not a response, store id and method name to recognize an answer
         mWaitResponseQueue.insert(std::map<std::string, std::string>::value_type(message["id"].asString(), message["method"].asString()));
      }
      int bytesSent = Send(mes);
      bytesSent = bytesSent; // to prevent compiler warnings in case DBG_MSG off
      DBG_MSG(("Length:%d, Sent: %d bytes\n", mes.length(), bytesSent));
   }

   std::string CMessageBrokerController::findMethodById(std::string id)
   {
      DBG_MSG(("CMessageBrokerController::findMethodById()\n"));
      sync_primitives::AutoLock auto_lock(queue_lock_);
      std::string res = "";
      std::map <std::string, std::string>::iterator it;
      it = mWaitResponseQueue.find(id);
      if (it != mWaitResponseQueue.end())
      {
         res = (*it).second;
         mWaitResponseQueue.erase(it);
      }
      return res;
   }

   int CMessageBrokerController::getNextMessageId()
   {
      if (mControllersIdCurrent < (mControllersIdStart+1000))
      {
         return mControllersIdCurrent++;
      } else
      {
         return mControllersIdCurrent = mControllersIdStart;
      }
   }
   
   void CMessageBrokerController::prepareMessage(Json::Value& root)
   {
      root["jsonrpc"] = "2.0";
      root["id"] = getNextMessageId();
   }

   void CMessageBrokerController::prepareErrorMessage(int errCode, std::string errMessage, Json::Value& error)
   {
      DBG_MSG(("CMessageBrokerController::prepareErrorMessage()\n"));
      Json::Value err;
      err["code"] = errCode;
      err["message"] = errMessage;
      error["error"] = err;
   }

   std::string CMessageBrokerController::getDestinationComponentName(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerController::getDestinationComponentName()\n"));
      std::string ret = "";
      std::string method = root["method"].asString();
      int pos = method.find(".");
      if (-1 != pos)
      {
         ret = method.substr(0, pos);
      }
      DBG_MSG(("Destination component is: %s\n", ret.c_str()));
      return ret;
   }

   std::string CMessageBrokerController::getMethodName(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerController::getMethodName()\n"));
      std::string ret = "";
      std::string method = root["method"].asString();
      int pos = method.find(".");
      if (-1 != pos)
      {
         ret = method.substr(pos+1);
      }
      DBG_MSG(("Method is: %s\n", ret.c_str()));
      return ret;
   }

   bool CMessageBrokerController::isNotification(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerController::isNotification()\n"));
      bool ret = false;
      if (false == root.isMember("id"))
      {
         ret = true;
      }
      DBG_MSG(("Result: %d\n", ret));
      return ret;
   }

   bool CMessageBrokerController::isResponse(Json::Value& root)
   {
      DBG_MSG(("CMessageBrokerController::isResponse()\n"));
      bool ret = false;
      if ((true == root.isMember("result")) || (true == root.isMember("error")))
      {
         ret = true;
      }
      DBG_MSG(("Result: %d\n", ret));
      return ret;
   }

   void CMessageBrokerController::registerController(int id)
   {
      DBG_MSG(("CMessageBrokerController::registerController()\n"));
      Json::Value root;
      Json::Value params;
      prepareMessage(root);
      root["id"] = id;
      root["method"] = "MB.registerComponent";
      params["componentName"] = mControllersName;
      root["params"] = params;
      sendJsonMessage(root);
   }

   void CMessageBrokerController::unregisterController()
   {
      DBG_MSG(("CMessageBrokerController::unregisterController()\n"));
      Json::Value root;
      Json::Value params;
      prepareMessage(root);
      root["method"] = "MB.unregisterComponent";
      params["componentName"] = mControllersName;
      root["params"] = params;
      sendJsonMessage(root);
   }

   void CMessageBrokerController::subscribeTo(std::string property)
   {
      DBG_MSG(("CMessageBrokerController::subscribeTo()\n"));
      Json::Value root;
      Json::Value params;
      prepareMessage(root);
      root["method"] = "MB.subscribeTo";
      params["propertyName"] = property;
      root["params"] = params;
      sendJsonMessage(root);
   }

   void CMessageBrokerController::unsubscribeFrom(std::string property)
   {
      DBG_MSG(("CMessageBrokerController::unsubscribeFrom()\n"));
      Json::Value root;
      Json::Value params;
      prepareMessage(root);
      root["method"] = "MB.unsubscribeFrom";
      params["propertyName"] = property;
      root["params"] = params;
      sendJsonMessage(root);
   }

   void* CMessageBrokerController::MethodForReceiverThread(void * arg)
   {
      sync_primitives::AutoLock auto_lock(receiving_thread_lock_);
      stop = false;
      arg = arg; // to avoid compiler warnings
      while(!stop)
      {
         std::string data = "";
         Recv(data);
      }
      return NULL;
   }

   bool CMessageBrokerController::checkMessage(Json::Value& root, Json::Value& error)
   {
      DBG_MSG(("CMessageBrokerController::checkMessage()\n"));
      Json::Value err;
   
      try
      {
         /* check the JSON-RPC version => 2.0 */
         if (!root.isObject() || !root.isMember("jsonrpc") || root["jsonrpc"] != "2.0") 
         {
            error["id"] = Json::Value::null;
            error["jsonrpc"] = "2.0";
            err["code"] = NsMessageBroker::INVALID_REQUEST;
            err["message"] = "Invalid MessageBroker request.";
            error["error"] = err;
            return false;
         }

         if (root.isMember("id") && (root["id"].isArray() || root["id"].isObject()))
         {
            error["id"] = Json::Value::null;
            error["jsonrpc"] = "2.0";
            err["code"] = NsMessageBroker::INVALID_REQUEST;
            err["message"] = "Invalid MessageBroker request.";
            error["error"] = err;
            return false;
         }

         if (root.isMember("method"))
         {
            if (!root["method"].isString())
            {
               error["id"] = Json::Value::null;
               error["jsonrpc"] = "2.0";
               err["code"] = NsMessageBroker::INVALID_REQUEST;
               err["message"] = "Invalid MessageBroker request.";
               error["error"] = err;
               return false;
            }
            /* Check the params is  an object*/
            if (root.isMember("params") && !root["params"].isObject())
            {
               error["id"] = Json::Value::null;
               error["jsonrpc"] = "2.0";
               err["code"] = INVALID_REQUEST;
               err["message"] = "Invalid JSONRPC params.";
               error["error"] = err;
               return false;
            }
         } else if (!root.isMember("result") && !root.isMember("error"))
         {
            return false;
         }
         return true;
      } catch (...)
      {
         DBG_MSG_ERROR(("CMessageBrokerController::checkMessage() EXCEPTION has been caught!\n"));
         return false;
      }
   }

} /* namespace NsMessageBroker */
