#include "AppMgr/MenuMapping.h"

namespace NsAppManager
{

/**
 * \brief Default class constructor
 */
MenuMapping::MenuMapping()
{
}

/**
 * \brief add a command to a mapping
 * \param commandId command id
 * \param menuId menu id
 */
void MenuMapping::addCommand(const unsigned int& commandId, const unsigned int& menuId)
{
    mMenuMapping.insert(MenuMapItem(commandId, menuId));
}

/**
 * \brief remove a command from a mapping
 * \param commandId command id
 */
void MenuMapping::removeCommand(const unsigned int &commandId)
{
    mMenuMapping.erase(commandId);
}

/**
 * \brief remove an application from a mapping
 * \param menuId id of a menu to remove all commands mapping from
 */
void MenuMapping::removeMenu(const unsigned int& menuId)
{
    MenuCommands commands;
    findCommandsAssignedToMenu(menuId, commands);

    for(MenuCommands::iterator it = commands.begin(); it != commands.end(); it++)
    {
        mMenuMapping.erase(*it);
    }
}

/**
 * \brief find a registry item subscribed to command
 * \param commandId command id
 * \return id of menu where a command resides
 */
unsigned int MenuMapping::findMenuAssignedToCommand(const unsigned int& commandId) const
{
    MenuMap::const_iterator it = mMenuMapping.find(commandId);
    if(it != mMenuMapping.end())
    {
        return it->second;
    }
    return -1;
}

/**
 * \brief find a registry item subscribed to command
 * \param menuId menu id
 * \param commands commands residing within the given menu
 */
void MenuMapping::findCommandsAssignedToMenu(const unsigned int& menuId, MenuCommands &commands) const
{
    commands.clear();
    for(MenuMap::const_iterator it = mMenuMapping.begin(); it != mMenuMapping.end(); it++)
    {
        const unsigned int& menuIdFound = it->second;
        if(menuId == menuIdFound)
        {
            commands.push_back(it->first);
        }
    }
}

/**
 * \brief Copy constructor
 */
MenuMapping::MenuMapping(const MenuMapping &)
{
}

}
