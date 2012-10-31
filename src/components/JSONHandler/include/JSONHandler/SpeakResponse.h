#ifndef SPEAKRESPONSE_CLASS
#define SPEAKRESPONSE_CLASS

#include "JSONHandler/RPC2Response.h"

namespace RPC2Communication
{

    class SpeakResponse : public RPC2Response
    {
    public:
        SpeakResponse();
        SpeakResponse( int method );
        ~SpeakResponse();
        
    private:
        friend class SpeakResponseMarshaller;
    };

}

#endif