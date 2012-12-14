#ifndef NSAPPLINKRPC_ALERT_REQUEST_INCLUDE
#define NSAPPLINKRPC_ALERT_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Shows an alert which typically consists of text-to-speech message and text on the display. At least either alertText1, alertText2 or TTSChunks need to be provided.

  class Alert_request : public ALRPCRequest
  {
  public:
  
    Alert_request(const Alert_request& c);
    Alert_request(void);
    
    virtual ~Alert_request(void);
  
    Alert_request& operator =(const Alert_request&);
  
    bool checkIntegrity(void);

    const std::string* get_alertText1(void) const;
    const std::string* get_alertText2(void) const;
    const std::vector<TTSChunk>* get_ttsChunks(void) const;
    const unsigned int* get_duration(void) const;
    const bool* get_playTone(void) const;

    void reset_alertText1(void);
    bool set_alertText1(const std::string& alertText1_);
    void reset_alertText2(void);
    bool set_alertText2(const std::string& alertText2_);
    void reset_ttsChunks(void);
    bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);
    void reset_duration(void);
    bool set_duration(unsigned int duration_);
    void reset_playTone(void);
    bool set_playTone(bool playTone_);

  private:
  
    friend class Alert_requestMarshaller;


///  The first line of the alert text field
      std::string* alertText1;	//!< (500)

///  The second line of the alert text field
      std::string* alertText2;	//!< (500)

/**
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* ttsChunks;	//!<   [%s..%s] 

/**
     Timeout in milliseconds.
     Typical timeouts are 3-5 seconds
     If omitted, timeout is set to 5s.
*/
      unsigned int* duration;	//!<  (3000,10000)

/**
     Defines if tone should be played. Tone is played before TTS.
     If omitted, no tone is played.
*/
      bool* playTone;
  };

}

#endif
