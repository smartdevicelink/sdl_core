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

#ifndef APPCHOICESETS_H
#define APPCHOICESETS_H

#include "JSONHandler/SDLRPCObjects/V1/Choice.h"
#include "JSONHandler/SDLRPCObjects/V2/Choice.h"
#include <vector>
#include <string>
#include <map>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{
    /**
     * \brief interaction choice sets protocol v1
     */
    typedef std::vector<NsSmartDeviceLinkRPC::Choice> ChoiceSetV1;

    /**
     * \brief interaction choice sets protocol v2
     */
    typedef std::vector<NsSmartDeviceLinkRPCV2::Choice> ChoiceSetV2;

    /**
     * \brief interaction choice sets
     */
    struct ChoiceSetGeneric
    {
        ChoiceSetV1 choiceSetV1;
        ChoiceSetV2 choiceSetV2;
        ChoiceSetGeneric();
        ChoiceSetGeneric(const ChoiceSetGeneric&src);
        const ChoiceSetGeneric& operator= (const ChoiceSetGeneric&);
        ~ChoiceSetGeneric();
    };

    /**
     * \brief mapping of interaction choice set id to choice set item vector
     */
    typedef std::pair<unsigned int, ChoiceSetGeneric> ChoiceSetItem;

    /**
     * \brief mapping of  interaction choice set id to choice set item vector
     */
    typedef std::map<unsigned int, ChoiceSetGeneric> ChoiceSetItems;

    /**
     * \brief AppChoiceSets acts as a mapping between interaction choice set ids and respective choice sets
     */
    class AppChoiceSets
    {
    public:

        /**
         * \brief Default class constructor
         */
        AppChoiceSets();

        /**
         * \brief Default class destructor
         */
        ~AppChoiceSets();

        /**
         * \brief add an interaction choice set item to a mapping
         * \param choiceSetId interaction choice set id
         * \param choiceSet interaction choice set
         */
        void addItem(const unsigned int& choiceSetId, const ChoiceSetV1& choiceSet);

        /**
         * \brief add an interaction choice set item to a mapping
         * \param choiceSetId interaction choice set id
         * \param choiceSet interaction choice set
         */
        void addItem(const unsigned int& choiceSetId, const ChoiceSetV2& choiceSet);

        /**
         * \brief remove an interaction choice set from a mapping
         * \param choiceSetId interaction choice set id
         */
        void removeItem(const unsigned int& choiceSetId);

        /**
         * \brief find a mapped choice set item
         * \param choiceSetId interaction choice set id
         * \return a mapped choice set item
         */
        const ChoiceSetGeneric *findItem(const unsigned int& choiceSetId);

        /**
         * \brief gets all interaction choice set items
         * \return interaction choice set items
         */
        ChoiceSetItems getAllChoiceSets() const;

        /**
         * \brief get count of interaction choice sets
         * \return interaction choice sets count
         */
        size_t size() const;

        /**
         * \brief cleans all the items
         */
        void clear();

    private:

        /**
         * \brief Copy constructor
         */
        AppChoiceSets(const AppChoiceSets&);

        ChoiceSetItems mChoiceSets;
        static log4cplus::Logger mLogger;
    };

}

#endif // APPCHOICESETS_H
