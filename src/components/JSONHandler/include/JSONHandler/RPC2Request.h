#ifndef RPC2_REQUEST_CLASS
#define RPC2_REQUEST_CLASS value

class RPC2Request : public RPC2Command
{
public:
    RPC2Request( const std::string & methodName );
    virtual ~RPC2Request();

    virtual void setID( const std::string & id );
    virtual void setID( int id );

    virtual std::string getIDString() const;
    virtual int getID() const;
    /* data */
private:
    std::string mID;
};


#endif