//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef BUTTONMAPPING_H
#define BUTTONMAPPING_H

#include <map>
#include "JSONHandler/ALRPCObjects/V1/ButtonName.h"
#include "JSONHandler/ALRPCObjects/V2/ButtonName.h"
#include "AppMgr/Application.h"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    class Application;

    /**
     * \brief Comparer acts as a comparer that allows to create std::map with NsAppLinkRPC::ButtonName as a key
     */
    struct Comparer {
        bool operator() (const NsAppLinkRPCV2::ButtonName &b1, const NsAppLinkRPCV2::ButtonName &b2) const;
    };

    /**
     * \brief a button_name-registered-app map
     */
    typedef std::multimap<NsAppLinkRPCV2::ButtonName, Application*, Comparer> ButtonMap;

    /**
     * \brief a button_name-registered-app map item
     */
    typedef std::pair<NsAppLinkRPCV2::ButtonName, Application*> ButtonMapItem;

    /**
      *\brief Iterators of beginning and end of result range
    */
    typedef std::pair<ButtonMap::const_iterator, ButtonMap::const_iterator> ResultRange;

    /**
     * \brief ButtonMapping acts as a mapping of buttons to registered application which subscribes to them
     */
    class ButtonMapping
    {
    public:

        /**
         * \brief Default class constructor
         */
        ButtonMapping();

        /**
         * \brief Default class destructor
         */
        ~ButtonMapping();

        /**
         * \brief add a button to a mapping
         * \param buttonName button name
         * \param app application to map a button to
         */
        void addButton( const NsAppLinkRPCV2::ButtonName& buttonName, Application* app );

        /**
         * \brief remove a button from a mapping
         * \param buttonName button name
         */
        void removeButton(const NsAppLinkRPCV2::ButtonName& buttonName, Application * item);

        /**
         * \brief remove an application from a mapping
         * \param app application to remove all associated buttons from mapping
         */
        void removeItem( Application* app );

        /**
         * \brief cleans all the mapping
         */
        void clear( );

        /**
         * \brief find a registry item subscribed to button
         * \param btnName button name
         * \return Application instance
         */
        Application *findRegistryItemSubscribedToButton(const NsAppLinkRPCV2::ButtonName &btnName) const;

        /**
         * \brief Find all apps subscribed to button
         * \param btnName Button name
         * \return @ResultRange Results range
         */
        ResultRange findSubscribedToButton(const NsAppLinkRPCV2::ButtonName &btnName) const;

        bool exist(const NsAppLinkRPCV2::ButtonName& buttonName, Application* item);
    private:

        /**
         * \brief Copy constructor
         */
        ButtonMapping(const ButtonMapping&);

        ButtonMap    mButtonsMapping;
        static log4cplus::Logger mLogger;
    };

}

#endif // BUTTONMAPPING_H
