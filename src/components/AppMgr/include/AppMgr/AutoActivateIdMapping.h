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

#ifndef AUTOACTIVATEIDMAPPING_H
#define AUTOACTIVATEIDMAPPING_H

#include <string>
#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief a mapping between application and its auto-activate id (application name is a key)
     */
    typedef std::map<std::string, std::string> AutoActivateIDs;

    /**
     * \brief an element of a mapping between application and its auto-activate id (application name is a key)
     */
    typedef std::pair<std::string, std::string> AutoActivateID;

    /**
     * \brief AutoActivateIdMapping acts as a mapping of auto-activate ids to names of registered applications
     * \brief An autoActivateId is being assigned automatically to a newly-registered app name
     */
    class AutoActivateIdMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        AutoActivateIdMapping();

        /**
         * \brief Default class destructor
         */
        ~AutoActivateIdMapping();

        /**
         * \brief add an application to a mapping
         * \param appName application to be added to mapping
         * \return auto-activate id assigned to an application
         */
        std::string addApplicationName( const std::string& appName );

        /**
         * \brief remove an application from a mapping
         * \param appName application to remove all associated ids from mapping
         */
        void removeApplicationName( const std::string& appName );

        /**
         * \brief find an auto-activate id subscribed to message
         * \param name application name
         * \return auto-activate id
         */
        std::string findAutoActivateIdAssignedToName(const std::string& name) const;

        /**
         * \brief remove all mappings
         */
        void clear();

    private:

        /**
         * \brief Copy constructor
         */
        AutoActivateIdMapping(const AutoActivateIdMapping&);

        /**
         * \brief add an id and an application to a mapping
         * \param appName application name
         * \param id auto-activate id
         */
        void addId( const std::string& appName, const std::string& id );

        /**
         * \brief remove an auto-activate id from a mapping
         * \param id auto-activate id
         */
        void removeId(const std::string& id);

        AutoActivateIDs  mAutoActivateIds;
        unsigned long mLastAutoActivateId;

        static log4cplus::Logger mLogger;
    };

}

#endif // AUTOACTIVATEIDMAPPING_H
