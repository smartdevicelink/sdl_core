#ifndef AUTOACTIVATEIDMAPPING_H
#define AUTOACTIVATEIDMAPPING_H

#include <string>
#include <map>

namespace NsAppManager
{

/**
 * \brief a mapping between application and its autoactivateid (application name is a key)
 */
typedef std::map<std::string, std::string> AutoActivateIDs;

/**
 * \brief an element of a mapping between application and its autoactivateid (application name is a key)
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
     * \brief add an application to a mapping
     * \param app application to be added to mapping
     * \return auto-activate id assigned to an application
     */
    const std::string &addApplicationName( const std::string& appName );

    /**
     * \brief remove an application from a mapping
     * \param app application to remove all associated messages from mapping
     */
    void removeApplicationName( const std::string& appName );

    /**
     * \brief find a registry item subscribed to message
     * \param msgId message id
     * \return RegistryItem instance
     */
    std::string findAutoActivateIdAssignedToName(const std::string& id) const;

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
     * \brief add a message to a mapping
     * \param msgId message id
     * \param app application to map a message to
     */
    void addId( const std::string& appName, const std::string& id );

    /**
     * \brief remove a message from a mapping
     * \param msgId message id
     */
    void removeId(const std::string& id);

    AutoActivateIDs  mAutoActivateIds;
    unsigned long mLastAutoActivateId;
};

}

#endif // AUTOACTIVATEIDMAPPING_H
