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

#ifndef APPMENUS_H
#define APPMENUS_H

#include <map>
#include <string>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief mapping of menu item name to menu item position
     */
    typedef std::pair<std::string, const unsigned int*> MenuValue;

    /**
     * \brief mapping of menu item id to the combination of menu item name and menu item position
     */
    typedef std::pair<unsigned int, MenuValue> MenuItem;

    /**
     * \brief mapping of menu item id to the combination of menu item name and menu item position
     */
    typedef std::map<unsigned int, MenuValue> MenuItems;

    /**
     * \brief AppMenus acts as a mapping between menu items id and respective item names with position
     */
    class AppMenus
    {
    public:

        /**
         * \brief Default class constructor
         */
        AppMenus();

        /**
         * \brief Default class destructor
         */
        ~AppMenus();

        /**
         * \brief add a menu item to a mapping
         * \param menuId menu id
         * \param menuName menu item name
         * \param position menu item position within the parent menu
         */
        void addItem(const unsigned int& menuId, const std::string& menuName, const unsigned int* position);

        /**
         * \brief remove a menu item from a mapping
         * \param menuId menu id
         */
        void removeItem(const unsigned int& menuId);

        /**
         * \brief find a menu item from a mapping
         * \param menuId menu id
         * \return a menu item from a mapping
         */
        const MenuValue* findItem(const unsigned int& menuId);

        /**
         * \brief gets all menu items
         * \return menu items
         */
        MenuItems getAllMenuItems() const;

        /**
         * \brief get count of commands
         * \return commands count
         */
        size_t size() const;

        /**
         * \brief cleans all the items
         */
        void clear();

    private:

        /**
         * \brief Copy constructor
         */
        AppMenus(const AppMenus&);

        MenuItems mMenuItems;
        static log4cplus::Logger mLogger;
    };

}

#endif // APPMENUS_H
