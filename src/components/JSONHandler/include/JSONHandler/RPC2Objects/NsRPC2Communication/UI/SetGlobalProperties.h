#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V1/VrHelpItem.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
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
      const std::string* get_vrHelpTitle(void);
      const std::vector< NsAppLinkRPC::VrHelpItem>* get_vrHelp(void);
      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< NsAppLinkRPC::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);

/// vrHelpTitle <= 500
      bool set_vrHelpTitle(const std::string& vrHelpTitle);

      void reset_vrHelpTitle(void);

/// 1 <= size <= 100
      bool set_vrHelp(const std::vector< NsAppLinkRPC::VrHelpItem>& vrHelp);

      void reset_vrHelp(void);

      bool set_appId(int appId);


    private:

      friend class SetGlobalPropertiesMarshaller;

      std::vector< NsAppLinkRPC::TTSChunk>* helpPrompt;
      std::vector< NsAppLinkRPC::TTSChunk>* timeoutPrompt;
      std::string* vrHelpTitle;
      std::vector< NsAppLinkRPC::VrHelpItem>* vrHelp;
      int appId;

    };
  }
}

#endif
