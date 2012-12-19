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
