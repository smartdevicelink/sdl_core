/**
 * \file CMessageBroker.cpp
 * \brief CMessageBroker singletone class implementation.
 * \author AKara
 */

#include <cassert>
#include <stdio.h>
#include <vector>

#include <string>

#include "CMessageBroker.hpp"
#include "CMessageBrokerRegistry.hpp"

#include "system.h"

#include "json/json.h"

#include "libMBDebugHelper.h"

namespace NsMessageBroker {
/**
 * \class CMessage
 * \brief CMessage class implementation.
 */
class CMessage {
  public:
    /**
     * \brief Constructor.
     */
    CMessage(int aSenderFp, Json::Value aMessage) {
      mSenderFd = aSenderFp;
      mMessage = aMessage;
    }

    /**
     * \brief Destructor.
     */
    ~CMessage() {
    }

    /**
     * \brief getter for Json::Value message.
     * \return Json::Value message.
     */
    Json::Value getMessage() const {
      return mMessage;
    }

    /**
     * \brief getter for sender FileDescriptor.
     * \return sender FileDescriptor.
     */
    int getSenderFd() const {
      return mSenderFd;
    }
  private:
    /**
     * \brief sender FileDescriptor.
     */
    int mSenderFd;

    /**
     * \brief Json::Value message.
     */
    Json::Value mMessage;
};


class CMessageBroker_Private {
  public:
    /**
     * \brief Constructor.
     */
    CMessageBroker_Private();

    /**
     * \brief Check if que empty (Thread safe).
     * \return True when empty.
     */
    bool isEventQueueEmpty();

    /**
     * \brief Pop message from que (Thread safe).
     * \return Pointer to CMessage.
     */
    CMessage* popMessage();

    /**
     * \brief Push message to que (Thread safe).
     * \param pMessage pointer to new CMessage object.
     */
    void pushMessage(CMessage* pMessage);

    /**
     * \brief gets  destination component name.
     * \param pMessage JSON message.
     * \return string destination component name.
     */
    std::string getDestinationComponentName(CMessage* pMessage);

    /**
     * \brief gets  method name.
     * \param pMessage JSON message.
     * \return string method name.
     */
    std::string getMethodName(CMessage* pMessage);

    /**
     * \brief checks is message notification or not.
     * \param pMessage JSON message.
     * \return true if notification.
     */
    bool isNotification(CMessage* pMessage);

    /**
     * \brief checks is message response or not.
     * \param pMessage JSON message.
     * \return true if response.
     */
    bool isResponse(CMessage* pMessage);

    /**
     * \brief checks message.
     * \param pMessage JSON message.
     * \param error JSON message to fill in case of any errors.
     * \return true if message is good.
     */
    bool checkMessage(CMessage* pMessage, Json::Value& error);

    /**
     * \brief Process internal MessageBrocker message
     *
     * \brief Register controller in MessageBroker.
     *  Use following JSON command to register new component:
     * \code
     *  {"jsonrpc": "2.0", "method": "MB.registerComponent", "params": "<ComponentName>"}
     * \endcode
     *
     * \brief Unregister controller in MessageBroker.
     *  Use following JSON command to unregister component:
     * \code
     *  {"jsonrpc": "2.0", "method": "MB.unregisterComponent", "params": "<ComponentName>"}
     * \endcode
     *
     * \brief Subscribe controller on property change.
     *  Use following JSON command to subscribe to notifications:
     * \code
     *  {"jsonrpc": "2.0", "method": "MB.subscribeTo", "params": "<ComponentName>.<NotificationName>"}
     * \endcode
     *
     * \brief Unsubscribe controller from property change.
     *  Use following JSON command to unsubscribe from notifications:
     * \code
     *  {"jsonrpc": "2.0", "method": "MB.unsubscribeFrom", "params": "<ComponentName>.<NotificationName>"}
     * \endcode
     *
     * \param pMessage JSON message.
     */
    void processInternalMessage(CMessage* pMessage);

    /**
     * \brief process external message.
     * \param pMessage JSON message.
     */
    void processExternalMessage(CMessage* pMessage);

    /**
     * \brief process response.
     * \param pMessage JSON message.
     */
    void processResponse(CMessage* pMessage);

    /**
     * \brief Process notification message.
     * \brief Notify subscribers about property change.
     * expected notification format example:
     * \code
     * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
     * \endcode
     * \param pMessage JSON message.
     */
    void processNotification(CMessage* pMessage);

    /**
     * \brief send error message.
     * \param pMessage JSON message.
     */
    void processError(CMessage* pMessage);

    /**
     * \brief send Json message.
     * \param fd FileDescriptor of socket.
     * \param message JSON message.
     */
    void sendJsonMessage(int fd, Json::Value message);

    /**
     * \brief push message to wait response que.
     * \param pMessage JSON message.
     */
    void pushMessageToWaitQue(CMessage* pMessage);

    /**
     * \brief Returns start position for Id's generator of controller.
     * \return start position for Id's generator of controller (1000 id's).
     */
    int getNextControllerIdDiapason() {
      return 1000 * mControllersIdCounter++;
    }

    /**
     * \brief pop message from wait response que.
     * \param pMessage JSON message.
     */
    int popMessageFromWaitQue(CMessage* pMessage);

    /**
     * \brief Tries to remove the parsed part of the buffer
     * \param root Parsed JSON value
     * \param aJSONData The string buffer
     * \return true on success, false on failure
     */
    bool cutParsedJSON(const Json::Value& root, std::string& aJSONData);

    /**
     * \brief Finds the position just after a JSON object or array in a buffer
     * \param isObject Must be true for object, false for array
     * \param aJSONData The string buffer
     * \return The position in the buffer after the object or array on success,
     *         std::strin::npos on failure
     */
    size_t jumpOverJSONObjectOrArray(bool isObject, const std::string& aJSONData);

    /**
     * \brief Finds the position just after a JSON string in a buffer
     * \param aJSONData The string buffer
     * \return The position in the buffer after the string on success,
     *         std::strin::npos on failure
     */
    size_t jumpOverJSONString(const std::string& aJSONData);

    /**
     * \brief Que of messages.
     */
    std::deque<CMessage*> mMessagesQueue;

    /**
     * \brief Counter of messages Id's diapason for the next controllers
     * From mControllersIdCounter*1000 to mControllersIdCounter*1000+999.
     */
    int mControllersIdCounter;

    /**
     * \brief Que of messages which are waiting the response in format: MessageId:SenderFd.
     */
    std::map<int, int> mWaitResponseQueue;

    /**
     * \brief Pointer to sender.
     */
    CSender* mpSender;

    /**
     * \brief Pointer to registry.
     */
    CMessageBrokerRegistry* mpRegistry;

    /**
     * \brief JSON reader.
     */
    Json::Reader m_reader;

    /**
     * \brief JSON writer.
     */
    Json::FastWriter m_writer;

    /**
     * \brief JSON writer for receiver.
     */
    Json::FastWriter m_recieverWriter;

    /**
     * \brief Messages que mutex.
     */
    System::Mutex mMessagesQueueMutex;

    /**
     * \brief Binary semaphore that is used to notify the
     * messaging thread that a new message is available.
     */
    System::BinarySemaphore m_messageQueueSemaphore;
};

CMessageBroker_Private::CMessageBroker_Private() :
  mControllersIdCounter(1),
  mpSender(NULL) {
  mpRegistry = CMessageBrokerRegistry::getInstance();
}


CMessageBroker::CMessageBroker() :
  p(new CMessageBroker_Private()) {
}

CMessageBroker::~CMessageBroker() {
  delete p, p = 0;
}

CMessageBroker* CMessageBroker::getInstance() {
  static CMessageBroker instance;
  return &instance;
}


size_t CMessageBroker_Private::jumpOverJSONObjectOrArray(bool isObject,
                                                 const std::string& aJSONData) {
  const char openBracket  = isObject? '{' : '[';
  const char closeBracket = isObject? '}' : ']';
  int open_minus_close_brackets(1);
  size_t position = aJSONData.find(openBracket);  // Find the beginning of the object

  while ((position != std::string::npos) && (open_minus_close_brackets > 0)) {
    position = aJSONData.find_first_of(std::string("\"")+openBracket+closeBracket,
                                       position+1);
    if (std::string::npos == position) {
      break;
    }
    if ('"' == aJSONData[position]) {
      // Ignore string interior, which might contain brackets and escaped "-s
      do {
        position = aJSONData.find('"', position+1);  // Find the closing quote
      } while ((std::string::npos != position) && ('\\' == aJSONData[position-1]));
    } else if (openBracket == aJSONData[position]) {
      ++open_minus_close_brackets;
    } else if (closeBracket == aJSONData[position]) {
      --open_minus_close_brackets;
    }
  }

  if ((0 == open_minus_close_brackets) && (std::string::npos != position)) {
    ++position;  // Move after the closing bracket
  } else {
    position = std::string::npos;
  }

  return position;
}


size_t CMessageBroker_Private::jumpOverJSONString(const std::string& aJSONData) {
  size_t position = aJSONData.find('"');  // Find the beginning of the string

  do {
    position = aJSONData.find('"', position+1);  // Find the closing quote
  } while ((std::string::npos != position) && ('\\' == aJSONData[position-1]));

  if (std::string::npos != position) {
    ++position;  // Move after the closing quote
  }

  return position;
}


bool CMessageBroker_Private::cutParsedJSON(const Json::Value& root,
                                           std::string& aJSONData) {
  if (root.isNull() || aJSONData.empty()) {
    DBG_MSG_ERROR(("JSON is null or the buffer is empty!\n"));
    return false;
  }

  std::string parsed_json_str = m_recieverWriter.write(root);
  DBG_MSG(("Parsed JSON string: '%s'\n", parsed_json_str.c_str()));

  // Trim front spaces (if any)
  const size_t nonempty_position = aJSONData.find_first_not_of(" \t\n\v\f\r");
  aJSONData.erase(0, nonempty_position);
  if (std::string::npos == nonempty_position) {
    DBG_MSG_ERROR(("Buffer contains only blanks!\n"));
    return false;
  }

  // JSON writer puts '\n' at the end. Remove it.
  const size_t final_lf_pos = parsed_json_str.rfind('\n');
  if (final_lf_pos == parsed_json_str.length()-1) {
    parsed_json_str.erase(final_lf_pos, 1);
  }

  /* RFC 4627: "A JSON value MUST be an object, array, number, or string, or
   * one of the following three literal names: false null true"
   * So we will try to find the borders of the parsed part based on its type. */

  size_t position(std::string::npos);

  if (0 == aJSONData.find(parsed_json_str)) {
    // If by chance parsed JSON is the same in the buffer and is at the beginning
    position = parsed_json_str.length();
  } else if (root.isObject() || root.isArray()) {
    position = jumpOverJSONObjectOrArray(root.isObject(), aJSONData);
  } else if (root.isString()) {
    position = jumpOverJSONString(aJSONData);
  } else if (root.isNumeric()) {
    position = aJSONData.find_first_not_of("+-0123456789.eE");
  } else if (root.isBool() || ("null" == parsed_json_str)) {
    position = aJSONData.find(parsed_json_str);
    if (std::string::npos != position) {
      position += parsed_json_str.length();
    }
  } else {
    DBG_MSG_ERROR(("Unknown JSON type!\n"));
  }

  if (std::string::npos == position) {
    DBG_MSG_ERROR(("Error finding JSON object boundaries!\n"));
    /* This should not happen, because the string is already parsed as a
     * valid JSON. If this happens then above code is wrong. It is better
     * to assert() than just return here, because otherwise we may enter an
     * endless cycle - fail to process one and the same message again and
     * again. Or we may clear the buffer and return, but in this way we will
     * loose the next messages, miss a bug here, and create another bug. */
    assert(std::string::npos != position);
    return false;  // For release version
  }

  if ((position >= aJSONData.length()) ||
      ((position == aJSONData.length()-1) && isspace(aJSONData[position]))) {
    // No next object. Clear entire aJSONData.
    aJSONData = "";
  } else {
    // There is another object. Clear the current one.
    aJSONData.erase(0, position);
  }

  return true;
}


void CMessageBroker::onMessageReceived(int fd, std::string& aJSONData, bool tryHard) {
  DBG_MSG(("CMessageBroker::onMessageReceived(%d, '%s')\n", fd, aJSONData.c_str()));

  while (! aJSONData.empty()) {
    Json::Value root;
    if ((! p->m_reader.parse(aJSONData, root)) || root.isNull()) {
      DBG_MSG_ERROR(("Unable to parse JSON!\n"));
      if (! tryHard) {
        return;
      }
      uint8_t first_byte = static_cast<uint8_t>(aJSONData[0]);
      if ((first_byte <= 0x08) || ((first_byte >= 0x80) && (first_byte <= 0x88))) {
        DBG_MSG(("There is an unparsed websocket header probably.\n"));
        /* Websocket headers can have FIN flag set in the first byte (0x80).
         * Then there are 3 zero bits and 4 bits for opcode (from 0x00 to 0x0A).
         * But actually we don't use opcodes above 0x08.
         * Use this fact to distinguish websocket header from payload text data.
         * It can be a coincidence of course, but we have to give it a try. */
        return;
      } else if ('{' == aJSONData[0]) {
        const bool is_object = true;
        const size_t next_object_pos =
            p->jumpOverJSONObjectOrArray(is_object, aJSONData);

        if (next_object_pos != std::string::npos) {
          DBG_MSG_ERROR(("Invalid JSON object probably. Skipping.\n"));
          aJSONData.erase(0, next_object_pos);
          DBG_MSG(("Buffer after cut is: '%s'\n", aJSONData.c_str()));
          continue;
        }
        DBG_MSG_ERROR(("Incomplete JSON object probably.\n"));
        return;
      } else {
        DBG_MSG_ERROR(("Step in the buffer and try again...\n"));
        aJSONData.erase(0, 1);
        DBG_MSG(("Buffer after cut is: '%s'\n", aJSONData.c_str()));
        continue;
      }

    } else if (! root.isObject()) {
      /* JSON RPC 2.0 messages are objects. Batch calls must be pre-rpocessed,
       * so no need for "and !root.isArray()" */
      DBG_MSG_ERROR(("Parsed JSON is not an object!\n"));
      if (! tryHard) {
        return;
      }
      // Cut parsed data from the buffer below and continue

    } else if ((!root.isMember("jsonrpc")) || (root["jsonrpc"]!="2.0")) {
      DBG_MSG_ERROR(("'jsonrpc' is not set correctly in parsed JSON!\n"));
      if (! tryHard) {
        return;
      }
      // Cut parsed object from the buffer below and continue

    } else {
      // Parsing successful. Pass the message up.
      p->pushMessage(new CMessage(fd, root));
    }

    p->cutParsedJSON(root, aJSONData);

    DBG_MSG(("Buffer after cut is: '%s'\n", aJSONData.c_str()));
  }
}

void CMessageBroker::Test() {
  Json::Value root, err;
  std::string ReceivingBuffer =
    "{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"AVA\"}}123{\"id\":0,\"jsonrpc\":\"2.0\",\"method\":\"MB.registerComponent\",\"params\":{\"componentName\":\"AVA\"}}";
  DBG_MSG(("String is:%s\n", ReceivingBuffer.c_str()));
  while (1) {
    if (!p->m_reader.parse(ReceivingBuffer, root)) {
      DBG_MSG_ERROR(("Received not JSON string! %s\n", ReceivingBuffer.c_str()));
      return;
    }
    std::string wmes = p->m_recieverWriter.write(root);
    DBG_MSG(("Parsed JSON string:%s; length: %zu\n", wmes.c_str(), wmes.length()));
    DBG_MSG(("Buffer is:%s\n", ReceivingBuffer.c_str()));
    ssize_t beginpos = ReceivingBuffer.find(wmes);
    ReceivingBuffer.erase(0, beginpos + wmes.length());
    DBG_MSG(("Buffer after cut is:%s\n", ReceivingBuffer.c_str()));
    CMessage message(0, root);
    if (p->checkMessage(&message, err)) {
      //here put message to que
    } else {
      DBG_MSG_ERROR(("Wrong message:%s\n", wmes.c_str()));
    }
  }
}

void CMessageBroker::OnSocketClosed(const int fd) {
  DBG_MSG(("CMessageBroker::OnSocketClosed(%d)\n", fd));
  if (p->mpRegistry) {
    p->mpRegistry->removeControllersByDescriptor(fd);
  }
}

void CMessageBroker::startMessageBroker(CSender* pSender) {
  DBG_MSG(("CMessageBroker::startMessageBroker()\n"));
  p->mpSender = pSender;
}

void CMessageBroker::stopMessageBroker() {
  p->mpSender = NULL;
  DBG_MSG(("CMessageBroker::stopMessageBroker()\n"));
}

CMessage* CMessageBroker_Private::popMessage() {
  CMessage* ret = NULL;
  DBG_MSG(("CMessageBroker::popMessage()\n"));
  mMessagesQueueMutex.Lock();
  if (false == mMessagesQueue.empty()) {
    ret = mMessagesQueue.front();
    mMessagesQueue.pop_front();// delete message from que
  } else {
    DBG_MSG(("Que is empty!\n"));
  }
  mMessagesQueueMutex.Unlock();
  return ret;
}

void CMessageBroker_Private::pushMessage(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::pushMessage()\n"));
  mMessagesQueueMutex.Lock();
  if (pMessage) {
    mMessagesQueue.push_back(pMessage);
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
  mMessagesQueueMutex.Unlock();

  m_messageQueueSemaphore.Notify();
}

bool CMessageBroker_Private::isEventQueueEmpty() {
  bool bResult = true;
  mMessagesQueueMutex.Lock();
  bResult = mMessagesQueue.empty();
  mMessagesQueueMutex.Unlock();
  return bResult;
}

std::string CMessageBroker_Private::getDestinationComponentName(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::getDestinationComponentName()\n"));
  std::string ret = "";
  if (pMessage) {
    Json::Value mes = pMessage->getMessage();
    std::string method = mes["method"].asString();
    int pos = method.find(".");
    if (-1 != pos) {
      ret = method.substr(0, pos);
    }
    DBG_MSG(("Destination component is: %s\n", ret.c_str()));
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
  return ret;
}

std::string CMessageBroker_Private::getMethodName(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::getMethodName()\n"));
  std::string ret = "";
  if (pMessage) {
    Json::Value mes = pMessage->getMessage();
    std::string method = mes["method"].asString();
    int pos = method.find(".");
    if (-1 != pos) {
      ret = method.substr(pos + 1);
    }
    DBG_MSG(("Method is: %s\n", ret.c_str()));
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
  return ret;
}

bool CMessageBroker_Private::isNotification(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::isNotification()\n"));
  bool ret = false;
  Json::Value mes = pMessage->getMessage();
  if (false == mes.isMember("id")) {
    ret = true;
  }
  DBG_MSG(("Result: %d\n", ret));
  return ret;
}

bool CMessageBroker_Private::isResponse(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::isResponse()\n"));
  bool ret = false;
  Json::Value mes = pMessage->getMessage();
  if ((true == mes.isMember("result")) || (true == mes.isMember("error"))) {
    ret = true;
  }
  DBG_MSG(("Result: %d\n", ret));
  return ret;
}

void CMessageBroker_Private::pushMessageToWaitQue(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::pushMessageToWaitQue()\n"));
  if (pMessage) {
    Json::Value root = pMessage->getMessage();
    mWaitResponseQueue.insert(std::map<int, int>::value_type(root["id"].asInt(), pMessage->getSenderFd()));
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
}

int CMessageBroker_Private::popMessageFromWaitQue(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::popMessageFromWaitQue()\n"));
  int result = -1;
  if (pMessage) {
    Json::Value root = pMessage->getMessage();
    int messageId = root["id"].asInt();
    std::map <int, int>::iterator it;
    it = mWaitResponseQueue.find(messageId);
    if (it != mWaitResponseQueue.end()) {
      result = (*it).second;
      mWaitResponseQueue.erase(it);
    }
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
  DBG_MSG(("Senders Fd: %d\n", result));
  return result;
}

void CMessageBroker_Private::processInternalMessage(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::processInternalMessage()\n"));
  if (pMessage) {
    std::string amethodName = getMethodName(pMessage);
    DBG_MSG(("Method: %s\n", amethodName.c_str()));
    Json::Value root = pMessage->getMessage();
    if ("registerComponent" == amethodName) {
      Json::Value params = root["params"];
      if (params.isMember("componentName") && params["componentName"].isString()) {
        std::string controllerName = params["componentName"].asString();
        if (mpRegistry->addController(pMessage->getSenderFd(), controllerName)) {
          Json::Value response;
          response["id"] = root["id"];
          response["jsonrpc"] = "2.0";
          response["result"] = getNextControllerIdDiapason();
          sendJsonMessage(pMessage->getSenderFd(), response);
        } else {
          Json::Value error, err;
          error["id"] = root["id"];
          error["jsonrpc"] = "2.0";
          err["code"] = CONTROLLER_EXISTS;
          err["message"] = "Controller has been already registered.";
          error["error"] = err;
          processError(new CMessage(pMessage->getSenderFd(), error));
        }
      } else {
        Json::Value error, err;
        error["id"] = root["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = INVALID_REQUEST;
        err["message"] = "Wrong method parameter.";
        error["error"] = err;
        processError(new CMessage(pMessage->getSenderFd(), error));
      }
    } else if ("subscribeTo" == amethodName) {
      Json::Value params = root["params"];
      if (params.isMember("propertyName") && params["propertyName"].isString()) {
        std::string propertyName = params["propertyName"].asString();
        if (mpRegistry->addSubscriber(pMessage->getSenderFd(), propertyName)) {
          Json::Value response;
          response["id"] = root["id"];
          response["jsonrpc"] = "2.0";
          response["result"] = "OK";
          sendJsonMessage(pMessage->getSenderFd(), response);
        } else {
          Json::Value error, err;
          error["id"] = root["id"];
          error["jsonrpc"] = "2.0";
          err["code"] = CONTROLLER_EXISTS;
          err["message"] = "Subscribe has been already registered.";
          error["error"] = err;
          processError(new CMessage(pMessage->getSenderFd(), error));
        }
      } else {
        Json::Value error, err;
        error["id"] = root["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = INVALID_REQUEST;
        err["message"] = "Wrong method parameter.";
        error["error"] = err;
        processError(new CMessage(pMessage->getSenderFd(), error));
      }
    } else if ("unregisterComponent" == amethodName) {
      Json::Value params = root["params"];
      if (params.isMember("componentName") && params["componentName"].isString()) {
        std::string controllerName = params["componentName"].asString();
        mpRegistry->deleteController(controllerName);
        Json::Value response;
        response["id"] = root["id"];
        response["jsonrpc"] = "2.0";
        response["result"] = "OK";
        sendJsonMessage(pMessage->getSenderFd(), response);
      } else {
        Json::Value error, err;
        error["id"] = root["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = INVALID_REQUEST;
        err["message"] = "Wrong method parameter.";
        error["error"] = err;
        processError(new CMessage(pMessage->getSenderFd(), error));
      }
    } else if ("unsubscribeFrom" == amethodName) {
      Json::Value params = root["params"];
      if (params.isMember("propertyName") && params["propertyName"].isString()) {
        std::string propertyName = params["propertyName"].asString();
        mpRegistry->deleteSubscriber(pMessage->getSenderFd(), propertyName);
        Json::Value response;
        response["id"] = root["id"];
        response["jsonrpc"] = "2.0";
        response["result"] = "OK";
        sendJsonMessage(pMessage->getSenderFd(), response);
      } else {
        Json::Value error, err;
        error["id"] = root["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = INVALID_REQUEST;
        err["message"] = "Wrong method parameter.";
        error["error"] = err;
        processError(new CMessage(pMessage->getSenderFd(), error));
      }
    } else {
      DBG_MSG(("Unknown method!\n"));
      Json::Value error;
      Json::Value err;
      error["id"] = root["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Invalid MessageBroker method.";
      error["error"] = err;
      processError(new CMessage(pMessage->getSenderFd(), error));
    }
  } else {
    DBG_MSG_ERROR(("NULL pointer!\n"));
  }
}

void CMessageBroker_Private::processExternalMessage(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::processExternalMessage()\n"));
  if (pMessage) {
    std::string destComponentName = getDestinationComponentName(pMessage);
    int destFd = mpRegistry->getDestinationFd(destComponentName);
    Json::Value root = pMessage->getMessage();
    if (0 < destFd) {
      sendJsonMessage(destFd, root);
      pushMessageToWaitQue(pMessage);
    } else {
      // error, controller not found in the registry
      DBG_MSG(("Unknown method!\n"));
      Json::Value error;
      Json::Value err;
      Json::Value error_data;
      error["id"] = root["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = UNSUPPORTED_RESOURCE;
      err["message"] = "Destination controller not found!";
      error_data["method"] = root["method"];
      err["data"] = error_data;
      error["error"] = err;
      processError(new CMessage(pMessage->getSenderFd(), error));
    }
  } else {
    DBG_MSG_ERROR(("NULL pointer\n"));
  }
}

void CMessageBroker_Private::processResponse(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::processResponse()\n"));
  if (pMessage) {
    int senderFd = popMessageFromWaitQue(pMessage);
    if (-1 != senderFd) {
      sendJsonMessage(senderFd, pMessage->getMessage());
    }
  } else {
    DBG_MSG_ERROR(("NULL pointer\n"));
  }
}

void CMessageBroker_Private::processNotification(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::processNotification()\n"));
  if (pMessage) {
    Json::Value root = pMessage->getMessage();
    std::string methodName = root["method"].asString();
    DBG_MSG(("Property: %s\n", methodName.c_str()));
    std::vector<int> result;
    int subscribersCount = mpRegistry->getSubscribersFd(methodName, result);
    if (0 < subscribersCount) {
      std::vector<int>::iterator it;
      for (it = result.begin(); it != result.end(); it++) {
        sendJsonMessage(*it, root);
      }
    } else {
      DBG_MSG(("No subscribers for this property!\n"));
    }
  } else {
    DBG_MSG_ERROR(("NULL pointer\n"));
  }
}

void CMessageBroker_Private::processError(CMessage* pMessage) {
  DBG_MSG(("CMessageBroker::processError()\n"));
  if (pMessage) {
    sendJsonMessage(pMessage->getSenderFd(), pMessage->getMessage());
    delete pMessage;// delete CMessage object with error description!!!
  } else {
    DBG_MSG_ERROR(("NULL pointer\n"));
  }
}

void CMessageBroker_Private::sendJsonMessage(int fd, Json::Value message) {
  DBG_MSG(("CMessageBroker::sendJsonMessage(%d)\n", fd));
  if (mpSender) {
    std::string mes = m_writer.write(message);
    int retVal = mpSender->Send(fd, mes);
    if (retVal == -1) {
      DBG_MSG_ERROR(("Message hasn't been sent!\n"));
      return;
    }
    DBG_MSG(("Length: %zu, Sent: %d bytes\n", mes.length(), retVal));
  } else {
    DBG_MSG_ERROR(("mpSender NULL pointer\n"));
  }
}

void* CMessageBroker::MethodForThread(void* arg) {
  arg = arg; // to avoid compiler warnings
  while (1) {
    while (!p->isEventQueueEmpty()) {
      CMessage* message = p->popMessage();
      if (message) {
        Json::Value error;
        if (p->checkMessage(message, error)) {
          if (p->isNotification(message)) {
            DBG_MSG(("Message is notification!\n"));
            p->processNotification(message);
          } else if (p->isResponse(message)) {
            DBG_MSG(("Message is response!\n"));
            p->processResponse(message);
          } else {
            if ("MB" == p->getDestinationComponentName(message)) {
              DBG_MSG(("Internal MessageBroker method!\n"));
              p->processInternalMessage(message);
            } else {
              DBG_MSG(("Not MessageBroker method!\n"));
              p->processExternalMessage(message);
            }
          }
        } else {
          DBG_MSG_ERROR(("Message contains wrong data!\n"));
          CMessage* errMessage = new CMessage(message->getSenderFd(), error);
          if (NULL != errMessage) {
            p->processError(errMessage);
          } else {
            DBG_MSG_ERROR(("NULL pointer!\n"));
          }
        }
        delete message;// delete message object
      }
    }
    p->m_messageQueueSemaphore.Wait();
  }

  return NULL;
}

bool CMessageBroker_Private::checkMessage(CMessage* pMessage, Json::Value& error) {
  DBG_MSG(("CMessageBroker::checkMessage()\n"));
  Json::Value root;
  root = pMessage->getMessage();
  Json::Value err;

  /* check the JSON-RPC version => 2.0 */
  if (!root.isObject() || !root.isMember("jsonrpc") || root["jsonrpc"] != "2.0") {
    error["id"] = Json::Value::null;
    error["jsonrpc"] = "2.0";
    err["code"] = INVALID_REQUEST;
    err["message"] = "Invalid JSON RPC version.";
    error["error"] = err;
    return false;
  }

  /* Check the id of message */
  if (root.isMember("id") && (root["id"].isArray() || root["id"].isObject() || root["id"].isString())) {
    error["id"] = Json::Value::null;
    error["jsonrpc"] = "2.0";
    err["code"] = INVALID_REQUEST;
    err["message"] = "Invalid ID of message.";
    error["error"] = err;
    return false;
  }

  /* extract "method" attribute */
  if (root.isMember("method")) {
    if (!root["method"].isString()) {
      error["id"] = Json::Value::null;
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Invalid JSONRPC method.";
      error["error"] = err;
      return false;
    }
    /* Check the params is  an object*/
    if (root.isMember("params") && !root["params"].isObject()) {
      error["id"] = Json::Value::null;
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Invalid JSONRPC params.";
      error["error"] = err;
      return false;
    }
  } else if (!(root.isMember("result") || root.isMember("error"))) {
    error["id"] = Json::Value::null;
    error["jsonrpc"] = "2.0";
    err["code"] = INVALID_REQUEST;
    err["message"] = "Unknwn message type.";
    error["error"] = err;
    return false;
  }
  return true;
}
} /* namespace NsMessageBroker */
