#include "AppMgr/MenuMapping.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger MenuMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("MenuMapping"));

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
    LOG4CPLUS_INFO_EXT(mLogger, "Inserting a command " << commandId << " into a menu " << menuId );
    mMenuMapping.insert(MenuMapItem(commandId, menuId));
}

/**
 * \brief remove a command from a mapping
 * \param commandId command id
 */
void MenuMapping::removeCommand(const unsigned int &commandId)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Removing a command " << commandId );
    mMenuMapping.erase(commandId);
}

/**
 * \brief remove an application from a mapping
 * \param menuId id of a menu to remove all commands mapping from
 */
void MenuMapping::removeMenu(const unsigned int& menuId)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Removing a menu " << menuId );
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
    LOG4CPLUS_INFO_EXT(mLogger, "Finding a menu associated with a command " << commandId );
    MenuMap::const_iterator it = mMenuMapping.find(commandId);
    if(it != mMenuMapping.end())
    {
        return it->second;
    }
    LOG4CPLUS_ERROR_EXT(mLogger, "Command " << commandId << " isn't associated with any menu" );
    return -1;
}

/**
 * \brief find a registry item subscribed to command
 * \param menuId menu id
 * \param commands commands residing within the given menu
 */
void MenuMapping::findCommandsAssignedToMenu(const unsigned int& menuId, MenuCommands &commands) const
{
    LOG4CPLUS_INFO_EXT(mLogger, "Finding a command list associated with a menu " << menuId );
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
