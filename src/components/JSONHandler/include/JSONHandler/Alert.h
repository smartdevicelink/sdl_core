#ifndef ALERT_CLASS
#define ALERT_CLASS

#include "JSONHandler/RPC2Request.h"
#include <string>

namespace RPC2Communication {     

    class Alert : public RPC2Request     
    {
    public:         
        Alert();         
        ~Alert();

        const std::string * getAlertText1() const;
        const std::string * getAlertText2() const;
        int * getDuration() const;
        bool * getPlayTone() const;

        void setAlertText1( const std::string & str );
        void setAlertText2( const std::string & str );
        void setDuration( int duration );
        void setPlayTone( bool playTone );

    private:
        std::string * mAlertText1;
        std::string * mAlertText2;
        int  * mDuration;
        bool * mPlayTone;
       friend class AlertMarshaller;
    
    };

}
#endif