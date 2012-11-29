#ifndef NSAPPLINKRPC_SETGLOBALPROPERTIES_V2_REQUEST_INCLUDE
#define NSAPPLINKRPC_SETGLOBALPROPERTIES_V2_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk_v2.h"
#include "VrHelpItem.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Allows setting global properties.

  class SetGlobalProperties_v2_request : public ALRPCRequest
  {
  public:
  
    SetGlobalProperties_v2_request(const SetGlobalProperties_v2_request& c);
    SetGlobalProperties_v2_request(void);
    
    virtual ~SetGlobalProperties_v2_request(void);
  
    SetGlobalProperties_v2_request& operator =(const SetGlobalProperties_v2_request&);
  
    bool checkIntegrity(void);

    const std::vector<TTSChunk_v2>* get_helpPrompt(void) const;
    const std::vector<TTSChunk_v2>* get_timeoutPrompt(void) const;
    const std::string* get_vrHelpTitle(void) const;
    const std::vector<VrHelpItem>* get_vrHelp(void) const;

    void reset_helpPrompt(void);
    bool set_helpPrompt(const std::vector<TTSChunk_v2>& helpPrompt_);
    void reset_timeoutPrompt(void);
    bool set_timeoutPrompt(const std::vector<TTSChunk_v2>& timeoutPrompt_);
    void reset_vrHelpTitle(void);
    bool set_vrHelpTitle(const std::string& vrHelpTitle_);
    void reset_vrHelp(void);
    bool set_vrHelp(const std::vector<VrHelpItem>& vrHelp_);

  private:
  
    friend class SetGlobalProperties_v2_requestMarshaller;


/**
     The help prompt.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk_v2>* helpPrompt;	//!<   [%s..%s] 

/**
     Help text for a wait timeout.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk_v2>* timeoutPrompt;	//!<   [%s..%s] 

/**
     VR Help Title text.
     If omitted on supported displays, the default SYNC help title shall be used.
*/
      std::string* vrHelpTitle;	//!< (500)

/**
     VR Help Items.
     If omitted on supported displays, the default SYNC generated help items shall be used.
     If the list of VR Help Items contains nonsequential positions (e.g. [1,2,4]), the RPC shall be rejected.
*/
      std::vector<VrHelpItem>* vrHelp;	//!<   [%s..%s] 
  };

}

#endif
