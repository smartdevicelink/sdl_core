#ifndef NSRPC2COMMUNICATION_UI_PERFORMINTERACTION_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMINTERACTION_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V1/InteractionMode.h"
#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V2/VrHelpItem.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class PerformInteraction : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      PerformInteraction(const PerformInteraction& c);
      PerformInteraction(void);
    
      PerformInteraction& operator =(const PerformInteraction&);
    
      virtual ~PerformInteraction(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_initialText(void);

      const std::vector< NsAppLinkRPC::TTSChunk>& get_initialPrompt(void);

      const NsAppLinkRPC::InteractionMode& get_interactionMode(void);

      const std::vector< unsigned int>& get_interactionChoiceSetIDList(void);

      const std::vector< NsAppLinkRPC::TTSChunk>* get_helpPrompt(void);
      const std::vector< NsAppLinkRPC::TTSChunk>* get_timeoutPrompt(void);
      const unsigned int* get_timeout(void);
      const std::vector< NsAppLinkRPCV2::VrHelpItem>* get_vrHelp(void);
      int get_appId(void);


// setters
/// initialText <= 500
      bool set_initialText(const std::string& initialText);

/// 1 <= size <= 100
      bool set_initialPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& initialPrompt);

      bool set_interactionMode(const NsAppLinkRPC::InteractionMode& interactionMode);

/// interactionChoiceSetIDList[] <= 2000000000 ; 1 <= size <= 100
      bool set_interactionChoiceSetIDList(const std::vector< unsigned int>& interactionChoiceSetIDList);

/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);

/// 5000 <= timeout <= 100000
      bool set_timeout(const unsigned int& timeout);

      void reset_timeout(void);

/// 1 <= size <= 100
      bool set_vrHelp(const std::vector< NsAppLinkRPCV2::VrHelpItem>& vrHelp);

      void reset_vrHelp(void);

      bool set_appId(int appId);


    private:

      friend class PerformInteractionMarshaller;

      std::string initialText;
      std::vector< NsAppLinkRPC::TTSChunk> initialPrompt;
      NsAppLinkRPC::InteractionMode interactionMode;
      std::vector< unsigned int> interactionChoiceSetIDList;
      std::vector< NsAppLinkRPC::TTSChunk>* helpPrompt;
      std::vector< NsAppLinkRPC::TTSChunk>* timeoutPrompt;
      unsigned int* timeout;
      std::vector< NsAppLinkRPCV2::VrHelpItem>* vrHelp;
      int appId;

    };
  }
}

#endif
