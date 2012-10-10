#ifndef RPC2COMMAND_CLASS
#define RPC2COMMAND_CLASS value

class RPC2Command
{
public:
    RPC2Command( std::string methodName );
    virtual ~RPC2Command();

    virtual void setMethodName( std::string methodName );
    virtual std::string getMethodName() const;

    /* data */
private:
    std::string mMethodName;
};

#endif