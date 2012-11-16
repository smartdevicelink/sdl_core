#ifndef SYNCPMANAGER_H
#define SYNCPMANAGER_H

#include <vector>
#include <string>
#include "LoggerHelper.hpp"

namespace log4cplus
{
    class Logger;
}

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

    /**
     * \brief serialize a string vector to the text file
     * \param fileName name of the file to serialize to
     * \param value a value to serialize
     * \return success of an operation - true or false
     */
    bool serializeToFile(const std::string& fileName, const PData &value) const;

    /**
     * \brief deserialize a string vector from the text file
     * \param fileName name of the file to deserialize from
     * \param value a value to deserialize
     * \return success of an operation - true or false
     */
    bool deserializeFromFile(const std::string &fileName, PData &value);

    PData mPData;
    static log4cplus::Logger mLogger;
};

}

#endif // SYNCPMANAGER_H
