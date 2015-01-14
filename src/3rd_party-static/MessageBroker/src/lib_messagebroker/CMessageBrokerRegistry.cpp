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
      it = mControllersList.find(name);
      if (it != mControllersList.end())
      {
         int fd = it->second;
         mControllersList.erase(it);
         std::multimap <std::string, int>::iterator it_s = mSubscribersList.begin();
         for (; it_s !=mSubscribersList.end(); ) {
           if (it_s->second == fd) {
             mSubscribersList.erase(it_s++);
           } else {
             ++it_s;
           }
         }
      } else
      {
         DBG_MSG(("No such controller in the list!\n"));
      }
      DBG_MSG(("Count of controllers: %d\n", mControllersList.size()));
   }

   bool CMessageBrokerRegistry::addSubscriber(int fd, std::string name)
   {
      DBG_MSG(("CMessageBrokerRegistry::addSubscriber()\n"));
      bool result = true;
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
