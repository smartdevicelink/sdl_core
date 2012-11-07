#ifndef RPC2COMMUNICATION_UI_PERFORMINTERACTION_INCLUDE
#define RPC2COMMUNICATION_UI_PERFORMINTERACTION_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/InteractionMode.h"
#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class PerformInteraction : public ::RPC2Communication::RPC2Request
    {
    public:
    
      PerformInteraction(const PerformInteraction& c);
      PerformInteraction(void);
    
      PerformInteraction& operator =(const PerformInteraction&);
    
      virtual ~PerformInteraction(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_initialText(void);

      const std::vector< ::AppLinkRPC::TTSChunk>& get_initialPrompt(void);

      const ::AppLinkRPC::InteractionMode& get_interactionMode(void);

      const std::vector< unsigned int>& get_interactionChoiceSetIDList(void);

  const     std::vector< ::AppLinkRPC::TTSChunk>* get_helpPrompt(void);
  const     std::vector< ::AppLinkRPC::TTSChunk>* get_timeoutPrompt(void);
  const     unsigned int* get_timeout(void);

// setters
/// initialText <= 500
      bool set_initialText(const std::string& initialText);

/// 1 <= size <= 100
      bool set_initialPrompt(const std::vector< ::AppLinkRPC::TTSChunk>& initialPrompt);

      bool set_interactionMode(const ::AppLinkRPC::InteractionMode& interactionMode);

/// interactionChoiceSetIDList[] <= 2000000000 ; 1 <= size <= 100
      bool set_interactionChoiceSetIDList(const std::vector< unsigned int>& interactionChoiceSetIDList);

/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< ::AppLinkRPC::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< ::AppLinkRPC::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);

/// 5000 <= timeout <= 100000
      bool set_timeout(const unsigned int& timeout);

      void reset_timeout(void);


    private:

      friend class PerformInteractionMarshaller;

      std::string initialText;
      std::vector< ::AppLinkRPC::TTSChunk> initialPrompt;
      ::AppLinkRPC::InteractionMode interactionMode;
      std::vector< unsigned int> interactionChoiceSetIDList;
      std::vector< ::AppLinkRPC::TTSChunk>* helpPrompt;
      std::vector< ::AppLinkRPC::TTSChunk>* timeoutPrompt;
      unsigned int* timeout;

    };
  }
}

#endif
