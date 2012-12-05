/**
 * \file Application.h
 * \brief Application metaphor
 * \author vsalo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "AppMgr/MenuMapping.h"
#include "AppMgr/CommandMapping.h"
#include "AppMgr/AppMenus.h"
#include "AppMgr/AppChoiceSets.h"
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <cstddef>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief An application unique id - combination of connection id and session id
     */
    typedef std::tuple<int, unsigned char> ApplicationUniqueID;

    /**
     * \brief class Application acts as a metaphor for every mobile application being registered on HMI
     */
    class Application
    {
    public:

        /**
         * \brief Class constructor
         * \param name application name
         * \param connectionId id of the connection associated with this application
         * \param sessionId id of the session associated with this application
         * \param appId application id
         * \param protocolVersion protocol version
         */
        Application(const std::string& name, unsigned int connectionId, unsigned char sessionId, int appId, unsigned int protocolVersion);

        /**
         * \brief Default class destructor
         */
        virtual ~Application( );

        /**
         * \brief operator ==
         * \param item the item to compare with
         * \return comparison result
         */
        bool operator==(const Application& item) const;

        /**
         * \brief Set application NGN media screen app name
         * \param value application NGN media screen app name
         */
        void setNgnMediaScreenAppName(const std::string& value);

        /**
         * \brief Set application voice recognition synonyms
         * \param value application voice recognition synonyms
         */
        void setVrSynonyms(const std::vector<std::string>& value);

        /**
         * \brief Set if the application is a media application
         * \param value is the application a media application
         */
        void setIsMediaApplication(bool value);

        /**
         * \brief retrieve application NGN media screen application name
         * \return application NGN media screen application name
         */
        const std::string& getNgnMediaScreenAppName( ) const;

        /**
         * \brief retrieve application voice-recognition synonyms
         * \return application voice-recognition synonyms
         */
        const std::vector<std::string>& getVrSynonyms( ) const;

        /**
         * \brief retrieve is the application a media application
         * \return is the application a media application
         */
        bool getIsMediaApplication( ) const;

        /**
         * \brief retrieve application session ID
         * \return application session ID
         */
        const unsigned char& getSessionID() const;

        /**
         * \brief retrieve application session ID
         * \return application connection ID
         */
        const unsigned int& getConnectionID() const;

        /**
         * \brief retrieve application protocol version
         * \return application protocol version
         */
        const unsigned int& getProtocolVersion() const;

        /**
         * \brief retrieve application name
         * \return application name
         */
        const std::string& getName() const;

        /**
         * \brief retrieve application ID
         * \return application ID
         */
        const int& getAppID( ) const;

        /**
         * \brief add an interaction choice set item to the application
         * \param choiceSetId interaction choice set id
         * \param choiceSet interaction choice set
         */
        void addChoiceSet(const unsigned int& choiceSetId, const ChoiceSet& choiceSet);

        /**
         * \brief remove an interaction choice set from the application
         * \param choiceSetId interaction choice set id
         */
        void removeChoiceSet(const unsigned int& choiceSetId);

        /**
         * \brief gets all interaction choice set items
         * \return interaction choice set items
         */
        ChoiceSetItems getAllChoiceSets() const;

        /**
         * \brief get count of interaction choice sets
         * \return interaction choice sets count
         */
        size_t getChoiceSetsCount() const;

        /**
         * \brief add a command to a menu
         * \param commandId command id
         * \param menuId menu id
         */
        void addMenuCommand(const unsigned int &commandId, const unsigned int &menuId );

        /**
         * \brief remove a command from a menu(s)
         * \param commandId command id
         */
        void removeMenuCommand(const unsigned int& commandId);

        /**
         * \brief find commands within a menu
         * \param menuId menu id
         * \return commands residing within the given menu
         */
        MenuCommands findMenuCommands(const unsigned int &menuId) const;

        /**
         * \brief add a menu item to the application
         * \param menuId menu id
         * \param menuName menu item name
         * \param position menu item position within the parent menu
         */
        void addMenu(const unsigned int &menuId, const std::string& menuName, const unsigned int* position=0);

        /**
         * \brief remove a menu item from the application
         * \param menuId menu id
         */
        void removeMenu(const unsigned int& menuId);

        /**
         * \brief gets all application menus
         * \return application menus
         */
        MenuItems getAllMenus() const;

        /**
         * \brief get count of items
         * \return items count
         */
        size_t getMenusCount() const;

        /**
         * \brief get count of items
         * \return items count
         */
        size_t getMenuCommandsCount() const;

        /**
         * \brief add a command to an application
         * \param commandId command id
         * \param type command type
         * \param params VR or UI params supplied with the AddCommand request
         */
        void addCommand(unsigned int commandId, CommandType type , CommandParams params);

        /**
         * \brief remove a command from application
         * \param commandId command id
         * \param type a type of a command
         */
        void removeCommand(unsigned int commandId, CommandType type);

        /**
         * \brief finds commands in application
         * \param commandId command id
         * \return command list
         */
        Commands findCommands(unsigned int commandId) const;

        /**
         * \brief gets all application commands
         * \return application commands
         */
        Commands getAllCommands() const;

        /**
         * \brief get count of items
         * \return items count
         */
        size_t getCommandsCount() const;

        /**
         * \brief retrieve types associated with command id in current application
         * \param commandId command id to search for types
         * \return input container of command types to be filled with result
         */
        CommandTypes getCommandTypes(unsigned int commandId) const;

        /**
         * \brief get count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to count unresponded requests for
         * \return unresponded requests count
         */
        unsigned int getUnrespondedRequestCount(const unsigned int& cmdId) const;

        /**
         * \brief increment count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to increment unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int incrementUnrespondedRequestCount(const unsigned int& cmdId);

        /**
         * \brief decrement count of unresponsed requests associated with the given command id
         * \param cmdId id of command we need to decrement unresponded request count for
         * \return unresponded requests count after the operation
         */
        unsigned int decrementUnrespondedRequestCount(const unsigned int& cmdId);

    protected:
        static log4cplus::Logger mLogger;

        /**
         * \brief Copy constructor
         */
        Application(const Application& );

        const std::string mName;
        const unsigned char mSessionID;
        const unsigned int mConnectionID;
        const unsigned int mProtocolVersion;
        const int mAppID;
        std::string mNgnMediaScreenAppName;
        std::vector<std::string> mVrSynonyms;
        bool m_bIsMediaApplication;
        MenuMapping mMenuMapping;
        CommandMapping   mCommandMapping;
        AppMenus mMenus;
        AppChoiceSets mChoiceSets;
    };

} // namespace NsAppManager

#endif /* APPLICATION_H_ */
