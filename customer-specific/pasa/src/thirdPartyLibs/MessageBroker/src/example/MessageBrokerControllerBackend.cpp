/**
 * \file MessageBrokerControllerBackend.cpp
 * \brief MessageBroker Controller for Backend.
 * \author AKara
 */

#include "MessageBrokerControllerBackend.hpp"

#include "MBDebugHelper.h" 

namespace NsMessageBroker
{
  CMessageBrokerControllerBackend::CMessageBrokerControllerBackend(const std::string& address, unsigned short port):
CMessageBrokerController(address, port, std::string("Backend"))
  {
  }

  CMessageBrokerControllerBackend::~CMessageBrokerControllerBackend()
  {
  }

  void CMessageBrokerControllerBackend::processRequest(Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processRequest()\n"));
    if (getControllersName() == getDestinationComponentName(root))
    {
      Json::Value response;
      response["jsonrpc"] = root["jsonrpc"];
      response["id"] = root["id"];
      if ("isFirstStart" == getMethodName(root))
      {
        isFirstStart(response);
      } else if ("isFullScreen" == getMethodName(root))
      {
        isFullScreen(response);
      } else if ("getWindowSize" == getMethodName(root))
      {
        getWindowSize(response);
      } else if ("getWindowDensity" == getMethodName(root))
      {
        getWindowDensity(response);
      } else if ("getOSInfo" == getMethodName(root))
      {
        getOSInfo(response);
      } else if ("logToOS" == getMethodName(root))
      {
        logToOS(response);
      } else
      {
        DBG_MSG_ERROR(("Method has not been found!\n"));
        Json::Value err;
        err["code"] = NsMessageBroker::INVALID_REQUEST;
        err["message"] = "Method has not been found.";
        response["error"] = err;
      }
      sendJsonMessage(response);
    } else
    {
      DBG_MSG_ERROR(("Wrong message destination!\n"));
    }
  }

  void CMessageBrokerControllerBackend::processNotification(Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processNotification()\n"));
    root=root;//to prevent compiler warning
  }

  void CMessageBrokerControllerBackend::processResponse(std::string method, Json::Value& root)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::processResponse()\n"));
    root=root;//to prevent compiler warning
    method=method;//to prevent compiler warning
  }

  void CMessageBrokerControllerBackend::isFirstStart(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::isFirstStart()\n"));
    Json::Value res;
    res["isFirstStart"] = false;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::isFullScreen(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::isFullScreen()\n"));
    Json::Value res;
    res["isFullScreen"] = false;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getWindowSize(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getWindowSize()\n"));
    Json::Value res;
    res["width"] = 800;
    res["height"] = 480;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getWindowDensity(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getWindowDensity()\n"));
    Json::Value res;
    res["windowDensity"] = 1;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::getOSInfo(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::getOSInfo()\n"));
    Json::Value res;
    res["osType"] = "";
    res["osVersion"] = "";
    res["isNativeApplication"] = true;
    response["result"] = res;
  }

  void CMessageBrokerControllerBackend::logToOS(Json::Value& response)
  {
    DBG_MSG(("CMessageBrokerControllerBackend::logToOS()\n"));
    response["result"] = "";
  }

} /* namespace NsMessageBroker */
