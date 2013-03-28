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

namespace NsAppManager
{

    log4cplus::Logger AutoActivateIdMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AutoActivateIdMapping"));

    /**
     * \brief Default class constructor
     */
    AutoActivateIdMapping::AutoActivateIdMapping()
        :mLastAutoActivateId(0)
    {
    }

    /**
     * \brief Default class destructor
     */
    AutoActivateIdMapping::~AutoActivateIdMapping()
    {
        mAutoActivateIds.clear();
    }

    /**
     * \brief add an application to a mapping
     * \param appName application to be added to mapping
     * \return auto-activate id assigned to an application
     */
    std::string AutoActivateIdMapping::addApplicationName(const std::string &appName)
    {
        if(appName.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Trying to add a null-named application!" );
            return "";
        }
        std::stringstream st(std::stringstream::in | std::stringstream::out);
        st << mLastAutoActivateId;
        std::string strAAId = "";
        st >> strAAId;
        addId(appName, strAAId);
        mLastAutoActivateId++;
        LOG4CPLUS_INFO_EXT(mLogger, "Added an application " << appName << " to auto-activate id " << strAAId );
        return strAAId;
    }

    /**
     * \brief add an id and an application to a mapping
     * \param appName application name
     * \param id auto-activate id
     */
    void AutoActivateIdMapping::addId(const std::string &appName, const std::string &id)
    {
        if(appName.empty() || id.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Trying to add a null-named application or to a null id value!" );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Added an application " << appName << " to auto-activate id " << id );
        mAutoActivateIds.insert(AutoActivateID(appName, id));
    }

    /**
     * \brief remove an auto-activate id from a mapping
     * \param id auto-activate id
     */
    void AutoActivateIdMapping::removeId(const std::string &id)
    {
        if(id.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Trying to add a null id!" );
            return;
        }
        if(mAutoActivateIds.empty())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Nothing to remove: a map is empty!" );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Removing an auto-activate id " << id );
        for(AutoActivateIDs::iterator it = mAutoActivateIds.begin(); it != mAutoActivateIds.end(); it++)
        {
            if(id == it->second)
            {
                mAutoActivateIds.erase(it->first);
            }
        }
    }

    /**
     * \brief remove an application from a mapping
     * \param appName application to remove all associated ids from mapping
     */
    void AutoActivateIdMapping::removeApplicationName(const std::string &appName)
    {
        if(appName.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Trying to remove a null-named application!" );
            return;
        }
        if(mAutoActivateIds.empty())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Nothing to remove: a map is empty!" );
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Removing an application " << appName );
        mAutoActivateIds.erase(appName);
    }

    /**
     * \brief find an auto-activate id subscribed to message
     * \param name application name
     * \return auto-activate id
     */
    std::string AutoActivateIdMapping::findAutoActivateIdAssignedToName(const std::string &name) const
    {
        if(name.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Trying to search for a null-named application!" );
            return "";
        }
        if(mAutoActivateIds.empty())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Nothing to find: a map is empty!" );
            return "";
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Found " << mAutoActivateIds.size() << " elements in autoactivateid map");
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for an auto-activate id for the application " << name );
        AutoActivateIDs::const_iterator it = mAutoActivateIds.find( name );
        if ( it != mAutoActivateIds.end() )
        {
            const std::string& aaId = it->second;
            LOG4CPLUS_INFO_EXT(mLogger, "An application " << name << " has auto activate id " << aaId );
            return aaId;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Application " << name << " not found in subscribed." );
        return "";
    }

    /**
     * \brief remove all mappings
     */
    void AutoActivateIdMapping::clear()
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Clearing a mapping" );
        mAutoActivateIds.clear();
    }

    /**
     * \brief Copy constructor
     */
    AutoActivateIdMapping::AutoActivateIdMapping(const AutoActivateIdMapping &)
    {
    }

}
