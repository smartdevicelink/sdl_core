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

struct Comparer {
    bool operator() (const AppLinkRPC::ButtonName &b1, const AppLinkRPC::ButtonName &b2) const;
};

typedef std::map<AppLinkRPC::ButtonName, RegistryItem*, Comparer> ButtonMap;
typedef std::pair<AppLinkRPC::ButtonName, RegistryItem*> ButtonMapItem;

class ButtonMapping
{
public:
    ButtonMapping();
    void addButton( const AppLinkRPC::ButtonName& buttonName, RegistryItem* app );
    void removeButton(const AppLinkRPC::ButtonName& buttonName);
    void removeItem( RegistryItem* app );
    RegistryItem *findRegistryItemSubscribedToButton(const AppLinkRPC::ButtonName &btnName) const;

private:
    ButtonMapping(const ButtonMapping&);

    ButtonMap    mButtonsMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // BUTTONMAPPING_H
