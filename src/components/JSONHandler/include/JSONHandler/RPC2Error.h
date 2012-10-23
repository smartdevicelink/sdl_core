#ifndef RPC2ERROR_CLASS
#define RPC2ERROR_CLASS value

#include <string>

namespace RPC2Communication
{    
    class RPC2Error
    {
    public:
        RPC2Error();
        virtual ~RPC2Error();

        virtual int getCode( ) const;
        virtual std::string getMessage() const;

        virtual void setCode( int code );
        virtual void setMessage( const std::string & message );

    private:
        int mCode;
        std::string mMessage;    
    };

}

#endif