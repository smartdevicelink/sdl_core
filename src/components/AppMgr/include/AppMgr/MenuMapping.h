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

#ifndef MENUMAPPING_H
#define MENUMAPPING_H

#include <map>
#include <vector>
#include <cstddef>
#include "utils/logger.h"

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
    public:
        /**
         * \brief Default class constructor
         */
        MenuMapping();

        /**
         * \brief Default class destructor
         */
        ~MenuMapping();

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
         * \brief get count of items
         * \return items count
         */
        size_t size() const;

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
         * \brief find commands within a menu
         * \param menuId menu id
         * \return commands residing within the given menu
         */
        MenuCommands findCommandsAssignedToMenu(const unsigned int &menuId) const;

        /**
         * \brief cleans menu mapping
         */
        void clear( );

    private:

        /**
         * \brief Copy constructor
         */
        MenuMapping(const MenuMapping&);

        MenuMap    mMenuMapping;
        static log4cxx::LoggerPtr logger_;
    };

}

#endif // MENUMAPPING_H
