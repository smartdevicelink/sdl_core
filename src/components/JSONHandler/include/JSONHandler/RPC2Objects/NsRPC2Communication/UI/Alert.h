#ifndef NSRPC2COMMUNICATION_UI_ALERT_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERT_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


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
      const unsigned int* get_duration(void);
      const bool* get_playTone(void);

// setters
/// AlertText1 <= 500
      bool set_AlertText1(const std::string& AlertText1);

      void reset_AlertText1(void);

/// AlertText2 <= 500
      bool set_AlertText2(const std::string& AlertText2);

      void reset_AlertText2(void);

/// 3000 <= duration <= 10000
      bool set_duration(const unsigned int& duration);

      void reset_duration(void);

      bool set_playTone(const bool& playTone);

      void reset_playTone(void);


    private:

      friend class AlertMarshaller;

      std::string* AlertText1;
      std::string* AlertText2;
      unsigned int* duration;
      bool* playTone;

    };
  }
}

#endif
