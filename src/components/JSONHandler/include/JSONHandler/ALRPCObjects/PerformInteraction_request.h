#ifndef PERFORMINTERACTION_REQUEST_INCLUDE
#define PERFORMINTERACTION_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "InteractionMode.h"
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


///  Triggers an interaction (e.g. "Permit GPS?" - Yes, no, Always Allow).

class PerformInteraction_request : public ALRPCRequest
{
public:

  PerformInteraction_request(const PerformInteraction_request& c);
  PerformInteraction_request(void);
  
  virtual ~PerformInteraction_request(void);

  PerformInteraction_request& operator =(const PerformInteraction_request&);

  bool checkIntegrity(void);

  const std::string& get_initialText(void) const;
  const std::vector<TTSChunk>& get_initialPrompt(void) const;
  const InteractionMode& get_interactionMode(void) const;
  const std::vector<unsigned int>& get_interactionChoiceSetIDList(void) const;
  const std::vector<TTSChunk>* get_helpPrompt(void) const;
  const std::vector<TTSChunk>* get_timeoutPrompt(void) const;
  const unsigned int* get_timeout(void) const;
  const std::string* get_vrHelpTitle(void) const;
  const std::vector<VrHelpItem>* get_vrHelp(void) const;

  bool set_initialText(const std::string& initialText_);
  bool set_initialPrompt(const std::vector<TTSChunk>& initialPrompt_);
  bool set_interactionMode(const InteractionMode& interactionMode_);
  bool set_interactionChoiceSetIDList(const std::vector<unsigned int>& interactionChoiceSetIDList_);
  void reset_helpPrompt(void);
  bool set_helpPrompt(const std::vector<TTSChunk>& helpPrompt_);
  void reset_timeoutPrompt(void);
  bool set_timeoutPrompt(const std::vector<TTSChunk>& timeoutPrompt_);
  void reset_timeout(void);
  bool set_timeout(unsigned int timeout_);
  void reset_vrHelpTitle(void);
  bool set_vrHelpTitle(const std::string& vrHelpTitle_);
  void reset_vrHelp(void);
  bool set_vrHelp(const std::vector<VrHelpItem>& vrHelp_);

private:

  friend class PerformInteraction_requestMarshaller;

  std::string initialText;	//!< (500)
  std::vector<TTSChunk> initialPrompt;	//!<   [%s..%s] 
  InteractionMode interactionMode;
  std::vector<unsigned int> interactionChoiceSetIDList;	//!<   [%s..%s]  (0,2000000000)
  std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 
  std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 
  unsigned int* timeout;	//!<  (5000,100000)
  std::string* vrHelpTitle;	//!< (500)
  std::vector<VrHelpItem>* vrHelp;	//!<   [%s..%s] 
};

#endif
