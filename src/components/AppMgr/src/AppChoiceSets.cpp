//
// Copyright (c) 2013 Ford Motor Company
//

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
    void AppChoiceSets::addItem(const unsigned int &choiceSetId, const ChoiceSetV1 &choiceSet)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding an interaction choice set item " << choiceSetId );
        ChoiceSetGeneric choiceSetGeneric;
        choiceSetGeneric.choiceSetV1 = choiceSet;
        mChoiceSets.insert(ChoiceSetItem(choiceSetId, choiceSetGeneric));
    }

    /**
     * \brief add an interaction choice set item to a mapping
     * \param choiceSetId interaction choice set id
     * \param choiceSet interaction choice set
     */
    void AppChoiceSets::addItem(const unsigned int &choiceSetId, const ChoiceSetV2 &choiceSet)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Adding an interaction choice set item " << choiceSetId );
        ChoiceSetGeneric choiceSetGeneric;
        choiceSetGeneric.choiceSetV2 = choiceSet;
        mChoiceSets.insert(ChoiceSetItem(choiceSetId, choiceSetGeneric));
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
     * \brief find a mapped choice set item
     * \param choiceSetId interaction choice set id
     * \return a mapped choice set item
     */
    const ChoiceSetGeneric* AppChoiceSets::findItem(const unsigned int &choiceSetId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for an interaction choice set item with id " << choiceSetId );
        ChoiceSetItems::iterator it = mChoiceSets.find(choiceSetId);
        if(it != mChoiceSets.end())
        {
            const ChoiceSetItem& item = *it;
            if(choiceSetId == item.first)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Found an interaction choice set item with id " << choiceSetId );
                return &item.second;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Interaction choice set item for id " << choiceSetId << " not found!" );
        return 0;
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

    /**
     * \brief ChoiceSetGeneric default constructor
     */
    ChoiceSetGeneric::ChoiceSetGeneric()
    {
    }

    /**
     * \brief ChoiceSetGeneric default copy constructor
     * \param src source
     */
    ChoiceSetGeneric::ChoiceSetGeneric(const ChoiceSetGeneric &src)
    {
        *this = src;
    }

    const ChoiceSetGeneric &ChoiceSetGeneric::operator =(const ChoiceSetGeneric& src)
    {
        choiceSetV1 = src.choiceSetV1;
        choiceSetV2 = src.choiceSetV2;
        return *this;
    }

    /**
     * \brief ChoiceSetGeneric default destructor
     */
    ChoiceSetGeneric::~ChoiceSetGeneric()
    {
        choiceSetV1.clear();
        choiceSetV2.clear();
    }

}
