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
     * \brief Default class destructor
     */
    MenuMapping::~MenuMapping()
    {
        clear();
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
     * \brief get count of items
     * \return items count
     */
    size_t MenuMapping::size() const
    {
        return mMenuMapping.size();
    }

    /**
     * \brief remove an application from a mapping
     * \param menuId id of a menu to remove all commands mapping from
     */
    void MenuMapping::removeMenu(const unsigned int& menuId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing a menu " << menuId );
        const MenuCommands& commands = findCommandsAssignedToMenu(menuId);

        for(MenuCommands::const_iterator it = commands.begin(); it != commands.end(); it++)
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
     * \brief find commands within a menu
     * \param menuId menu id
     * \return commands residing within the given menu
     */
    MenuCommands MenuMapping::findCommandsAssignedToMenu(const unsigned int& menuId) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Finding a command list associated with a menu " << menuId );
        MenuCommands commands;
        for(MenuMap::const_iterator it = mMenuMapping.begin(); it != mMenuMapping.end(); it++)
        {
            const unsigned int& menuIdFound = it->second;
            if(menuId == menuIdFound)
            {
                commands.push_back(it->first);
            }
        }
        return commands;
    }

    /**
     * \brief cleans menu mapping
     */
    void MenuMapping::clear()
    {
        mMenuMapping.clear();
    }

    /**
     * \brief Copy constructor
     */
    MenuMapping::MenuMapping(const MenuMapping &)
    {
    }

}
