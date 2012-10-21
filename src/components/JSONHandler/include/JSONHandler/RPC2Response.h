#ifndef RPC2_RESPONSE_CLASS
#define RPC2_RESPONSE_CLASS value

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

        /* data */
    private:
        std::string mID;

    };

}

#endif