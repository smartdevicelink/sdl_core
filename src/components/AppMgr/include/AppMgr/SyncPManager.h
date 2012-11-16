#ifndef SYNCPMANAGER_H
#define SYNCPMANAGER_H

#include <vector>
#include <string>

namespace NsAppManager
{

class SyncPManager
{
public:

    typedef std::vector<std::string> PData;

    SyncPManager();

    ~SyncPManager();

    void setPData(const PData& data);

    const PData& getPData() const;

private:

    SyncPManager(const SyncPManager&);

    PData mPData;

};

}

#endif // SYNCPMANAGER_H
