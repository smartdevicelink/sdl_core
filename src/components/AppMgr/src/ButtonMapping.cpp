//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

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
    void ButtonMapping::addButton(const NsSmartDeviceLinkRPCV2::ButtonName &buttonName, Application *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Adding a button to a null registry item");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Subscribed to button " << buttonName.get() << " in app " << app->getName() );
        mButtonsMapping.insert(ButtonMapItem(buttonName, app));
    }

    /**
     * \brief remove a button from a mapping
     * \param buttonName button name
     */
    void ButtonMapping::removeButton(const NsSmartDeviceLinkRPCV2::ButtonName &buttonName, Application * item)
    {
        //mButtonsMapping.erase(buttonName);

        ResultRange result = mButtonsMapping.equal_range(buttonName);
        for( ButtonMap::const_iterator it=result.first; it!=result.second; ++it )
        {
            if ( it->second->getAppID() == item->getAppID() )
            {
                mButtonsMapping.erase(it);
                break;
            }
        }
    }

    /**
     * \brief remove an application from a mapping
     * \param app application to remove all associated buttons from mapping
     */
    void ButtonMapping::removeItem(Application *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to remove a null item");
            return;
        }
        for(ButtonMap::iterator it = mButtonsMapping.begin(); 
                it != mButtonsMapping.end(); it++)
        {
            if(it->second->getAppID() == app->getAppID())
            {
                mButtonsMapping.erase(it);
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
    Application* ButtonMapping::findRegistryItemSubscribedToButton( const NsSmartDeviceLinkRPCV2::ButtonName &btnName ) const
    {
        ButtonMap::const_iterator it = mButtonsMapping.find( btnName );
        if ( it != mButtonsMapping.end() )
        {
            if ( !it->second )
            {                
                LOG4CPLUS_INFO_EXT(mLogger, "An application " <<
                        it->second->getName() << " is subscribed to a button "
                        << btnName.get() );
                
                return it->second;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Button " << btnName.get() << " not found in subscribed." );
        return 0;
    }

    ResultRange ButtonMapping::findSubscribedToButton(const NsSmartDeviceLinkRPCV2::ButtonName &btnName) const
    {
        return mButtonsMapping.equal_range(btnName);
    }

    /**
     * \brief Copy constructor
     */
    ButtonMapping::ButtonMapping(const ButtonMapping &)
    {
    }

    bool ButtonMapping::exist(const NsSmartDeviceLinkRPCV2::ButtonName& buttonName, Application* item)
    {
        ResultRange result = mButtonsMapping.equal_range(buttonName);
        for( ButtonMap::const_iterator it=result.first; it!=result.second; ++it )
        {
            if ( it->second->getAppID() == item->getAppID() )
            {
                return true;
            }
        }
        return false;
        /*ButtonMap::const_iterator it = mButtonsMapping.find(buttonName);
        if (it != mButtonsMapping.end())
        {
            if (it->second)
            {
                if (*(it->second) == *item)
                {
                    return true;
                }
            }
        }

        return false;*/
    }

    /**
     * \brief comparison operator
     * \param b1 button name 1
     * \param b2 button name 2
     * \return comparison result
     */
    bool Comparer::operator ()(const NsSmartDeviceLinkRPCV2::ButtonName &b1, const NsSmartDeviceLinkRPCV2::ButtonName &b2) const
    {
        return b1.get() < b2.get();
    }

}
