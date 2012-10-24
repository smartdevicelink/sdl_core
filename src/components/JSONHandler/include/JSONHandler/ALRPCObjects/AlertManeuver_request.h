#ifndef ALERTMANEUVER_REQUEST_INCLUDE
#define ALERTMANEUVER_REQUEST_INCLUDE

#include <vector>

#include "SoftButton.h"
#include "TTSChunk.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


class AlertManeuver_request : public ALRPCRequest
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

  std::vector<TTSChunk> ttsChunks;	//!<   [%s..%s] 
  std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
};

#endif
