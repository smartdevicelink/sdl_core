#ifndef SENDDATA_H
#define SENDDATA_H

#include <vector>
#include <string>
#include "JSONHandler/RPC2Request.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    class SendData : public ::RPC2Communication::RPC2Request
    {
    public:

      SendData(const SendData& c);
      SendData(void);

      SendData& operator =(const SendData&);

      virtual ~SendData(void);

      bool checkIntegrity(void);

// getters

      const std::vector< std::string >& get_data(void);


// setters

/// 1 <= size <= 100
      bool set_data(const std::vector< std::string >& data);

    private:

      friend class SendDataMarshaller;

      std::vector< std::string > data;

    };
  }
}

#endif // SENDDATA_H
