#ifndef RPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE
#define RPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

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

    class SetGlobalProperties : public ::RPC2Communication::RPC2Request
    {
    public:
    
      SetGlobalProperties(const SetGlobalProperties& c);
      SetGlobalProperties(void);
    
      SetGlobalProperties& operator =(const SetGlobalProperties&);
    
      virtual ~SetGlobalProperties(void);
    
      bool checkIntegrity(void);
    
// getters
  const     std::vector< ::AppLinkRPC::TTSChunk>* get_helpPrompt(void);
  const     std::vector< ::AppLinkRPC::TTSChunk>* get_timeoutPrompt(void);

// setters
/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< ::AppLinkRPC::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< ::AppLinkRPC::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);


    private:

      friend class SetGlobalPropertiesMarshaller;

      std::vector< ::AppLinkRPC::TTSChunk>* helpPrompt;
      std::vector< ::AppLinkRPC::TTSChunk>* timeoutPrompt;

    };
  }
}

#endif
