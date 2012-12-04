#ifndef NSRPC2COMMUNICATION_UI_ALERT_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERT_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V1/SoftButton.h"

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

    class Alert : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      Alert(const Alert& c);
      Alert(void);
    
      Alert& operator =(const Alert&);
    
      virtual ~Alert(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string* get_AlertText1(void);
      const std::string* get_AlertText2(void);
      const std::string* get_alertText3(void);
      const std::vector< NsAppLinkRPC::TTSChunk>* get_ttsChunks(void);
      const unsigned int* get_duration(void);
      const bool* get_playTone(void);
      const std::vector< NsAppLinkRPC::SoftButton>* get_softButtons(void);
      int get_appId(void);


// setters
/// AlertText1 <= 500
      bool set_AlertText1(const std::string& AlertText1);

      void reset_AlertText1(void);

/// AlertText2 <= 500
      bool set_AlertText2(const std::string& AlertText2);

      void reset_AlertText2(void);

/// alertText3 <= 500
      bool set_alertText3(const std::string& alertText3);

      void reset_alertText3(void);

/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< NsAppLinkRPC::TTSChunk>& ttsChunks);

      void reset_ttsChunks(void);

/// 3000 <= duration <= 10000
      bool set_duration(const unsigned int& duration);

      void reset_duration(void);

      bool set_playTone(const bool& playTone);

      void reset_playTone(void);

/// 0 <= size <= 4
      bool set_softButtons(const std::vector< NsAppLinkRPC::SoftButton>& softButtons);

      void reset_softButtons(void);

      bool set_appId(int appId);


    private:

      friend class AlertMarshaller;

      std::string* AlertText1;
      std::string* AlertText2;
      std::string* alertText3;
      std::vector< NsAppLinkRPC::TTSChunk>* ttsChunks;
      unsigned int* duration;
      bool* playTone;
      std::vector< NsAppLinkRPC::SoftButton>* softButtons;
      int appId;

    };
  }
}

#endif
