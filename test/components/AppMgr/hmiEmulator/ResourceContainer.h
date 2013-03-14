#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include "Application.h"
#include <string>
#include <tuple>
#include <map>
#include <vector>

#include "JSONHandler/SDLRPCObjects/ButtonCapabilities.h"
#include "JSONHandler/SDLRPCObjects/DisplayCapabilities.h"
#include "JSONHandler/SDLRPCObjects/HmiZoneCapabilities.h"
#include "JSONHandler/SDLRPCObjects/VrCapabilities.h"
#include "JSONHandler/SDLRPCObjects/SpeechCapabilities.h"
#include "JSONHandler/SDLRPCObjects/GlobalProperty.h"
#include "JSONHandler/SDLRPCObjects/TTSChunk.h"
#include "JSONHandler/SDLRPCObjects/MenuParams.h"
#include "JSONHandler/SDLRPCObjects/Choice.h"
#include "JSONHandler/SDLRPCObjects/MediaClockFormat.h"
#include "JSONHandler/SDLRPCObjects/StartTime.h"
#include "JSONHandler/SDLRPCObjects/UpdateMode.h"
#include "LoggerHelper.hpp"

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
    typedef std::vector<NsSmartDeviceLinkRPC::ButtonCapabilities> ButtonCapabilities;

    /**
     * \brief HmiZoneCapabilities container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::HmiZoneCapabilities> HmiZoneCapabilities;

    /**
     * \brief VrCapabilities container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::VrCapabilities> VrCapabilities;

    /**
     * \brief SpeechCapabilities container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::SpeechCapabilities> SpeechCapabilities;

    /**
     * \brief HelpPrompt container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::TTSChunk> HelpPrompt;

    /**
     * \brief TimeoutPrompt container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::TTSChunk> TimeoutPrompt;

    /**
     * \brief InitialPrompt container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::TTSChunk> InitialPrompt;

    /**
     * \brief GlobalProperties container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::GlobalProperty> GlobalProperties;

    /**
     * \brief ChoiceSet container
     */
    typedef std::vector<NsSmartDeviceLinkRPC::Choice> ChoiceSet;

    /**
     * \brief VrCommands container
     */
    typedef std::vector<std::string> VrCommandsBase;

    /**
     * \brief A command_id - menu_params mapping (command id is a key)
     */
    typedef std::map<unsigned int, NsSmartDeviceLinkRPC::MenuParams> UICommands;

    /**
     * \brief A command_id - menu_params mapping item (command id is a key)
     */
    typedef std::pair<unsigned int, NsSmartDeviceLinkRPC::MenuParams> UICommand;

    /**
     * \brief A command_id - VR commands mapping (command id is a key)
     */
    typedef std::map<unsigned int, VrCommandsBase> VrCommands;

    /**
     * \brief A command_id - VR commands mapping item (command id is a key)
     */
    typedef std::pair<unsigned int, VrCommandsBase> VrCommand;

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
     * \brief An interaction choice set id - InteractionChoiceSet mapping item
     */
    typedef std::map<unsigned int, ChoiceSet> InteractionChoiceSet;

    /**
     * \brief An interaction choice set id - InteractionChoiceSet mapping
     */
    typedef std::pair<unsigned int, ChoiceSet> InteractionChoiceSetItem;

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
        Application* findApplication(const std::string& name) const;

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
        void addUiCommand(const unsigned int& id, const NsSmartDeviceLinkRPC::MenuParams& menuParams);

        /**
         * \brief remove a command from a list of registered commands
         * \param id
         */
        void removeUiCommand(const unsigned int& id);

        /**
         * \brief finds a command in a registered commands list
         * \param id
         * \return command
         */
        const NsSmartDeviceLinkRPC::MenuParams *findUiCommand(const unsigned int& id) const;

        /**
         * \brief add a command to a list of registered commands
         * \param id
         * \param vrCommands
         */
        void addVrCommand(const unsigned int& id, const VrCommandsBase& vrCommands);

        /**
         * \brief remove a command from a list of registered commands
         * \param id
         */
        void removeVrCommand(const unsigned int& id);

        /**
         * \brief finds a command in a registered commands list
         * \param id
         * \return command
         */
        VrCommandsBase findVrCommand(const unsigned int& id) const;

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
        MenuItemBase findMenuItem(const unsigned int& id);

        /**
         * \brief add an interaction choice set to a list of registered interaction choice sets
         * \param id
         * \param set
         */
        void addInteractionChoiceSet(const unsigned int& id, const ChoiceSet& set);

        /**
         * \brief remove an interaction choice set from a list of registered interaction choice sets
         * \param id
         */
        void removeInteractionChoiceSet(const unsigned int& id);

        /**
         * \brief finds an interaction choice set in a registered interaction choice sets list
         * \param id
         * \return interaction choice set
         */
        ChoiceSet findInteractionChoiceSet(const unsigned int& id);

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
        const NsSmartDeviceLinkRPC::DisplayCapabilities& getDisplayCapabilities() const;

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

        /**
         * \brief sets timer start time
         * \param time timer start time
         */
        void setStartTime(const NsSmartDeviceLinkRPC::StartTime& time);

        /**
         * \brief gets timer start time
         * \return timer start time
         */
        const NsSmartDeviceLinkRPC::StartTime& getStartTime() const;

        /**
         * \brief sets timer update mode
         * \param mode timer update mode
         */
        void setTimerUpdateMode(const NsSmartDeviceLinkRPC::UpdateMode& mode);

        /**
         * \brief gets timer update mode
         * \return timer update mode
         */
        const NsSmartDeviceLinkRPC::UpdateMode& getTimerUpdateMode() const;

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
        NsSmartDeviceLinkRPC::DisplayCapabilities mDisplayCapabilities;
        HmiZoneCapabilities mHmiZoneCapabilities;
        VrCapabilities mVrCapabilities;
        SpeechCapabilities mSpeechCapabilities;
        HelpPrompt mHelpPrompt;
        TimeoutPrompt mTimeoutPrompt;
        GlobalProperties mGlobalProperties;
        UICommands mUiCommands;
        VrCommands mVrCommands;
        Menu mMenuItems;
        InteractionChoiceSet mInteractionChoiceSet;
        NsSmartDeviceLinkRPC::StartTime mStartTime;
        NsSmartDeviceLinkRPC::UpdateMode mUpdateMode;
        std::string mActiveApplication;

        static log4cplus::Logger mLogger;
    };
}

#endif // RESOURCECONTAINER_H
