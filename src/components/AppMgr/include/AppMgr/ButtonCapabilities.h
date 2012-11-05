#ifndef BUTTONCAPABILITIES_H
#define BUTTONCAPABILITIES_H

#include <vector>
#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"

namespace RPC2Communication
{
    class GetCapabilitiesResponse;
}

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

class ButtonCapabilitiesContainer
{
public:
    typedef std::vector<ButtonCapabilities> Capabilities;

    ButtonCapabilitiesContainer();
    void set( RPC2Communication::GetCapabilitiesResponse* msg );
    const Capabilities& get() const;
private:
    ButtonCapabilitiesContainer(const ButtonCapabilitiesContainer&);

    Capabilities mButtonCapabilities;
    static log4cplus::Logger mLogger;
};

}

#endif // BUTTONCAPABILITIES_H
