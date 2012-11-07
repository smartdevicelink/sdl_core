#ifndef RPC2COMMUNICATION_VR_ADDCOMMAND_INCLUDE
#define RPC2COMMUNICATION_VR_ADDCOMMAND_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace VR
  {

    class AddCommand : public ::RPC2Communication::RPC2Request
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


// setters
/// cmdId <= 2000000000
      bool set_cmdId(unsigned int cmdId);

/// vrCommands[] <= 99 ; 1 <= size <= 100
      bool set_vrCommands(const std::vector< std::string>& vrCommands);


    private:

      friend class AddCommandMarshaller;

      unsigned int cmdId;
      std::vector< std::string> vrCommands;

    };
  }
}

#endif
