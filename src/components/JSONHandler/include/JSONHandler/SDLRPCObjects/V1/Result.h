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

#ifndef NSSMARTDEVICELINKKRPC_RESULT_INCLUDE
#define NSSMARTDEVICELINKKRPC_RESULT_INCLUDE

namespace NsSmartDeviceLinkRPCV2
{
    class Result;
}

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    RC
*/

namespace NsSmartDeviceLinkRPC
{

    class Result
    {
    public:
        enum ResultInternal
        {
            INVALID_ENUM = -1,

            ///  The request succeeded
            SUCCESS = 0,

            /**
                 The data sent is invalid. For example
                 Invalid Json syntax
                 Parameters out of bounds (number or enum range)
                 Mandatory parameters not provided
                 Parameter provided with wrong type
                 Invalid characters
                 Empty string
            */
            INVALID_DATA = 1,

            ///  The request is not supported by Sync
            UNSUPPORTED_REQUEST = 2,

            ///  The system could not process the request because the necessary memory couldn't be allocated
            OUT_OF_MEMORY = 3,

            ///  There are too many requests pending (means, that the response has not been delivered, yet).
            TOO_MANY_PENDING_REQUESTS = 4,

            /**
                 One of the provided IDs is not valid. For example
                 This applies to CorrelationID, SubscriptionID [@TODO if SubscriptionID is used], CommandID, MenuID, [@TODO: missed one?]
            */
            INVALID_ID = 5,

            ///  There was a conflict with an registered name (application or menu item) or vr command
            DUPLICATE_NAME = 6,

            ///  There are already too many registered applications
            TOO_MANY_APPLICATIONS = 7,

            ///  RegisterApplication has been called again, after a RegisterApplication was successful before.
            APPLICATION_REGISTERED_ALREADY = 8,

            ///  Sync doesn't support the protocol that is requested by the mobile application
            UNSUPPORTED_VERSION = 9,

            /**
                 The requested language is currently not supported.
                 Might be because of a mismatch of the currently active language on Sync and the requested language
            */
            WRONG_LANGUAGE = 10,

            ///  An command can not be executed because no application has been registered with RegisterApplication.
            APPLICATION_NOT_REGISTERED = 11,

            /**
                 The data may not be changed, because it is currently in use.
                 For example when trying to delete a command set that is currently involved in an interaction.
            */
            IN_USE = 12,

            ///  The item to subscribe to is already subscribed to.
            SUBSCRIBED_ALREADY = 13,

            /**
                 The requested command was rejected, e.g. because mobile app is in background and cannot perform any HMI commands.
                 Or an HMI command (e.g. Speak) is rejected because a higher priority HMI command (e.g. Alert) is playing.
            */
            REJECTED = 14,

            /**
                 A command was aborted, for example due to user interaction (e.g. user pressed button).
                 Or an HMI command (e.g. Speak) is aborted because a higher priority HMI command (e.g. Alert) was requested.
            */
            ABORTED = 15,

            /**
                 A command was ignored, because the intended result is already in effect.
                 For example, SetMediaClockTimer was used to pause the media clock although the clock is paused already.
                 NOTE: potentially replaces SUBSCRIBED_ALREADY
            */
            IGNORED = 16,

            /**
                 A button that was requested for subscription is not supported under the current system.
                 NOTE: could become a more generic UNSUPPORTED_RESOURCE by merging with VEHIVLE_DATA_NOT_AVAILABLE.
            */
            UNSUPPORTED_BUTTON = 17,

            ///  A specified file could not be found on Sync.
            FILE_NOT_FOUND = 18,

            ///  Provided data is valid but something went wrong in the lower layers.
            GENERIC_ERROR = 19
        };

        Result();
        Result(ResultInternal e);
        Result(const Result& result);

        ResultInternal get(void) const;
        void set(ResultInternal e);

        // TODO(AK): remove this after new code generation
        operator NsSmartDeviceLinkRPCV2::Result();
        Result& operator= (const Result& result);

    private:
        ResultInternal mInternal;
        friend class ResultMarshaller;
    };

}

#endif
