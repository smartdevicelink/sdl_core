#ifndef APPCHOICESETS_H
#define APPCHOICESETS_H

#include "JSONHandler/ALRPCObjects/V1/Choice.h"
#include "JSONHandler/ALRPCObjects/V2/Choice.h"
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
     * \brief interaction choice sets
     */
    union ChoiceSetGeneric
    {
        std::vector<NsAppLinkRPC::Choice> choiceSetV1;
        std::vector<NsAppLinkRPCV2::Choice> choiceSetV2;
        ChoiceSetGeneric();
        ChoiceSetGeneric(const ChoiceSetGeneric&);
        ~ChoiceSetGeneric();
    };

    /**
     * \brief interaction choice sets protocol v1
     */
    typedef std::vector<NsAppLinkRPC::Choice> ChoiceSetV1;

    /**
     * \brief interaction choice sets protocol v2
     */
    typedef std::vector<NsAppLinkRPCV2::Choice> ChoiceSetV2;

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
