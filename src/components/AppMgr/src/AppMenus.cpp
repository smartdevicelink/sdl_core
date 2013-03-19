//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "AppMgr/AppMenus.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger AppMenus::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMenus"));

    /**
     * \brief Default class constructor
     */
    AppMenus::AppMenus()
    {
    }

    /**
     * \brief Default class destructor
     */
    AppMenus::~AppMenus()
    {
        clear();
    }

    /**
     * \brief add a menu item to a mapping
     * \param menuId menu id
     * \param menuName menu item name
     * \param position menu item position within the parent menu
     */
    void AppMenus::addItem(const unsigned int &menuId, const std::string &menuName, const unsigned int *position)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding a menu item " << menuId << " name " << menuName );
        mMenuItems.insert(MenuItem(menuId, MenuValue(menuName, position)));
    }

    /**
     * \brief remove a menu item from a mapping
     * \param menuId menu id
     */
    void AppMenus::removeItem(const unsigned int &menuId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Deleting a menu item " << menuId );
        mMenuItems.erase(menuId);
    }

    /**
     * \brief find a menu item from a mapping
     * \param menuId menu id
     * \return a menu item from a mapping
     */
    const MenuValue *AppMenus::findItem(const unsigned int &menuId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for a menu item " << menuId );
        const MenuItems::const_iterator& it = mMenuItems.find(menuId);
        if(it != mMenuItems.end())
        {
            const MenuItem& item = *it;
            if(menuId == item.first)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Found a menu item for id " << menuId << " !");
                return &item.second;
            }
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "A menu item " << menuId << " has not been found!");
        return 0;
    }

    /**
     * \brief gets all menu items
     * \return menu items
     */
    MenuItems AppMenus::getAllMenuItems() const
    {
        return mMenuItems;
    }

    /**
     * \brief get count of commands
     * \return commands count
     */
    size_t AppMenus::size() const
    {
        return mMenuItems.size();
    }

    /**
     * \brief cleans all the items
     */
    void AppMenus::clear()
    {
        mMenuItems.clear();
    }

    /**
     * \brief Copy constructor
     */
    AppMenus::AppMenus(const AppMenus &)
    {
    }
}

