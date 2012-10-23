#ifndef GETCAPABILITIES_CLASS
#define GETCAPABILITIES_CLASS

#include "JSONHandler/RPC2Request.h"

namespace RPC2Communication {     

    class GetCapabilities : public RPC2Request
        {
        public:
            GetCapabilities();
            ~GetCapabilities();
        
        private:
            friend class GetCapabilitiesMarshaller;
        };    
}

#endif
