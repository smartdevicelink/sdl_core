#ifndef RPC2COMMAND_CLASS
#define RPC2COMMAND_CLASS

namespace RPC2Communication
{

    class RPC2Command
    {
    public:
        enum CommandType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, ERROR = 0x3, UNDEFINED };
    public:
        RPC2Command( );
        RPC2Command( CommandType type );
        RPC2Command( CommandType type, int method );
        virtual ~RPC2Command();

        virtual CommandType getCommandType( ) const;
        virtual void setCommandType( CommandType commandType );

        virtual int getMethod() const;
        virtual void setMethod( int method );
    protected:
        CommandType mCommandType;
        int mMethod;
    };

}

#endif
