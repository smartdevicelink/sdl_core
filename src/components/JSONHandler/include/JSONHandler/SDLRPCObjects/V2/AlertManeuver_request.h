//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ALERTMANEUVER_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ALERTMANEUVER_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "TTSChunk.h"
#include "JSONHandler/SDLRPCMessage.h"


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

  class AlertManeuver_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    AlertManeuver_request(const AlertManeuver_request& c);
    AlertManeuver_request(void);
    
    virtual ~AlertManeuver_request(void);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk>& get_ttsChunks(void) const;
    const std::vector<SoftButton>& get_softButtons(void) const;

    bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class AlertManeuver_requestMarshaller;


///  An array of text chunks of type TTSChunk. See TTSChunk
      std::vector<TTSChunk> ttsChunks;	//!<   [%s..%s] 

///  If omitted on supported displays, only the system defined "Close" SoftButton shall be displayed.
      std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
  };

}

#endif
