#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include "Application.h"
#include <string>
#include <tuple>
#include <map>

#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "JSONHandler/ALRPCObjects/HmiZoneCapabilities.h"
#include "JSONHandler/ALRPCObjects/VrCapabilities.h"
#include "JSONHandler/ALRPCObjects/SpeechCapabilities.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{
    typedef std::map<std::string, Application*> RegisteredApplications;

    typedef std::pair<std::string, Application*> RegisteredApplication;

    /**
     * \brief ButtonCapabilities container
     */
    typedef std::vector<NsAppLinkRPC::ButtonCapabilities> ButtonCapabilities;

    /**
     * \brief HmiZoneCapabilities container
     */
    typedef std::vector<NsAppLinkRPC::HmiZoneCapabilities> HmiZoneCapabilities;

    /**
     * \brief VrCapabilities container
     */
    typedef std::vector<NsAppLinkRPC::VrCapabilities> VrCapabilities;

    /**
     * \brief SpeechCapabilities container
     */
    typedef std::vector<NsAppLinkRPC::SpeechCapabilities> SpeechCapabilities;

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

        /**
         * \brief gets button capabilities
         * \return button capabilities
         */
        const ButtonCapabilities& getButtonCapabilities() const;

        /**
         * \brief gets TTS capabilities
         * \return TTS capabilities
         */
        const SpeechCapabilities& getTtsCapabilities() const;

        /**
         * \brief gets display capabilities
         * \return display capabilities
         */
        const NsAppLinkRPC::DisplayCapabilities& getDisplayCapabilities() const;

        /**
         * \brief gets HMI zone capabilities
         * \return HMI zone capabilities
         */
        const HmiZoneCapabilities& getHmiZoneCapabilities() const;

        /**
         * \brief gets VR capabilities
         * \return VR capabilities
         */
        const VrCapabilities& getVrCapabilities() const;

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
        ButtonCapabilities mButtonCapabilities;
        NsAppLinkRPC::DisplayCapabilities mDisplayCapabilities;
        HmiZoneCapabilities mHmiZoneCapabilities;
        VrCapabilities mVrCapabilities;
        SpeechCapabilities mSpeechCapabilities;

        std::string mActiveApplication;
    };
}

#endif // RESOURCECONTAINER_H
