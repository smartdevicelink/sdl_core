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
    typedef std::vector<std::string> RawData;

    SyncPManager();

    ~SyncPManager();

    void setPData(const PData& data);

    const PData& getPData() const;

    void setRawData(const RawData& data);

    RawData getRawData() const;

private:

    SyncPManager(const SyncPManager&);

    PData mPData;

};

}

#endif // SYNCPMANAGER_H
