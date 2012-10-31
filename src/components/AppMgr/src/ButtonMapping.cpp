#include "AppMgr/ButtonMapping.h"
#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

log4cplus::Logger ButtonMapping::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ButtonMapping"));

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

ButtonMapping::ButtonMapping(const ButtonMapping &)
{
}

}

