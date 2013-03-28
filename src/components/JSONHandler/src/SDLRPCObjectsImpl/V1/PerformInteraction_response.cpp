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

#include "../include/JSONHandler/SDLRPCObjects/V1/PerformInteraction_response.h"
#include "PerformInteraction_responseMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "TriggerSourceMarshaller.h"

namespace
{
    const int PROTOCOL_VERSION = 1;
}

/*
  interface Ford Sync RAPI
  version   1.2
  date      2011-05-17
  generated at  Thu Jan 24 06:36:21 2013
  source stamp  Thu Jan 24 06:35:34 2013
  author    RC
*/

using namespace NsSmartDeviceLinkRPC;
PerformInteraction_response& PerformInteraction_response::operator =(const PerformInteraction_response& c)
{
    success = c.success;
    resultCode = c.resultCode;
    info = c.info ? new std::string(c.info[0]) : 0;
    choiceID = c.choiceID ? new unsigned int(c.choiceID[0]) : 0;
    triggerSource = c.triggerSource ? new TriggerSource(c.triggerSource[0]) : 0;

    return *this;
}

PerformInteraction_response::~PerformInteraction_response(void)
{
    if (choiceID)
    {
        delete choiceID;
    }
    if (triggerSource)
    {
        delete triggerSource;
    }
}

PerformInteraction_response::PerformInteraction_response(const PerformInteraction_response& c)
{
    *this = c;
}

bool PerformInteraction_response::checkIntegrity(void)
{
    return PerformInteraction_responseMarshaller::checkIntegrity(*this);
}

PerformInteraction_response::PerformInteraction_response(void)
    : SDLRPCResponse(PROTOCOL_VERSION, Marshaller::METHOD_PERFORMINTERACTION_RESPONSE),
    choiceID(0),
    triggerSource(0)
{}

bool PerformInteraction_response::set_choiceID(unsigned int choiceID_)
{
    if (choiceID_ > 2000000000)
    {
        return false;
    }
    delete choiceID;
    choiceID = 0;

    choiceID = new unsigned int(choiceID_);
    return true;
}

void PerformInteraction_response::reset_choiceID(void)
{
    if (choiceID)
    {
        delete choiceID;
    }
    choiceID = 0;
}

bool PerformInteraction_response::set_triggerSource(const TriggerSource& triggerSource_)
{
    if (!TriggerSourceMarshaller::checkIntegrityConst(triggerSource_))
    {
        return false;
    }
    delete triggerSource;
    triggerSource = 0;

    triggerSource = new TriggerSource(triggerSource_);
    return true;
}

void PerformInteraction_response::reset_triggerSource(void)
{
    if (triggerSource)
    {
        delete triggerSource;
    }
    triggerSource = 0;
}

const unsigned int* PerformInteraction_response::get_choiceID(void) const
{
    return choiceID;
}

const TriggerSource* PerformInteraction_response::get_triggerSource(void) const
{
    return triggerSource;
}

