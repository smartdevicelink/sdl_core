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

