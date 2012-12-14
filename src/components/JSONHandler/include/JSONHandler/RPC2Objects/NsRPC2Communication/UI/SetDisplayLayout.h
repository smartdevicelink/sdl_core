#ifndef NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUT_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUT_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class SetDisplayLayout : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      SetDisplayLayout(const SetDisplayLayout& c);
      SetDisplayLayout(void);
    
      SetDisplayLayout& operator =(const SetDisplayLayout&);
    
      virtual ~SetDisplayLayout(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string& get_displayLayout(void);

      int get_appId(void);


// setters
      bool set_displayLayout(const std::string& displayLayout);

      bool set_appId(int appId);


    private:

      friend class SetDisplayLayoutMarshaller;

      std::string displayLayout;
      int appId;

    };
  }
}

#endif
