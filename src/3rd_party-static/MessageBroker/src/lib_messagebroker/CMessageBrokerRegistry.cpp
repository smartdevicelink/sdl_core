/**
 * \file CMessageBrokerRegistry.cpp
 * \brief CMessageBrokerRegistry singletone class implementation.
 * \author AKara
 */

#include "CMessageBrokerRegistry.hpp"

#include "libMBDebugHelper.h"

#include <vector>
#include <string>

namespace NsMessageBroker 
{
   CMessageBrokerRegistry::CMessageBrokerRegistry()
   {
   }

   CMessageBrokerRegistry::~CMessageBrokerRegistry()
   {
   }

   CMessageBrokerRegistry* CMessageBrokerRegistry::getInstance()
   {
      static CMessageBrokerRegistry instance;
      return &instance;
   }

   bool CMessageBrokerRegistry::addController(int fd, std::string name)
   {
      DBG_MSG(("CMessageBrokerRegistry::addController()\n"));
      bool result = false;
      std::map <std::string, int>::iterator it;

      sync_primitives::AutoLock lock(mControllersListLock);
      it = mControllersList.find(name);
      if (it == mControllersList.end())
      {
         mControllersList.insert(std::map <std::string, int>::value_type(name, fd));
         result = true;
      } else
      {
         DBG_MSG(("Controller already exists!\n"));
      }

      DBG_MSG(("Count of controllers: %d\n", mControllersList.size()));
      return result;
   }

   void CMessageBrokerRegistry::deleteController(std::string name)
   {
      DBG_MSG(("CMessageBrokerRegistry::deleteController()\n"));
      std::map <std::string, int>::iterator it;

      int fd;
      {
         sync_primitives::AutoLock controllers_lock(mControllersListLock);
         it = mControllersList.find(name);
         if (it != mControllersList.end())
         {
            fd = it->second;
            mControllersList.erase(it);
         } else {
            DBG_MSG(("No such controller in the list!\n"));
            return;
         }
         DBG_MSG(("Count of controllers: %d\n", mControllersList.size()));
      }

      sync_primitives::AutoLock subscribers_lock(mSubscribersListLock);
      std::multimap <std::string, int>::iterator it_s = mSubscribersList.begin();
      for (; it_s !=mSubscribersList.end(); ) {
         if (it_s->second == fd) {
            mSubscribersList.erase(it_s++);
         } else {
            ++it_s;
         }
      }
   }

   void CMessageBrokerRegistry::removeControllersByDescriptor(const int fd) {
      DBG_MSG(("CMessageBrokerRegistry::removeControllersByDescriptor(%d)\n",
               fd));
      std::vector<std::string> controllers_to_delete;
      {
        sync_primitives::AutoLock controllers_lock(mControllersListLock);
        std::map <std::string, int>::iterator it = mControllersList.begin();
        for (; it != mControllersList.end();) {
          if (it->second == fd) {
            controllers_to_delete.push_back((it++)->first);
          } else {
            ++it;
          }
        }
      }

      std::vector<std::string>::iterator delete_it =
          controllers_to_delete.begin();
      for (; controllers_to_delete.end() != delete_it; ++delete_it) {
        deleteController(*delete_it);
      }
   }

   bool CMessageBrokerRegistry::addSubscriber(int fd, std::string name)
   {
      DBG_MSG(("CMessageBrokerRegistry::addSubscriber()\n"));
      bool result = true;

      sync_primitives::AutoLock lock(mSubscribersListLock);
      std::pair<std::multimap <std::string, int>::iterator, std::multimap <std::string, int>::iterator> p = mSubscribersList.equal_range(name);
      if (p.first != p.second)
      {
         std::multimap <std::string, int>::iterator itr;
         for (itr = p.first; itr != p.second; itr++)
         {
            if (fd == itr->second)
            {
               result = false;
               DBG_MSG(("Subscriber already exists!\n"));
            }
         }
      }
      if (result)
      {
         mSubscribersList.insert(std::map <std::string, int>::value_type(name, fd));
      }

      DBG_MSG(("Count of subscribers: %d\n", mSubscribersList.size()));
      return result;
   }

   void CMessageBrokerRegistry::deleteSubscriber(int fd, std::string name)
   {
       DBG_MSG(("CMessageBrokerRegistry::deleteSubscriber()\n"));

       sync_primitives::AutoLock lock(mSubscribersListLock);
       std::pair<std::multimap <std::string, int>::iterator, std::multimap <std::string, int>::iterator> p = mSubscribersList.equal_range(name);
       if (p.first != p.second) {
           std::multimap <std::string, int>::iterator itr;
           for (itr = p.first; itr != p.second; ) {
               DBG_MSG(("My for loop %s, %d", itr->first.c_str() ,itr->second));
               if (fd == itr->second) {
                   mSubscribersList.erase(itr++);
               } else {
                   ++itr;
               }
           }
       }

       DBG_MSG(("Count of subscribers: %d\n", mSubscribersList.size()));
   }

   int CMessageBrokerRegistry::getDestinationFd(std::string name)
   {
      DBG_MSG(("CMessageBrokerRegistry::getDestinationFd()\n"));
      int result = -1;
      std::map <std::string, int>::iterator it;

      sync_primitives::AutoLock lock(mControllersListLock);
      it = mControllersList.find(name);
      if (it != mControllersList.end())
      {
         result = it->second;
      }

      DBG_MSG(("Controllers Fd: %d\n", result));
      return result;
   }

   int CMessageBrokerRegistry::getSubscribersFd(std::string name, std::vector<int>& result)
   {
      DBG_MSG(("CMessageBrokerRegistry::getSubscribersFd()\n"));
      int res = 0;
      std::map <std::string, int>::iterator it;

      sync_primitives::AutoLock lock(mSubscribersListLock);
      std::pair<std::multimap <std::string, int>::iterator, std::multimap <std::string, int>::iterator> p = mSubscribersList.equal_range(name);
      if (p.first != p.second)
      {
         std::multimap <std::string, int>::iterator itr;
         for (itr = p.first; itr != p.second; itr++)
         {
            result.push_back(itr->second);
            DBG_MSG(("Controllers Fd: %d\n", itr->second));
         }
      }

      res = result.size();
      DBG_MSG(("Result vector size: %d\n", res));
      return res;
   }
} /* namespace NsMessageBroker */
