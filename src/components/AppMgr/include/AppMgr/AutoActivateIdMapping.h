/**
 * \file AutoActivateIdMapping.h
 * \brief AutoActivateId mapping
 * \author vsalo
 */

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
