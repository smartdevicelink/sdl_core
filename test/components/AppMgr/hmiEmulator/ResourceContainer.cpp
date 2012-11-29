#include "ResourceContainer.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{

    /**
     * \brief Returning class instance
     * \return class instance
     */
    ResourceContainer &ResourceContainer::getInstance()
    {
        static ResourceContainer instance;
        return instance;
    }

    /**
     * \brief add an app to a list of registered apps
     * \param name
     * \param app
     */
    void ResourceContainer::addApplication(const std::string &name, Application *app)
    {
        mApplications.insert(Application(name, app));
    }

    /**
     * \brief remove an app from a list of registered apps
     * \param name
     */
    void ResourceContainer::removeApplication(const std::string &name)
    {
        mApplications.erase(name);
    }

    /**
     * \brief fings an app in a registered apps list
     * \param name
     * \return application
     */
    Application *ResourceContainer::findApplication(const std::string &name)
    {
        return mApplications.find(name);
    }

    /**
     * \brief sets which app is HMI_FULL
     * \param name HMI_FULL app name
     * \return an activated app
     */
    Application *ResourceContainer::setActiveApplication(const std::string &name)
    {

        Application* app = findApplication(name);
        if(app)
        {
            mActiveApplication = name;
            return app;
        }
        return 0;
    }

    /**
     * \brief gets the HMI_FULL app
     * \return an active app
     */
    Application *ResourceContainer::getActiveApplication()
    {
        if(!mActiveApplication.empty())
        {
            return findApplication(mActiveApplication);
        }
        return 0;
    }

    /**
     * \brief gets button capabilities
     * \return button capabilities
     */
    const ButtonCapabilities &ResourceContainer::getButtonCapabilities() const
    {
        return mButtonCapabilities;
    }

    /**
     * \brief gets TTS capabilities
     * \return TTS capabilities
     */
    const SpeechCapabilities &ResourceContainer::getTtsCapabilities() const
    {
        return mSpeechCapabilities;
    }

    /**
     * \brief gets display capabilities
     * \return display capabilities
     */
    const NsAppLinkRPC::DisplayCapabilities &ResourceContainer::getDisplayCapabilities() const
    {
        return mDisplayCapabilities;
    }

    /**
     * \brief gets HMI zone capabilities
     * \return HMI zone capabilities
     */
    const HmiZoneCapabilities &ResourceContainer::getHmiZoneCapabilities() const
    {
        return mHmiZoneCapabilities;
    }

    /**
     * \brief gets VR capabilities
     * \return VR capabilities
     */
    const VrCapabilities &ResourceContainer::getVrCapabilities() const
    {
        return mVrCapabilities;
    }

    /**
     * \brief Default class constructor
     */
    ResourceContainer::ResourceContainer()
    {
    }

    /**
     * \brief Copy constructor
     */
    ResourceContainer::ResourceContainer(const ResourceContainer &)
    {
    }

}
