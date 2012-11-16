#ifndef ONDRIVERDISTRACTIONMARSHALLER_H
#define ONDRIVERDISTRACTIONMARSHALLER_H

#include "JSONHandler/RPC2Objects/UI/OnDriverDistraction.h"
#include <json/json.h>
#include <string>

namespace RPC2Communication
{
  namespace UI
  {
    class OnDriverDistractionMarshaller
    {
    public:
      static bool checkIntegrity(OnDriverDistraction& e);
      static bool checkIntegrityConst(const OnDriverDistraction& e);

      static bool fromString(const std::string& s,OnDriverDistraction& e);
      static const std::string toString(const OnDriverDistraction& e);

      static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
      static Json::Value toJSON(const OnDriverDistraction& e);
    };
  }
}

#endif // ONDRIVERDISTRACTIONMARSHALLER_H
