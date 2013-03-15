//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_TTS_GETSUPPORTEDLANGUAGES_INCLUDE
#define NSRPC2COMMUNICATION_TTS_GETSUPPORTEDLANGUAGES_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
*/

namespace NsRPC2Communication
{
  namespace TTS
  {

    class GetSupportedLanguages : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetSupportedLanguages(const GetSupportedLanguages& c);
      GetSupportedLanguages(void);
    
      GetSupportedLanguages& operator =(const GetSupportedLanguages&);
    
      virtual ~GetSupportedLanguages(void);
    
      bool checkIntegrity(void);
    

    private:

      friend class GetSupportedLanguagesMarshaller;


    };
  }
}

#endif
