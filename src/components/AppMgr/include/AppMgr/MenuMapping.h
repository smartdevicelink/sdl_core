#ifndef MENUMAPPING_H
#define MENUMAPPING_H

#include <map>
#include <vector>

namespace NsAppManager
{

/**
 * \brief command-to-menu map (command id is a key: multiple commands can reside within a single menu)
 */
typedef std::map<unsigned int, unsigned int> MenuMap;

/**
 * \brief command-to-menu map item (command id is a key: multiple commands can reside within a single menu)
 */
typedef std::pair<unsigned int, unsigned int> MenuMapItem;

/**
 * \brief commands residing within the same menu
 */
typedef std::vector<unsigned int> MenuCommands;

/**
 * \brief MenuMapping acts as a mapping of command to registsred application that subscribed to them
 */
class MenuMapping
{
    /**
     * \brief Default class constructor
     */
    MenuMapping();

    /**
     * \brief add a command to a mapping
     * \param commandId command id
     * \param menuId menu id
     */
    void addCommand(const unsigned int &commandId, const unsigned int &menuId );

    /**
     * \brief remove a command from a mapping
     * \param commandId command id
     */
    void removeCommand(const unsigned int& commandId);

    /**
     * \brief remove an application from a mapping
     * \param menuId id of a menu to remove all commands mapping from
     */
    void removeMenu(const unsigned int &menuId );

    /**
     * \brief find a registry item subscribed to command
     * \param commandId command id
     * \return id of menu where a command resides
     */
    unsigned int findMenuAssignedToCommand(const unsigned int &commandId) const;

    /**
     * \brief find a registry item subscribed to command
     * \param menuId menu id
     * \param commands commands residing within the given menu
     */
    void findCommandsAssignedToMenu(const unsigned int &menuId, MenuCommands& commands) const;

private:

    /**
     * \brief Copy constructor
     */
    MenuMapping(const MenuMapping&);

    MenuMap    mMenuMapping;
};

}

#endif // MENUMAPPING_H
