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

#ifndef REGISTRYITEM_H_
#define REGISTRYITEM_H_

#include "Application.h"
#include "AppPolicy.h"
#include <set>
#include <string>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief RegistryItem acts as a wrapper for registered application in AppMgrRegistry
     */
    class RegistryItem
    {
    public:
        typedef std::set<AppPolicy*> Policies;

        /**
         * \brief Class constructor
         * \param app application to register
         */
        RegistryItem(Application *app );

        /**
         * \brief Default class destructor
         */
        virtual ~RegistryItem( );

        /**
         * \brief operator ==
         * \param item the item to compare with
         * \return comparison result
         */
        bool operator==(const RegistryItem& item) const;

        /**
         * \brief register application policy
         * \param hash policy text representation
         * \return app policy instance
         */
        const AppPolicy* registerPolicy( const std::string& hash );

        /**
         * \brief unregister application policy
         * \param policy policy to be unregistered
         */
        void unregisterPolicy( AppPolicy* policy );

        /**
         * \brief get associated application
         * \return asssociated application
         */
        Application *getApplication() const;

        /**
         * \brief comparison operator
         * \param item2 item to compare with
         * \return comparison result
         */
        bool operator<(const RegistryItem& item2) const;

        /**
         * \brief get application policies
         * \param app application to get policies for
         * \return app policy set
         */
        Policies getApplicationPolicies(const Application* app) const;

        /**
         * \brief get application policies
         * \param app name of an application to get policies for
         * \return app policy set
         */
        Policies getApplicationPolicies(const std::string& app) const;

    private:

        /**
         * \brief Copy constructor
         */
        RegistryItem( const RegistryItem& );

        Policies mAppPolicies;
        Application* mApplication;
        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif /* REGISTRYITEM_H_ */
