#include "AppMgr/AppChoiceSets.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    log4cplus::Logger AppChoiceSets::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppChoiceSets"));

    /**
     * \brief Default class constructor
     */
    AppChoiceSets::AppChoiceSets()
    {
    }

    /**
     * \brief Default class destructor
     */
    AppChoiceSets::~AppChoiceSets()
    {
        clear();
    }

    /**
     * \brief add an interaction choice set item to a mapping
     * \param choiceSetId interaction choice set id
     * \param choiceSet interaction choice set
     */
    void AppChoiceSets::addItem(const unsigned int &choiceSetId, const ChoiceSet &choiceSet)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding an interaction choice set item " << choiceSetId );
        mChoiceSets.insert(ChoiceSetItem(choiceSetId, choiceSet));
    }

    /**
     * \brief remove an interaction choice set from a mapping
     * \param choiceSetId interaction choice set id
     */
    void AppChoiceSets::removeItem(const unsigned int &choiceSetId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Deleting an interaction choice set item " << choiceSetId );
        mChoiceSets.erase(choiceSetId);
    }

    /**
     * \brief gets all interaction choice set items
     * \return interaction choice set items
     */
    ChoiceSetItems AppChoiceSets::getAllChoiceSets() const
    {
        return mChoiceSets;
    }

    /**
     * \brief get count of interaction choice sets
     * \return interaction choice sets count
     */
    size_t AppChoiceSets::size() const
    {
        return mChoiceSets.size();
    }

    /**
     * \brief cleans all the items
     */
    void AppChoiceSets::clear()
    {
        mChoiceSets.clear();
    }

    /**
     * \brief Copy constructor
     */
    AppChoiceSets::AppChoiceSets(const AppChoiceSets &)
    {
    }
}
