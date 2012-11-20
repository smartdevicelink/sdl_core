#ifndef NSRPC2COMMUNICATION_UI_SHOW_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOW_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/TextAlignment.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class Show : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      Show(const Show& c);
      Show(void);
    
      Show& operator =(const Show&);
    
      virtual ~Show(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string* get_mainField1(void);
      const std::string* get_mainField2(void);
      const NsAppLinkRPC::TextAlignment* get_alignment(void);
      const std::string* get_statusBar(void);
      const std::string* get_mediaClock(void);
      const std::string* get_mediaTrack(void);

// setters
/// mainField1 <= 500
      bool set_mainField1(const std::string& mainField1);

      void reset_mainField1(void);

/// mainField2 <= 500
      bool set_mainField2(const std::string& mainField2);

      void reset_mainField2(void);

      bool set_alignment(const NsAppLinkRPC::TextAlignment& alignment);

      void reset_alignment(void);

/// statusBar <= 500
      bool set_statusBar(const std::string& statusBar);

      void reset_statusBar(void);

/// mediaClock <= 500
      bool set_mediaClock(const std::string& mediaClock);

      void reset_mediaClock(void);

/// mediaTrack <= 500
      bool set_mediaTrack(const std::string& mediaTrack);

      void reset_mediaTrack(void);


    private:

      friend class ShowMarshaller;

      std::string* mainField1;
      std::string* mainField2;
      NsAppLinkRPC::TextAlignment* alignment;
      std::string* statusBar;
      std::string* mediaClock;
      std::string* mediaTrack;

    };
  }
}

#endif
