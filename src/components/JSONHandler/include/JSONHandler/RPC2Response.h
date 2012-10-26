#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS value

#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/ALRPCObjects/Result.h"
#include "JSONHandler/RPC2Error.h"

namespace RPC2Communication
{
    class RPC2Response : public RPC2Command
    {
    public:
        RPC2Response( );
        RPC2Response( int method );
        virtual ~RPC2Response();

        virtual void setID( const std::string & id );
        virtual void setID( int id );

        virtual std::string getIDString() const;
        virtual int getID() const;

        virtual Result getResult() const;
        virtual void setResult(const Result& r);

        virtual RPC2Error getError() const;
        virtual void setError(const RPC2Error& error);

    private:
        std::string mID;
        Result mResultCode;
        RPC2Error mError;

    };

}

#endif