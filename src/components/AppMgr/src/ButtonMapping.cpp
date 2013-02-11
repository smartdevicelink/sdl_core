/**
 * \file ButtonMapping.cpp
 * \brief Buttons mapping
 * \author vsalo
 */

#include "AppMgr/ButtonMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger ButtonMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonMapping"));

    /**
     * \brief Default class constructor
     */
    ButtonMapping::ButtonMapping()
    {
    }

    /**
     * \brief Default class destructor
     */
    ButtonMapping::~ButtonMapping()
    {
        clear();
    }

    /**
     * \brief add a button to a mapping
     * \param buttonName button name
     * \param app application to map a button to
     */
    void ButtonMapping::addButton(const NsAppLinkRPCV2::ButtonName &buttonName, RegistryItem *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Adding a button to a null registry item");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to button " << buttonName.get() << " in app " << app->getApplication()->getName() );
        mButtonsMapping.insert(ButtonMapItem(buttonName, app));
    }

    /**
     * \brief remove a button from a mapping
     * \param buttonName button name
     */
    void ButtonMapping::removeButton(const NsAppLinkRPCV2::ButtonName &buttonName)
    {
        mButtonsMapping.erase(buttonName);
    }

    /**
     * \brief remove an application from a mapping
     * \param app application to remove all associated buttons from mapping
     */
    void ButtonMapping::removeItem(RegistryItem *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
            return;
        }
        for(ButtonMap::iterator it = mButtonsMapping.begin(); it != mButtonsMapping.end(); it++)
        {
            if(it->second->getApplication()->getAppID() == app->getApplication()->getAppID())
            {
                mButtonsMapping.erase(it->first);
            }
        }
    }

    /**
     * \brief cleans all the mapping
     */
    void ButtonMapping::clear()
    {
        mButtonsMapping.clear();
    }

    /**
     * \brief find a registry item subscribed to button
     * \param btnName button name
     * \return RegistryItem instance
     */
    RegistryItem* ButtonMapping::findRegistryItemSubscribedToButton( const NsAppLinkRPCV2::ButtonName &btnName ) const
    {
        ButtonMap::const_iterator it = mButtonsMapping.find( btnName );
        if ( it != mButtonsMapping.end() )
        {
            if ( !it->second )
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found" );
                return 0;
            }
            if ( it->second->getApplication() )
            {
                LOG4CPLUS_INFO_EXT(mLogger, "An application " << it->second->getApplication()->getName() << " is subscribed to a button " << btnName.get() );
                return it->second;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Button " << btnName.get() << " not found in subscribed." );
        return 0;
    }

    ResultRange ButtonMapping::findSubscribedToButton(const NsAppLinkRPCV2::ButtonName &btnName) const
    {
        return mButtonsMapping.equal_range(btnName);
    }

    /**
     * \brief Copy constructor
     */
    ButtonMapping::ButtonMapping(const ButtonMapping &)
    {
    }

    bool ButtonMapping::exist(const NsAppLinkRPCV2::ButtonName& buttonName, RegistryItem* item)
    {
        ButtonMap::const_iterator it = mButtonsMapping.find(buttonName);
        if (it != mButtonsMapping.end())
        {
            if (it->second && it->second->getApplication())
            {
                if (*(it->second->getApplication()) == *(item->getApplication()))
                {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * \brief comparison operator
     * \param b1 button name 1
     * \param b2 button name 2
     * \return comparison result
     */
    bool Comparer::operator ()(const NsAppLinkRPCV2::ButtonName &b1, const NsAppLinkRPCV2::ButtonName &b2) const
    {
        return b1.get() < b2.get();
    }

}
