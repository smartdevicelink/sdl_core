#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include "Application.h"
#include <string>
#include <tuple>
#include <map>

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{
    typedef std::map<std::string, Application*> RegisteredApplications;

    typedef std::pair<std::string, Application*> RegisteredApplication;

    /**
     * \brief The ResourceContainer class acts as a container of all HMI
     */
    class ResourceContainer
    {
    public:
        /**
         * \brief Returning class instance
         * \return class instance
         */
        static ResourceContainer& getInstance();

        /**
         * \brief add an app to a list of registered apps
         * \param name
         * \param app
         */
        void addApplication(const std::string& name, Application* app);

        /**
         * \brief remove an app from a list of registered apps
         * \param name
         */
        void removeApplication(const std::string& name);

        /**
         * \brief fings an app in a registered apps list
         * \param name
         * \return application
         */
        Application* findApplication(const std::string& name);

        /**
         * \brief sets which app is HMI_FULL
         * \param name HMI_FULL app name
         * \return an activated app
         */
        Application* setActiveApplication(const std::string& name);

        /**
         * \brief gets the HMI_FULL app
         * \return an active app
         */
        Application* getActiveApplication();

    private:
        /**
         * \brief Default class constructor
         */
        ResourceContainer();

        /**
         * \brief Copy constructor
         */
        ResourceContainer(const ResourceContainer&);

        RegisteredApplications mApplications;

        std::string mActiveApplication;
    };
}

#endif // RESOURCECONTAINER_H
