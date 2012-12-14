#ifndef NSRPC2COMMUNICATION_VR_GETLANGUAGE_INCLUDE
#define NSRPC2COMMUNICATION_VR_GETLANGUAGE_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class GetLanguage : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetLanguage(const GetLanguage& c);
      GetLanguage(void);
    
      GetLanguage& operator =(const GetLanguage&);
    
      virtual ~GetLanguage(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetLanguageMarshaller;


    };
  }
}

#endif
