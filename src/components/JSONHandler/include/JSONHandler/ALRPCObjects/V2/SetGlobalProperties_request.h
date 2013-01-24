#ifndef NSAPPLINKRPCV2_SETGLOBALPROPERTIES_REQUEST_INCLUDE
#define NSAPPLINKRPCV2_SETGLOBALPROPERTIES_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "TTSChunk.h"
#include "VrHelpItem.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Allows setting global properties.

  class SetGlobalProperties_request : public NsAppLinkRPC::ALRPCMessage
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


/**
     The help prompt.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* helpPrompt;	//!<   [%s..%s] 

/**
     Help text for a wait timeout.
     An array of text chunks of type TTSChunk. See TTSChunk
     The array must have at least one item
*/
      std::vector<TTSChunk>* timeoutPrompt;	//!<   [%s..%s] 

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
