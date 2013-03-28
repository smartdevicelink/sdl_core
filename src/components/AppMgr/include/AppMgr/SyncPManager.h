//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

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
    void setPData(const PData& data, const std::string &appName, const int &methodId);

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
