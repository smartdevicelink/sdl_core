#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include "Application.h"
#include <string>
#include <tuple>
#include <map>
#include <vector>

#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"
#include "JSONHandler/ALRPCObjects/DisplayCapabilities.h"
#include "JSONHandler/ALRPCObjects/HmiZoneCapabilities.h"
#include "JSONHandler/ALRPCObjects/VrCapabilities.h"
#include "JSONHandler/ALRPCObjects/SpeechCapabilities.h"
#include "JSONHandler/ALRPCObjects/GlobalProperty.h"
#include "JSONHandler/ALRPCObjects/TTSChunk.h"
#include "JSONHandler/ALRPCObjects/MenuParams.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */
namespace NsHMIEmulator
{
    /**
     * \brief RegisteredApplications container
     */
    typedef std::map<std::string, Application*> RegisteredApplications;

    /**
     * \brief RegisteredApplications container item
     */
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
     * \brief HelpPrompt container
     */
    typedef std::vector<NsAppLinkRPC::TTSChunk> HelpPrompt;

    /**
     * \brief TimeoutPrompt container
     */
    typedef std::vector<NsAppLinkRPC::TTSChunk> TimeoutPrompt;

    /**
     * \brief GlobalProperties container
     */
    typedef std::vector<NsAppLinkRPC::GlobalProperty> GlobalProperties;

    /**
     * \brief A command_id - menu_params mapping (command id is a key)
     */
    typedef std::map<unsigned int, NsAppLinkRPC::MenuParams> Commands;

    /**
     * \brief A command_id - menu_params mapping item (command id is a key)
     */
    typedef std::pair<unsigned int, NsAppLinkRPC::MenuParams> Command;

    /**
     * \brief A menu item name - menu item position mapping item
     */
    typedef std::pair<std::string, unsigned int> MenuItemBase;

    /**
     * \brief A menu item id - MenuItemBase mapping
     */
    typedef std::map<unsigned int, MenuItemBase> Menu;

    /**
     * \brief A menu item id - MenuItemBase mapping item
     */
    typedef std::pair<unsigned int, MenuItemBase> MenuItem;

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
         * \brief finds an app in a registered apps list
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
         * \brief add a command to a list of registered commands
         * \param id
         * \param menuParams
         */
        void addCommand(const unsigned int& id, const NsAppLinkRPC::MenuParams& menuParams);

        /**
         * \brief remove a command from a list of registered commands
         * \param id
         */
        void removeCommand(const unsigned int& id);

        /**
         * \brief finds a command in a registered commands list
         * \param id
         * \return command
         */
        const Command& findCommand(const unsigned int& id);

        /**
         * \brief add a menu item to a list of registered menu items
         * \param id
         * \param name
         * \param position
         */
        void addMenuItem(const unsigned int& id, const std::string& name, unsigned int position=-1);

        /**
         * \brief remove a menu item from a list of registered menu items
         * \param id
         */
        void removeMenuItem(const unsigned int& id);

        /**
         * \brief finds a menu item in a registered menu items list
         * \param id
         * \return menu item
         */
        const MenuItem& findMenuItem(const unsigned int& id);

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

        /**
         * \brief sets help prompt
         * \param help help prompt
         */
        void setHelpPrompt(const HelpPrompt& help);

        /**
         * \brief gets help prompt
         * \return help prompt
         */
        const HelpPrompt& getHelpPrompt() const;

        /**
         * \brief sets timeout prompt
         * \param timeout timeout prompt
         */
        void setTimeoutPrompt(const TimeoutPrompt& timeout);

        /**
         * \brief gets timeout prompt
         * \return timeout prompt
         */
        const TimeoutPrompt& getTimeoutPrompt() const;

        /**
         * \brief sets global properties
         * \param properties global properties
         */
        void setGlobalProperties(const GlobalProperties& properties);

        /**
         * \brief gets global properties
         * \return global properties
         */
        const GlobalProperties& getGlobalProperties() const;

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
        HelpPrompt mHelpPrompt;
        TimeoutPrompt mTimeoutPrompt;
        GlobalProperties mGlobalProperties;
        Commands mCommands;
        Menu mMenuItems;

        std::string mActiveApplication;
    };
}

#endif // RESOURCECONTAINER_H
