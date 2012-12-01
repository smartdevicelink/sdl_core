#include "ResourceContainer.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{
    log4cplus::Logger ResourceContainer::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));

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
        LOG4CPLUS_INFO_EXT(mLogger, "Adding an app " << name);
        mApplications.insert(RegisteredApplication(name, app));
    }

    /**
     * \brief remove an app from a list of registered apps
     * \param name
     */
    void ResourceContainer::removeApplication(const std::string &name)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing an app " << name);
        mApplications.erase(name);
    }

    /**
     * \brief fings an app in a registered apps list
     * \param name
     * \return application
     */
    Application *ResourceContainer::findApplication(const std::string &name) const
    {
        RegisteredApplications::const_iterator it = mApplications.find(name);
        if(it != mApplications.end())
        {
            return it->second;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Cannot find an app!" << name);
        return 0;
    }

    /**
     * \brief sets which app is HMI_FULL
     * \param name HMI_FULL app name
     * \return an activated app
     */
    Application *ResourceContainer::setActiveApplication(const std::string &name)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Activating an app!" << name);
        Application* app = findApplication(name);
        if(app)
        {
            mActiveApplication = name;
            return app;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Activation of app " << name << " failed: app not found");
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
        LOG4CPLUS_ERROR_EXT(mLogger, "No application is active!");
        return 0;
    }

    /**
     * \brief add a command to a list of registered commands
     * \param id
     * \param menuParams
     */
    void ResourceContainer::addUiCommand(const unsigned int &id, const NsAppLinkRPC::MenuParams &menuParams)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding UI command " << id);
        mUiCommands.insert(UICommand(id, menuParams));
    }

    /**
     * \brief remove a command from a list of registered commands
     * \param id
     */
    void ResourceContainer::removeUiCommand(const unsigned int &id)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing UI command " << id);
        mUiCommands.erase(id);
    }

    /**
     * \brief finds a command in a registered commands list
     * \param id
     * \return command
     */
    const NsAppLinkRPC::MenuParams* ResourceContainer::findUiCommand(const unsigned int &id) const
    {
        UICommands::const_iterator it = mUiCommands.find(id);
        if(it != mUiCommands.end())
        {
            return &it->second;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Cannot find UI command " << id);
        return 0; //empty container
    }

    /**
     * \brief add a command to a list of registered commands
     * \param id
     * \param vrCommands
     */
    void ResourceContainer::addVrCommand(const unsigned int &id, const VrCommandsBase &vrCommands)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding VR command " << id);
        mVrCommands.insert(VrCommand(id, vrCommands));
    }

    /**
     * \brief remove a command from a list of registered commands
     * \param id
     */
    void ResourceContainer::removeVrCommand(const unsigned int &id)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Removing VR command " << id);
        mVrCommands.erase(id);
    }

    /**
     * \brief finds a command in a registered commands list
     * \param id
     * \return command
     */
    VrCommandsBase ResourceContainer::findVrCommand(const unsigned int &id) const
    {
        VrCommands::const_iterator it = mVrCommands.find(id);
        if(it != mVrCommands.end())
        {
            return it->second;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Cannot find VR command " << id);
        return VrCommandsBase(); //empty container
    }

    /**
     * \brief add a menu item to a list of registered menu items
     * \param id
     * \param name
     * \param position
     */
    void ResourceContainer::addMenuItem(const unsigned int &id, const std::string &name, unsigned int position)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding menu item " << id << " with name " << name << " at position " << position);
        mMenuItems.insert(MenuItem(id, MenuItemBase(name, position)));
    }

    /**
     * \brief remove a menu item from a list of registered menu items
     * \param id
     */
    void ResourceContainer::removeMenuItem(const unsigned int &id)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Deleting menu item " << id);
        mMenuItems.erase(id);
    }

    /**
     * \brief finds a menu item in a registered menu items list
     * \param id
     * \return menu item
     */
    MenuItemBase ResourceContainer::findMenuItem(const unsigned int &id)
    {
        Menu::const_iterator it = mMenuItems.find(id);
        if(it != mMenuItems.end())
        {
            return it->second;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Cannot find menu item " << id);
        return MenuItemBase(); //an empty container
    }

    /**
     * \brief add an interaction choice set to a list of registered interaction choice sets
     * \param id
     * \param set
     */
    void ResourceContainer::addInteractionChoiceSet(const unsigned int &id, const ChoiceSet &set)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding interaction choice set " << id);
        mInteractionChoiceSet.insert(InteractionChoiceSetItem(id, set));
    }

    /**
     * \brief remove an interaction choice set from a list of registered interaction choice sets
     * \param id
     */
    void ResourceContainer::removeInteractionChoiceSet(const unsigned int &id)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Deleting interaction choice set " << id);
        mInteractionChoiceSet.erase(id);
    }

    /**
     * \brief finds an interaction choice set in a registered interaction choice sets list
     * \param id
     * \return interaction choice set
     */
    ChoiceSet ResourceContainer::findInteractionChoiceSet(const unsigned int &id)
    {
        InteractionChoiceSet::const_iterator it = mInteractionChoiceSet.find(id);
        if(it != mInteractionChoiceSet.end())
        {
            return it->second;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, "Cannot find interaction choice set " << id);
        return ChoiceSet(); //an empty container
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
     * \brief sets help prompt
     * \param help help prompt
     */
    void ResourceContainer::setHelpPrompt(const HelpPrompt &help)
    {
        mHelpPrompt = help;
    }

    /**
     * \brief gets help prompt
     * \return help prompt
     */
    const HelpPrompt &ResourceContainer::getHelpPrompt() const
    {
        return mHelpPrompt;
    }

    /**
     * \brief sets timeout prompt
     * \param timeout timeout prompt
     */
    void ResourceContainer::setTimeoutPrompt(const TimeoutPrompt &timeout)
    {
        mTimeoutPrompt = timeout;
    }

    /**
     * \brief gets timeout prompt
     * \return timeout prompt
     */
    const TimeoutPrompt &ResourceContainer::getTimeoutPrompt() const
    {
        return mTimeoutPrompt;
    }

    /**
     * \brief sets global properties
     * \param properties global properties
     */
    void ResourceContainer::setGlobalProperties(const GlobalProperties &properties)
    {
        mGlobalProperties = properties;
    }

    /**
     * \brief gets global properties
     * \return global properties
     */
    const GlobalProperties &ResourceContainer::getGlobalProperties() const
    {
        return mGlobalProperties;
    }

    /**
     * \brief sets timer start time
     * \param time timer start time
     */
    void ResourceContainer::setStartTime(const NsAppLinkRPC::StartTime &time)
    {
        mStartTime = time;
    }

    /**
     * \brief gets timer start time
     * \return timer start time
     */
    const NsAppLinkRPC::StartTime &ResourceContainer::getStartTime() const
    {
        return mStartTime;
    }

    /**
     * \brief sets timer update mode
     * \param mode timer update mode
     */
    void ResourceContainer::setTimerUpdateMode(const NsAppLinkRPC::UpdateMode &mode)
    {
        mUpdateMode = mode;
    }

    /**
     * \brief gets timer update mode
     * \return timer update mode
     */
    const NsAppLinkRPC::UpdateMode &ResourceContainer::getTimerUpdateMode() const
    {
        return mUpdateMode;
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
