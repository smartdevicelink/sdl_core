#ifndef ALERT_REQUEST_INCLUDE
#define ALERT_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
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

  std::string* alertText1;	//!< (500)
  std::string* alertText2;	//!< (500)
  std::vector<TTSChunk>* ttsChunks;	//!<   [%s..%s] 
  unsigned int* duration;	//!<  (3000,10000)
  bool* playTone;
};

#endif
