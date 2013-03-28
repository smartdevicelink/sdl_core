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

#ifndef NSSMARTDEVICELINKKRPCV2_READDID_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_READDID_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "VehicleDataResultCode.h"
#include "JSONHandler/SDLRPCResponse.h"

/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPCV2
{
    class ReadDID_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
    {
    public:
        ReadDID_response(const ReadDID_response& c);
        ReadDID_response(void);

        virtual ~ReadDID_response(void);

        ReadDID_response& operator =(const ReadDID_response&);

        bool checkIntegrity(void);

        const std::vector<VehicleDataResultCode>* get_dataResult(void) const;
        const std::vector<std::string>* get_data(void) const;

        void reset_dataResult(void);
        bool set_dataResult(const std::vector<VehicleDataResultCode>& dataResult_);
        void reset_data(void);
        bool set_data(const std::vector<std::string>& data_);

    private:
        friend class ReadDID_responseMarshaller;

        ///  Permission status of data element.
        std::vector<VehicleDataResultCode>* dataResult;   //!<   [%s..%s]

        ///  Raw DID-based data returned for requested element.
        std::vector<std::string>* data;   //!<   [%s..%s] (5000)
    };
}

#endif
