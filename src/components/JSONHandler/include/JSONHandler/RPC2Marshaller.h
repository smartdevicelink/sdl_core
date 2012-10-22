#ifndef RPC2MARSHALLER_CLASS
#define RPC2MARSHALLER_CLASS

#include <json/value.h>
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/OnButtonEventMarshaller.h"

namespace RPC2Communication
{

    class RPC2Marshaller
    {
    public:
        enum Methods
        {
        METHOD_INVALID=-1,
        METHOD_ONBUTTONEVENT=0,
        METHOD_REQUEST1_REQUEST=1,
        METHOD_REQUEST1_RESPONSE=2
        };

        RPC2Marshaller();
        ~RPC2Marshaller();

        static RPC2Command* fromJSON(const Json::Value&);
        static Json::Value toJSON(const RPC2Command* msg);

        static RPC2Command* fromString(const std::string&);
        static std::string toString(const RPC2Command* msg);

        static bool fromString(const std::string& str, OnButtonEvent& res) {
        return mOnButtonEventMarshaller.fromString(str, res);
      }

      static std::string toString(const OnButtonEvent& res)  {
        return mOnButtonEventMarshaller.toString(res);
      }

    private:
        struct localHash
        {
            const char *name;
            unsigned int idx;
            void* marshaller;
        };

        static const char* getName(Methods e)
        {
            return (e>=0 && e<4) ? mHashTable[e].name : NULL;
        }

        static const Methods getIndex(const char* s);

        static const localHash mHashTable[3];

        static OnButtonEventMarshaller mOnButtonEventMarshaller;

    };

}

#endif