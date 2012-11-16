#ifndef SYNCPMANAGER_H
#define SYNCPMANAGER_H

#include <vector>
#include <string>

namespace NsAppManager
{

/**
 * \brief SyncPManager acts as a handler of PData for EncodedSyncPData and OnEncodedSyncPData
 */
class SyncPManager
{
public:

    /**
     * \brief vector of base64-encoded strings
     */
    typedef std::vector<std::string> PData;

    /**
     * \brief vector of raw strings
     */
    typedef std::vector<std::string> RawData;

    /**
     * \brief Default class constructor
     */
    SyncPManager();

    /**
     * \brief Default class destructor
     */
    ~SyncPManager();

    /**
     * \brief set base64-encoded PData
     * \param data vector of strings of base64-encoded information
     */
    void setPData(const PData& data);

    /**
     * \brief get base64-encoded PData
     * \return vector of strings of base64-encoded information
     */
    const PData& getPData() const;

    /**
     * \brief set raw string data
     * \param data vector of strings
     */
    void setRawData(const RawData& data);

    /**
     * \brief set raw string data
     * \return vector of strings
     */
    RawData getRawData() const;

private:

    /**
     * \brief Copy constructor
     */
    SyncPManager(const SyncPManager&);

    PData mPData;

};

}

#endif // SYNCPMANAGER_H
