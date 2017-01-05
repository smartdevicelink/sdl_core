#ifndef CMESSAGEBROKERREGISTRY_H
#define CMESSAGEBROKERREGISTRY_H

#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "utils/lock.h"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker 
{

   /**
   * \class CMessageBrokerRegistry
   * \brief Singletone CMessageBrokerRegistry class implementation.
   */
   class CMessageBrokerRegistry
   {
   public:
      /**
      * \brief Singletone instantiator.
      * \return pointer to CMessageBroker instance
      */
      static CMessageBrokerRegistry* getInstance();

      /**
      * \brief Destructor.
      */
      ~CMessageBrokerRegistry();

      /**
      * \brief adds controller to the registry.
      * \param fd file descriptor of controller.
      * \param name name of controller.
      * \return false if already exist.
      */
      bool addController(int fd, std::string name);

      /**
      * \brief deletes controller from the registry.
      * \param name name of controller.
      */
      void deleteController(std::string name);

      /**
      * \brief Remove all controllers by descriptor
      * \param fd descriptor
      */
      void removeControllersByDescriptor(const int fd);

      /**
      * \brief Remove all subscribers by descriptor
      * \param fd descriptor
      */
      void removeSubscribersByDescriptor(const int fd);

      /**
      * \brief adds notification subscriber to the registry.
      * \param fd file descriptor of controller.
      * \param name name of property which should be observed.
      * \return false if already exist.
      */
      bool addSubscriber(int fd, std::string name);

      /**
      * \brief deletes notification subscriber from the registry.
      * \param fd file descriptor of controller.
      * \param name name of property which should be observed.
      */
      void deleteSubscriber(int fd, std::string name);

      /**
      * \brief gets controller fd from the registry by name.
      * \param name name of controller.
      * \return file descriptor of controller.
      */
      int getDestinationFd(std::string name);

      /**
      * \brief gets subscribers fd's.
      * \param name name of property.
      * \param result vector for results.
      * \return count of subscribers.
      */
      int getSubscribersFd(std::string name, std::vector<int>& result);
   private:
      /**
      * \brief Constructor.
      */
      CMessageBrokerRegistry();

      /**
      * \brief Map to store controllers information like ComponentName:socketFd.
      * For example PhoneController:1080
      */
      std::map <std::string, int> mControllersList;
      sync_primitives::Lock mControllersListLock;

      /**
      * \brief Map to store subscribers information like ComponentName.PropertyName:socketFd:.
      * For example PhoneController.onPhoneBookChanged:1080
      */
      std::multimap <std::string, int> mSubscribersList;
      sync_primitives::Lock mSubscribersListLock;
   };
} /* namespace NsMessageBroker */

#endif // CMESSAGEBROKERREGISTRY_H

