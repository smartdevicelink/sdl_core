#ifndef NSAPPLINKRPC_ALERTMANEUVER_REQUEST_INCLUDE
#define NSAPPLINKRPC_ALERTMANEUVER_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "TTSChunk_v2.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class AlertManeuver_request : public ALRPCRequest
  {
  public:
  
    AlertManeuver_request(const AlertManeuver_request& c);
    AlertManeuver_request(void);
    
    virtual ~AlertManeuver_request(void);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk_v2>& get_ttsChunks(void) const;
    const std::vector<SoftButton>& get_softButtons(void) const;

    bool set_ttsChunks(const std::vector<TTSChunk_v2>& ttsChunks_);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class AlertManeuver_requestMarshaller;


///  An array of text chunks of type TTSChunk. See TTSChunk
      std::vector<TTSChunk_v2> ttsChunks;	//!<   [%s..%s] 

///  If omitted on supported displays, only the system defined "Close" SoftButton shall be displayed.
      std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
  };

}

#endif
