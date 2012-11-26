#include "AppMgr/AppMenus.h"

namespace NsAppManager
{
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
        mMenuItems.insert(MenuItem(menuId, MenuValue(menuName, position)));
    }

    /**
     * \brief remove a menu item from a mapping
     * \param menuId menu id
     */
    void AppMenus::removeItem(const unsigned int &menuId)
    {
        mMenuItems.erase(menuId);
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

