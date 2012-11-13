#ifndef BUTTONCAPABILITIES_H
#define BUTTONCAPABILITIES_H

#include <vector>
#include "JSONHandler/ALRPCObjects/ButtonCapabilities.h"

namespace RPC2Communication
{
    namespace Buttons
    {
        class GetCapabilitiesResponse;
    }
}

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

/**
 * \brief ButtonCapabilitiesContainer acts as container of button capabilities
 */
class ButtonCapabilitiesContainer
{
public:

    /**
     * \brief Button capabilities container
     */
    typedef std::vector<AppLinkRPC::ButtonCapabilities> Capabilities;

    /**
     * \brief Default class constructor
     */
    ButtonCapabilitiesContainer();

    /**
     * \brief set button capabilities
     * \param caps button capabilities
     */
    void set(const std::vector<AppLinkRPC::ButtonCapabilities> &caps );

    /**
     * \brief get button capabilities
     * \return button capabilities
     */
    const Capabilities& get() const;
private:

    /**
     * \brief Default class copy constructor
     */
    ButtonCapabilitiesContainer(const ButtonCapabilitiesContainer&);

    Capabilities mButtonCapabilities;
    static log4cplus::Logger mLogger;
};

}

#endif // BUTTONCAPABILITIES_H
