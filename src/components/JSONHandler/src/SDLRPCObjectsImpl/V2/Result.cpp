//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/Result.h"
#include "ResetGlobalProperties_responseMarshaller.h"

// TODO(AK): remove this after new code generation
#include "../include/JSONHandler/SDLRPCObjects/V1/Result.h"

namespace
{
    const int PROTOCOL_VERSION = 1;
}

using namespace NsSmartDeviceLinkRPCV2;

Result::Result() : mInternal(INVALID_ENUM)
{}

Result::Result(ResultInternal e) : mInternal(e)
{}

Result::Result(const Result& result)
{
    mInternal = result.mInternal;
}

Result::ResultInternal Result::get(void) const
{
    return mInternal;
}
void Result::set(ResultInternal e)
{
    mInternal = e;
}

// TODO(AK): remove this after new code generation
Result::operator NsSmartDeviceLinkRPC::Result()
{
    return NsSmartDeviceLinkRPC::Result(
               static_cast<NsSmartDeviceLinkRPC::Result::ResultInternal>(mInternal));
}

// TODO(AK): remove this after new code generation
Result& Result::operator= (const Result& result)
{
    mInternal = result.mInternal;
    return *this;
}
