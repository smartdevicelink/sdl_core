#ifndef PERFORMINTERACTION_REQUEST_INCLUDE
#define PERFORMINTERACTION_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "InteractionMode.h"
#include "TTSChunk.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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

private:

  friend class PerformInteraction_requestMarshaller;

  std::string initialText;	//!< (500)
  std::vector<TTSChunk> initialPrompt;	//!<   [%s..%s] 
  InteractionMode interactionMode;
  std::vector<unsigned int> interactionChoiceSetIDList;	//!<   [%s..%s]  (0,2000000000)
  std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 
  std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 
  unsigned int* timeout;	//!<  (5000,100000)
};

#endif
