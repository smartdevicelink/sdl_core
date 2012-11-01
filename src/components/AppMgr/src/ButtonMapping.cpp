#include "AppMgr/ButtonMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonMapping"));

ButtonMapping::ButtonMapping()
{
}

void ButtonMapping::addButton(const ButtonName &buttonName, RegistryItem *app)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Subscribe to button " << buttonName.get() << " in app " << app->getApplication()->getName() );
    mButtonsMapping.insert(ButtonMapItem(buttonName, app));
}

void ButtonMapping::removeButton(const ButtonName &buttonName)
{
    mButtonsMapping.erase(buttonName);
}

void ButtonMapping::removeItem(RegistryItem *app)
{
    for(ButtonMap::iterator it = mButtonsMapping.begin(); it != mButtonsMapping.end(); it++)
    {
        if(it->second->getApplication()->getSessionID() == app->getApplication()->getSessionID())
        {
            mButtonsMapping.erase(it->first);
        }
    }
}

RegistryItem* ButtonMapping::findRegistryItemSubscribedToButton( const ButtonName &appName ) const
{
    ButtonMap::const_iterator it = mButtonsMapping.find( appName );
    if ( it != mButtonsMapping.end() )
    {
        if ( !it->second )
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "RegistryItem not found" );
            return 0;
        }
        if ( it->second->getApplication() )
        {
            return it->second;
        }
    }
    LOG4CPLUS_INFO_EXT(mLogger, "Button " << appName.get() << " not found in subscribed." );
    return 0;
}

ButtonMapping::ButtonMapping(const ButtonMapping &)
{
}

bool Comparer::operator ()(const ButtonName &b1, const ButtonName &b2) const
{
    return b1.get() < b2.get();
}

}
