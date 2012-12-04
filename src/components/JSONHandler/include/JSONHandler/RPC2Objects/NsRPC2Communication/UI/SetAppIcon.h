#ifndef NSRPC2COMMUNICATION_UI_SETAPPICON_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETAPPICON_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


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

    class SetAppIcon : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SetAppIcon(const SetAppIcon& c);
      SetAppIcon(void);
    
      SetAppIcon& operator =(const SetAppIcon&);
    
      virtual ~SetAppIcon(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_syncFileName(void);

      int get_appId(void);


// setters
/// syncFileName <= 500
      bool set_syncFileName(const std::string& syncFileName);

      bool set_appId(int appId);


    private:

      friend class SetAppIconMarshaller;

      std::string syncFileName;
      int appId;

    };
  }
}

#endif
