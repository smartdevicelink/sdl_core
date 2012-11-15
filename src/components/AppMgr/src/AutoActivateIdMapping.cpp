#include "AppMgr/AutoActivateIdMapping.h"
#include <sstream>

namespace NsAppManager
{

/**
 * \brief Default class constructor
 */
AutoActivateIdMapping::AutoActivateIdMapping()
    :mLastAutoActivateId(0)
{
}

/**
 * \brief add an application to a mapping
 * \param app application to be added to mapping
 * \return auto-activate id assigned to an application
 */
const std::string& AutoActivateIdMapping::addApplicationName(const std::string &appName)
{
    std::stringstream st;
    st << mLastAutoActivateId;
    std::string strAAId = st.str();
    addId(appName, strAAId);
    mLastAutoActivateId++;
}

/**
 * \brief add a message to a mapping
 * \param msgId message id
 * \param app application to map a message to
 */
void AutoActivateIdMapping::addId(const std::string &appName, const std::string &id)
{
    mAutoActivateIds.insert(AutoActivateID(appName, id));
}

/**
 * \brief remove a message from a mapping
 * \param msgId message id
 */
void AutoActivateIdMapping::removeId(const std::string &id)
{
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
 * \param app application to remove all associated messages from mapping
 */
void AutoActivateIdMapping::removeApplicationName(const std::string &appName)
{
    mAutoActivateIds.erase(appName);
}

/**
 * \brief find a registry item subscribed to message
 * \param msgId message id
 * \return RegistryItem instance
 */
std::string AutoActivateIdMapping::findAutoActivateIdAssignedToName(const std::string &id) const
{
    AutoActivateIDs::const_iterator it = mAutoActivateIds.find( id );
    if ( it != mAutoActivateIds.end() )
    {
        return it->second;
    }
    return "";
}

/**
 * \brief remove all mappings
 */
void AutoActivateIdMapping::clear()
{
    mAutoActivateIds.clear();
}

/**
 * \brief Copy constructor
 */
AutoActivateIdMapping::AutoActivateIdMapping(const AutoActivateIdMapping &)
{
}

}
