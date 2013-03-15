//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SPEAK_RESPONSE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SPEAK_RESPONSE_INCLUDE

#include "JSONHandler/SDLRPCResponse.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{
  class Speak_response : public NsSmartDeviceLinkRPC::SDLRPCResponse
  {
  public:
    Speak_response(const Speak_response& c);
    Speak_response(void);

    virtual ~Speak_response(void);

    Speak_response& operator =(const Speak_response&);

    bool checkIntegrity(void);

  private:
    friend class Speak_responseMarshaller;
  };
}

#endif
