#ifndef SETGLOBALPROPERTIES_REQUEST_INCLUDE
#define SETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk.h"
#include "VrHelpItem.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Allows setting global properties.

class SetGlobalProperties_request : public ALRPCRequest
{
public:

  SetGlobalProperties_request(const SetGlobalProperties_request& c);
  SetGlobalProperties_request(void);
  
  virtual ~SetGlobalProperties_request(void);

  SetGlobalProperties_request& operator =(const SetGlobalProperties_request&);

  bool checkIntegrity(void);

  const std::vector<TTSChunk>* get_helpPrompt(void) const;
  const std::vector<TTSChunk>* get_timeoutPrompt(void) const;
  const std::string* get_vrHelpTitle(void) const;
  const std::vector<VrHelpItem>* get_vrHelp(void) const;

  void reset_helpPrompt(void);
  bool set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_);
  void reset_timeoutPrompt(void);
  bool set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_);
  void reset_vrHelpTitle(void);
  bool set_vrHelpTitle(const std::string& vrHelpTitle_);
  void reset_vrHelp(void);
  bool set_vrHelp(const std::vector<VrHelpItem>& vrHelp_);

private:

  friend class SetGlobalProperties_requestMarshaller;

  std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 
  std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 
  std::string* vrHelpTitle;	//!< (500)
  std::vector<VrHelpItem>* vrHelp;	//!<   [%s..%s] 
};

#endif
