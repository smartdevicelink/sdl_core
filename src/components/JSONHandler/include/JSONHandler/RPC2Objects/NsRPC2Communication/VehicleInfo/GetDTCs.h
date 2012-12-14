#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCS_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCS_INCLUDE

#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    class GetDTCs : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      GetDTCs(const GetDTCs& c);
      GetDTCs(void);
    
      GetDTCs& operator =(const GetDTCs&);
    
      virtual ~GetDTCs(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_ecuName(void);

      const bool* get_encrypted(void);
      int get_appId(void);


// setters
/// ecuName <= 65535
      bool set_ecuName(unsigned int ecuName);

      bool set_encrypted(const bool& encrypted);

      void reset_encrypted(void);

      bool set_appId(int appId);


    private:

      friend class GetDTCsMarshaller;

      unsigned int ecuName;
      bool* encrypted;
      int appId;

    };
  }
}

#endif
