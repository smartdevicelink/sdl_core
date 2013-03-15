//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETGLOBALPROPERTIES_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/VrHelpItem.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
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
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* get_helpPrompt(void);
      const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* get_timeoutPrompt(void);
      const std::string* get_vrHelpTitle(void);
      const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* get_vrHelp(void);
      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_helpPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& helpPrompt);

      void reset_helpPrompt(void);

/// 1 <= size <= 100
      bool set_timeoutPrompt(const std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>& timeoutPrompt);

      void reset_timeoutPrompt(void);

/// vrHelpTitle <= 500
      bool set_vrHelpTitle(const std::string& vrHelpTitle);

      void reset_vrHelpTitle(void);

/// 1 <= size <= 100
      bool set_vrHelp(const std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>& vrHelp);

      void reset_vrHelp(void);

      bool set_appId(int appId);


    private:

      friend class SetGlobalPropertiesMarshaller;

      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* helpPrompt;
      std::vector< NsSmartDeviceLinkRPCV2::TTSChunk>* timeoutPrompt;
      std::string* vrHelpTitle;
      std::vector< NsSmartDeviceLinkRPCV2::VrHelpItem>* vrHelp;
      int appId;

    };
  }
}

#endif
