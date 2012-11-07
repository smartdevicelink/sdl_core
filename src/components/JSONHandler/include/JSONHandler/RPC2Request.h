#ifndef RPC2_REQUEST_CLASS
#define RPC2_REQUEST_CLASS

#include "RPC2Command.h"

namespace RPC2Communication
{  
    class RPC2Request : public RPC2Command
    {
    public:
        RPC2Request( );
        RPC2Request( int method );
        RPC2Request( int method ,unsigned int id);
        virtual ~RPC2Request();

        virtual unsigned int getId() const;
        virtual void setId(unsigned int id);        

    private:
//! mId should be >0
        unsigned int mId;
        
    };

}

#endif
