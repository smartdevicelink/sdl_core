#include "AppMgr/ButtonMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonMapping"));

ButtonMapping::ButtonMapping()
{
}

void ButtonMapping::addButton(const AppLinkRPC::ButtonName &buttonName, RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Adding a button to a null registry item");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to button " << buttonName.get() << " in app " << app->getApplication()->getName() );
    mButtonsMapping.insert(ButtonMapItem(buttonName, app));
}

void ButtonMapping::removeButton(const AppLinkRPC::ButtonName &buttonName)
{
    mButtonsMapping.erase(buttonName);
}

void ButtonMapping::removeItem(RegistryItem *app)
{
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
        return;
    }
    for(ButtonMap::iterator it = mButtonsMapping.begin(); it != mButtonsMapping.end(); it++)
    {
        if(it->second->getApplication()->getSessionID() == app->getApplication()->getSessionID())
        {
            mButtonsMapping.erase(it->first);
        }
    }
}


RegistryItem* ButtonMapping::findRegistryItemSubscribedToButton( const AppLinkRPC::ButtonName &btnName ) const
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
            LOG4CPLUS_INFO_EXT(mLogger, "An application "<< it->second->getApplication()->getName() <<" is subscribed to a button " << btnName.get() );
            return it->second;
        }
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Button " << btnName.get() << " not found in subscribed." );
    return 0;
}

ButtonMapping::ButtonMapping(const ButtonMapping &)
{
}

bool Comparer::operator ()(const AppLinkRPC::ButtonName &b1, const AppLinkRPC::ButtonName &b2) const
{
    return b1.get() < b2.get();
}

}
