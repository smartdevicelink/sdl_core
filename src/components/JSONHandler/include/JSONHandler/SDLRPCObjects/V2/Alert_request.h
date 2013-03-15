//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_ALERT_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ALERT_REQUEST_INCLUDE

#include <vector>
#include <string>

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

///  Shows an alert which typically consists of text-to-speech message and text on the display. At least either alertText1, alertText2 or TTSChunks need to be provided.

  class Alert_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    Alert_request(const Alert_request& c);
    Alert_request(void);
    
    virtual ~Alert_request(void);
  
    Alert_request& operator =(const Alert_request&);
  
    bool checkIntegrity(void);

    const std::string* get_alertText1(void) const;
    const std::string* get_alertText2(void) const;
    const std::string* get_alertText3(void) const;
    const std::vector<TTSChunk>* get_ttsChunks(void) const;
    const unsigned int* get_duration(void) const;
    const bool* get_playTone(void) const;
    const std::vector<SoftButton>* get_softButtons(void) const;

    void reset_alertText1(void);
    bool set_alertText1(const std::string& alertText1_);
    void reset_alertText2(void);
    bool set_alertText2(const std::string& alertText2_);
    void reset_alertText3(void);
    bool set_alertText3(const std::string& alertText3_);
    void reset_ttsChunks(void);
    bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);
    void reset_duration(void);
    bool set_duration(unsigned int duration_);
    void reset_playTone(void);
    bool set_playTone(bool playTone_);
    void reset_softButtons(void);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class Alert_requestMarshaller;


///  The first line of the alert text field
      std::string* alertText1;	//!< (500)

///  The second line of the alert text field
      std::string* alertText2;	//!< (500)

///  The optional third line of the alert text field
      std::string* alertText3;	//!< (500)

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

/**
     App defined SoftButtons.
     If omitted on supported displays, the displayed alert shall not have any SoftButtons.
*/
      std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
  };

}

#endif
