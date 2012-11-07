#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS

#include "RPC2Command.h"

namespace RPC2Communication
{
    class RPC2Response : public RPC2Command
    {
    public:
        RPC2Response( );
        RPC2Response(int method);
        RPC2Response(int method ,unsigned int id);
        RPC2Response(int method ,unsigned int id,int res);
        virtual ~RPC2Response();

        virtual unsigned int getId() const;
        virtual void setId(unsigned int id);

        virtual int getResult() const;
        virtual void setResult(int r);


    private:
        unsigned int mId;
        int mResultCode;

    };
}

#endif
