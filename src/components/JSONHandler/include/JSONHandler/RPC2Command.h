#ifndef RPC2COMMAND_CLASS
#define RPC2COMMAND_CLASS value

#include <string>

namespace RPC2Communication
{

    class RPC2Command
    {
    public:
        enum CommandType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, UNDEFINED };
    public:
        RPC2Command( );
        RPC2Command( int method );
        virtual ~RPC2Command();

        virtual void setMethod( int method );
        virtual int getMethod() const;

        virtual void setCommandType( CommandType commandType );
        virtual CommandType getCommandType( ) const;

        /* data */
    private:
        int mMethod;
        CommandType mCommandType;
    };

}

#endif