#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_READDID_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_READDID_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    class ReadDID : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ReadDID(const ReadDID& c);
      ReadDID(void);
    
      ReadDID& operator =(const ReadDID&);
    
      virtual ~ReadDID(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_ecuName(void);

      const std::vector< unsigned int>& get_didLocation(void);

      const bool* get_encrypted(void);
      int get_appId(void);


// setters
/// ecuName <= 65535
      bool set_ecuName(unsigned int ecuName);

/// didLocation[] <= 65535 ; 0 <= size <= 1000
      bool set_didLocation(const std::vector< unsigned int>& didLocation);

      bool set_encrypted(const bool& encrypted);

      void reset_encrypted(void);

      bool set_appId(int appId);


    private:

      friend class ReadDIDMarshaller;

      unsigned int ecuName;
      std::vector< unsigned int> didLocation;
      bool* encrypted;
      int appId;

    };
  }
}

#endif
