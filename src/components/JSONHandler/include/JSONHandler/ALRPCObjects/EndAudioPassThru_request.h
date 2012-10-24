#ifndef ENDAUDIOPASSTHRU_REQUEST_INCLUDE
#define ENDAUDIOPASSTHRU_REQUEST_INCLUDE


#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  When this request is invoked, the audio capture stops.

class EndAudioPassThru_request : public ALRPCRequest
{
public:

  EndAudioPassThru_request(const EndAudioPassThru_request& c);
  EndAudioPassThru_request(void);
  
  virtual ~EndAudioPassThru_request(void);

  bool checkIntegrity(void);



private:

  friend class EndAudioPassThru_requestMarshaller;

};

#endif
