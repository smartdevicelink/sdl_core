//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_VR_DELETECOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_VR_DELETECOMMAND_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace VR
  {

    class DeleteCommand : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      DeleteCommand(const DeleteCommand& c);
      DeleteCommand(void);
    
      DeleteCommand& operator =(const DeleteCommand&);
    
      virtual ~DeleteCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_cmdId(void);

      int get_appId(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

      bool set_appId(int appId);


    private:

      friend class DeleteCommandMarshaller;

      unsigned int cmdId;
      int appId;

    };
  }
}

#endif
