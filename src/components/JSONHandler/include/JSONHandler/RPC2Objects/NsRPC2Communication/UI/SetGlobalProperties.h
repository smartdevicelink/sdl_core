#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/TTSChunk.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SetGlobalProperties : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SetGlobalProperties(const SetGlobalProperties& c);
      SetGlobalProperties(void);
    
      SetGlobalProperties& operator =(const SetGlobalProperties&);
    
      virtual ~SetGlobalProperties(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPC::TTSChunk>* get_helpPrompt(void);
      const std::vector< NsAppLinkRPC::TTSChunk>* get_timeoutPrompt(void);

// setters
/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);


    private:

      friend class SetGlobalPropertiesMarshaller;

      std::vector< NsAppLinkRPC::TTSChunk>* helpPrompt;
      std::vector< NsAppLinkRPC::TTSChunk>* timeoutPrompt;

    };
  }
}

#endif
