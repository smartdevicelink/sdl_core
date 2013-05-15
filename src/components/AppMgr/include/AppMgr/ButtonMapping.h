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

#ifndef BUTTONMAPPING_H
#define BUTTONMAPPING_H

#include <map>
#include "JSONHandler/SDLRPCObjects/V1/ButtonName.h"
#include "JSONHandler/SDLRPCObjects/V2/ButtonName.h"
#include "AppMgr/Application.h"

namespace NsAppManager
{

    class Application;

    /**
     * \brief Comparer acts as a comparer that allows to create std::map with NsSmartDeviceLinkRPC::ButtonName as a key
     */
    struct Comparer {
        bool operator() (const NsSmartDeviceLinkRPCV2::ButtonName &b1, const NsSmartDeviceLinkRPCV2::ButtonName &b2) const;
    };

    /**
     * \brief a button_name-registered-app map
     */
    typedef std::multimap<NsSmartDeviceLinkRPCV2::ButtonName, Application*, Comparer> ButtonMap;

    /**
     * \brief a button_name-registered-app map item
     */
    typedef std::pair<NsSmartDeviceLinkRPCV2::ButtonName, Application*> ButtonMapItem;

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
        void addButton( const NsSmartDeviceLinkRPCV2::ButtonName& buttonName, Application* app );

        /**
         * \brief remove a button from a mapping
         * \param buttonName button name
         */
        void removeButton(const NsSmartDeviceLinkRPCV2::ButtonName& buttonName, Application * item);

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
        Application *findRegistryItemSubscribedToButton(const NsSmartDeviceLinkRPCV2::ButtonName &btnName) const;

        /**
         * \brief Find all apps subscribed to button
         * \param btnName Button name
         * \return @ResultRange Results range
         */
        ResultRange findSubscribedToButton(const NsSmartDeviceLinkRPCV2::ButtonName &btnName) const;

        bool exist(const NsSmartDeviceLinkRPCV2::ButtonName& buttonName, Application* item);
    private:

        /**
         * \brief Copy constructor
         */
        ButtonMapping(const ButtonMapping&);

        ButtonMap    mButtonsMapping;
        static log4cxx::LoggerPtr logger_;
    };

}

#endif // BUTTONMAPPING_H
