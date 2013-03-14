#ifndef NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSE_INCLUDE
#define NSRPC2COMMUNICATION_UI_PERFORMINTERACTIONRESPONSE_INCLUDE

#include "JSONHandler/RPC2Response.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TriggerSource.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class PerformInteractionResponse : public ::NsRPC2Communication::RPC2Response
    {
    public:
    
      PerformInteractionResponse(const PerformInteractionResponse& c);
      PerformInteractionResponse(void);
    
      PerformInteractionResponse& operator =(const PerformInteractionResponse&);
    
      virtual ~PerformInteractionResponse(void);
    
      bool checkIntegrity(void);
    
// getters
      const unsigned int* get_choiceID(void);
      const NsSmartDeviceLinkRPCV2::TriggerSource* get_triggerSource(void);

// setters
/// choiceID <= 2000000000
      bool set_choiceID(const unsigned int& choiceID);

      void reset_choiceID(void);

      bool set_triggerSource(const NsSmartDeviceLinkRPCV2::TriggerSource& triggerSource);

      void reset_triggerSource(void);


    private:

      friend class PerformInteractionResponseMarshaller;

      unsigned int* choiceID;
      NsSmartDeviceLinkRPCV2::TriggerSource* triggerSource;

    };
  }
}

#endif
