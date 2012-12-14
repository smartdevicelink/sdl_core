#ifndef NSRPC2COMMUNICATION_VR_ADDCOMMAND_INCLUDE
#define NSRPC2COMMUNICATION_VR_ADDCOMMAND_INCLUDE

#include <string>
#include <vector>
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

    class AddCommand : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      AddCommand(const AddCommand& c);
      AddCommand(void);
    
      AddCommand& operator =(const AddCommand&);
    
      virtual ~AddCommand(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_cmdId(void);

      const std::vector< std::string>& get_vrCommands(void);

      int get_appId(void);


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

/// vrCommands[] <= 99 ; 1 <= size <= 100
      bool set_vrCommands(const std::vector< std::string>& vrCommands);

      bool set_appId(int appId);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      std::vector< std::string> vrCommands;
      int appId;

    };
  }
}

#endif
