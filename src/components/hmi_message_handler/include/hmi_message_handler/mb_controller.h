#ifndef MB_CONTROLLER_H 
#define MB_CONTROLLER_H 

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include "json/json.h"

#include "utils/lock.h"
#include "utils/atomic_object.h"

using namespace boost::beast::websocket;

#ifdef DEBUG_ON 
/**
* \def DBG_MSG
* \brief Debug message output with file name and line number.
* \param x formatted debug message.
* \return printf construction.
*/
#define DBG_MSG(x) printf("%s:%d ", __FILE__, __LINE__);\
                   printf x
#else
#define DBG_MSG(x)
#endif

#define DBG_MSG_ERROR(x) printf("ERROR!!! %s:%d ", __FILE__, __LINE__);\
                         printf x


namespace NsMessageBroker {

  enum ErrorCode {
    CONTROLLER_EXISTS   = -32000,
    SUBSCRIBTION_EXISTS = -32001,
    INVALID_REQUEST     = -32600
  };

  class WebsocketSession;

  class CMessageBrokerController : public std::enable_shared_from_this<CMessageBrokerController> {
    public:
      CMessageBrokerController(const std::string& address, uint16_t port, std::string name, int num_ports, boost::asio::io_context& ioc);

      ~CMessageBrokerController();

      bool StartListener();

      bool Run();

      void WaitForConnection();

      void StartSession(boost::system::error_code ec);

      void OnAccept(boost::system::error_code ec, boost::asio::strand<boost::asio::io_context::executor_type>& strand, stream<boost::asio::ip::tcp::socket>& ws);

      void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);

      bool isNotification(Json::Value& message);

      void sendNotification(Json::Value& message);

      bool isResponse(Json::Value& message);

      void sendResponse(Json::Value& message);

      void sendJsonMessage(Json::Value& message);

      void subscribeTo(std::string property);

      void registerController(int id = 0);

      void unregisterController();

      void* MethodForReceiverThread(void * arg);

      bool Connect();

      void exitReceivingThread();

      virtual void processResponse(std::string method, Json::Value& root) = 0;

      virtual void processRequest(Json::Value& root) = 0;

      virtual void processNotification(Json::Value& root) = 0;

      std::string getMethodName(std::string& method);

      std::string GetComponentName(std::string& method);

      void processInternalRequest(Json::Value& message, WebsocketSession* ws_session );

      void pushRequest(Json::Value& message, WebsocketSession* ws_session);

      //Registry
      bool addController(WebsocketSession* ws_session, std::string name);

      void deleteController(WebsocketSession* ws_session);

      void deleteController(std::string name);

      void removeSubscribersBySession(const WebsocketSession* ws);

      bool addSubscriber(WebsocketSession* ws_session, std::string name);

      void deleteSubscriber(WebsocketSession* ws, std::string name);

      int getSubscribersFd(std::string name, std::vector<WebsocketSession*>& result);

      int getNextControllerIdDiapason();

    private:
      boost::asio::io_context ioc_;
      const std::string& address_;
      uint16_t port_;
      std::string name_;
      int num_threads_;
      std::vector<std::thread> thread_vector_;
      
      boost::asio::ip::tcp::acceptor acceptor_;
      boost::asio::ip::tcp::socket socket_;
      boost::beast::multi_buffer buffer_;
      boost::asio::ip::tcp::endpoint endpoint_;

      int mControllersIdCounter;

      //Registry
      std::vector <std::shared_ptr<NsMessageBroker::WebsocketSession>> mConnectionList;
      sync_primitives::Lock mConnectionListLock;

      std::map <std::string, WebsocketSession*> mControllersList;
      sync_primitives::Lock mControllersListLock;

      std::multimap <std::string, WebsocketSession*> mSubscribersList;
      sync_primitives::Lock mSubscribersListLock;

      std::map <std::string, WebsocketSession*> mRequestList;
      sync_primitives::Lock mRequestListLock;

      std::atomic_bool shutdown_;


  };

} //NsMessageBroker




#endif /* MB_CONTROLLER_H */