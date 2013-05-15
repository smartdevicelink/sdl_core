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

#ifndef COMMANDMAPPING_H
#define COMMANDMAPPING_H

#include <map>
#include <tuple>
#include <vector>
#include <set>
#include <cstddef>
#include "JSONHandler/SDLRPCObjects/V1/MenuParams.h"
#include "JSONHandler/SDLRPCObjects/V2/MenuParams.h"
#include "utils/logger.h"


namespace NsAppManager
{

    class RegistryItem;
    class Application;

    /**
     * \brief CommandType acts as command type enum representation that allows iterating over types in cycle and using types in comparison
     */
    class CommandType
    {
    public:

        /**
         * \brief command types
         */
        typedef enum
        {
            UNDEFINED = -1,
            UI        = 0,
            FIRST     = UI,
            VR        = 1,
            TTS       = 2,
            LAST      = TTS
        } Type;

        /**
         * \brief Default constructor
         */
        CommandType();

        /**
         * \brief Copy constructor
         */
        CommandType(const CommandType& src);

        /**
         * \brief Class constructor
         * \param type command type to create a class with
         */
        CommandType(const Type& type);

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator==(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator==(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator<(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator<(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator>(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator>(const CommandType& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator!=(const Type& type) const;

        /**
         * \brief comparison operator
         * \param type of a command to compare with
         * \return comparison result
         */
        bool operator!=(const CommandType& type) const;

        /**
         * \brief pre-increment operator
         * \return incremented value
         */
        CommandType& operator++ ();

        /**
         * \brief post-increment operator
         * \return incremented value
         */
        CommandType operator++ (int);

        /**
         * \brief get command type
         * \return command type
         */
        const Type& getType() const;

    private:
        Type mType;
    };

    /**
     * \brief Acts as a wrapper of command UI or VR params
     */
    union CommandParams
    {
        const NsSmartDeviceLinkRPCV2::MenuParams* menuParamsV2;
        const NsSmartDeviceLinkRPC::MenuParams* menuParams;
        const std::vector<std::string>* vrCommands;
    };

    /**
     * \brief mapping of command id to specific command type
     */
    typedef std::tuple<unsigned int, CommandType> CommandBase;

    /**
     * \brief mapping of command base to params
     */
    typedef std::pair<CommandBase, CommandParams> Command;

    /**
     * \brief command types associated with command
     */
    typedef std::vector<CommandType> CommandTypes;

    /**
     * \brief commands vector
     */
    typedef std::map<CommandBase, CommandParams> Commands;

    /**
     * \brief command_id-to-request_number map (command id is a key);
     */
    typedef std::map<unsigned int, unsigned int> RequestsAwaitingResponse;

    /**
     * \brief command_id-to-request_number map item (command id is a key);
     */
    typedef std::pair<unsigned int, unsigned int> RequestAwaitingResponse;

    /**
     * \brief CommandMapping acts as a mapping of command to const NsSmartDeviceLinkRPC::MenuParams that are contained in some of them
     */
    class CommandMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        CommandMapping();

        /**
         * \brief Default class destructor
         */
        ~CommandMapping();

        /**
         * \brief add a command to a mapping
         * \param commandId command id
         * \param type command type
         * \param params VR or UI params supplied with the AddCommand request
         */
        void addCommand(unsigned int commandId, const CommandType &type , CommandParams params);

        /**
         * \brief remove a command from a mapping
         * \param commandId command id
         * \param type a type of a command
         */
        void removeCommand(unsigned int commandId, const CommandType &type);

        /**
         * \brief finds commands in mapping
         * \param commandId command id
         * \return true if found, false if not
         */
        bool findCommand(unsigned int commandId, const CommandType &type) const;

        /**
         * \brief finds commands in mapping
         * \param commandId command id
         * \return commands list
         */
        Commands findCommands(unsigned int commandId) const;

        /**
         * \brief gets all commands
         * \return commands
         */
        Commands getAllCommands() const;

        /**
         * \brief retrieve types associated with command id in current mapping
         * \param commandId command id to search for types
         * \return input container of command types to be filled with result
         */
        CommandTypes getTypes(unsigned int commandId) const;

        /**
         * \brief get count of commands
         * \return commands count
         */
        size_t size() const;

        /**
         * \brief get count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to count unresponded requests for
         * \return unresponded requests count
         */
        unsigned int getUnrespondedRequestCount(const unsigned int& cmdId) const;

        /**
         * \brief increment count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to increment unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int incrementUnrespondedRequestCount(const unsigned int& cmdId);

        /**
         * \brief decrement count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to decrement unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int decrementUnrespondedRequestCount(const unsigned int& cmdId);

        /**
         * \brief cleans all the items
         */
        void clear( );

        /**
         * \brief cleans all the requests awaiting response
         */
        void clearUnrespondedRequests( );

    private:

        /**
         * \brief Copy constructor
         */
        CommandMapping(const CommandMapping&);
        Commands mCommands;
        RequestsAwaitingResponse mRequestsPerCommand;
        static log4cxx::LoggerPtr logger_;
    };

}

#endif // COMMANDMAPPING_H
