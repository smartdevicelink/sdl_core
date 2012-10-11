#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS value

class RPC2Response : public RPC2Command
{
public:
    RPC2Response( const std::string & methodName );
    virtual ~RPC2Response();

    virtual void setID( const std::string & id );
    virtual void setID( int id );

    virtual std::string getIDString() const;
    virtual int getID() const;

    /* data */
private:
    std::string mID;

};


#endif