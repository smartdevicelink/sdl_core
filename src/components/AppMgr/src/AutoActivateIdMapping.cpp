#include "AppMgr/AutoActivateIdMapping.h"
#include <sstream>
#include "LoggerHelper.hpp"

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
 * \brief add an application to a mapping
 * \param appName application to be added to mapping
 * \return auto-activate id assigned to an application
 */
const std::string& AutoActivateIdMapping::addApplicationName(const std::string &appName)
{
    std::stringstream st;
    st << mLastAutoActivateId;
    std::string strAAId = st.str();
    addId(appName, strAAId);
    mLastAutoActivateId++;
    LOG4CPLUS_INFO_EXT(mLogger, "Added an application " << appName << " to auto-activate id " << strAAId );
}

/**
 * \brief add an id and an application to a mapping
 * \param appName application name
 * \param id auto-activate id
 */
void AutoActivateIdMapping::addId(const std::string &appName, const std::string &id)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Added an application " << appName << " to auto-activate id " << id );
    mAutoActivateIds.insert(AutoActivateID(appName, id));
}

/**
 * \brief remove an auto-activate id from a mapping
 * \param id auto-activate id
 */
void AutoActivateIdMapping::removeId(const std::string &id)
{
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
    AutoActivateIDs::const_iterator it = mAutoActivateIds.find( name );
    if ( it != mAutoActivateIds.end() )
    {
        LOG4CPLUS_INFO_EXT(mLogger, "An application "<< name <<" has auto activate id " << it->second );
        return it->second;
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
