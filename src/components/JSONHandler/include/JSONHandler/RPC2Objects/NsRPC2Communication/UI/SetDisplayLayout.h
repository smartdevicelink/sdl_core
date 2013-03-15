//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUT_INCLUDE
#define NSRPC2COMMUNICATION_UI_SETDISPLAYLAYOUT_INCLUDE

#include <string>
#include "JSONHandler/RPC2Request.h"


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
