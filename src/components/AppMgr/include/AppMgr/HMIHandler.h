#ifndef HMIHANDLER_H
#define HMIHANDLER_H

namespace NsAppManager
{

class HMIHandler
{
    static const HMIHandler& getInstance();
private:
    HMIHandler();
    HMIHandler(const HMIHandler&);
};

}

#endif // HMIHANDLER_H
