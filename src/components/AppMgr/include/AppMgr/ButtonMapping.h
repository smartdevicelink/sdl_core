#ifndef BUTTONMAPPING_H
#define BUTTONMAPPING_H

#include <map>
#include "JSONHandler/ALRPCObjects/ButtonName.h"

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

class RegistryItem;

/**
 * \brief Comparer acts as a comparer that allows to create std::map with AppLinkRPC::ButtonName as a key
 */
struct Comparer {
    bool operator() (const AppLinkRPC::ButtonName &b1, const AppLinkRPC::ButtonName &b2) const;
};

/**
 * \brief a button_name-registered-app map
 */
typedef std::map<AppLinkRPC::ButtonName, RegistryItem*, Comparer> ButtonMap;

/**
 * \brief a button_name-registered-app map item
 */
typedef std::pair<AppLinkRPC::ButtonName, RegistryItem*> ButtonMapItem;

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
     * \brief add a button to a mapping
     * \param buttonName button name
     * \param app application to map a button to
     */
    void addButton( const AppLinkRPC::ButtonName& buttonName, RegistryItem* app );

    /**
     * \brief remove a button from a mapping
     * \param buttonName button name
     */
    void removeButton(const AppLinkRPC::ButtonName& buttonName);

    /**
     * \brief remove an application from a mapping
     * \param app application to remove all associated buttons from mapping
     */
    void removeItem( RegistryItem* app );

    /**
     * \brief find a registry item subscribed to button
     * \param btnName button name
     * \return RegistryItem instance
     */
    RegistryItem *findRegistryItemSubscribedToButton(const AppLinkRPC::ButtonName &btnName) const;

private:

    /**
     * \brief Copy constructor
     */
    ButtonMapping(const ButtonMapping&);

    ButtonMap    mButtonsMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // BUTTONMAPPING_H
