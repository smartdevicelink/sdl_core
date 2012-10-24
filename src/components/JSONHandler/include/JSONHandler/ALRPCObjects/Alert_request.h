#ifndef ALERT_REQUEST_INCLUDE
#define ALERT_REQUEST_INCLUDE

#include <vector>
#include <string>

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

  std::string* alertText1;	//!< (500)
  std::string* alertText2;	//!< (500)
  std::string* alertText3;	//!< (500)
  std::vector<TTSChunk>* ttsChunks;	//!<   [%s..%s] 
  unsigned int* duration;	//!<  (3000,10000)
  bool* playTone;
  std::vector<SoftButton>* softButtons;	//!<   [%s..%s] 
};

#endif
