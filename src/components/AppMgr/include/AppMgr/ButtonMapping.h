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
    bool operator() (const ButtonName &b1, const ButtonName &b2) const;
};

typedef std::map<ButtonName, RegistryItem*, Comparer> ButtonMap;
typedef std::pair<ButtonName, RegistryItem*> ButtonMapItem;

class ButtonMapping
{
public:
    void addButton( const ButtonName& buttonName, RegistryItem* app );
    void removeButton(const ButtonName& buttonName);
    void removeItem( RegistryItem* app );

private:
    ButtonMapping(const ButtonMapping&);

    ButtonMap    mButtonsMapping;
    static log4cplus::Logger mLogger;
};

}

#endif // BUTTONMAPPING_H
