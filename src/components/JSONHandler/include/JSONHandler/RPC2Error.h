#ifndef RPC2COMMUNICATION_RPC2ERROR
#define RPC2COMMUNICATION_RPC2ERROR

#include <string>

#include "RPC2Command.h"

namespace RPC2Communication
{
    class RPC2Error : public RPC2Command
    {
    public:
	enum Codes
	{
          INVALID_REQUEST=-32600,
          METHOD_NOT_FOUND=-32601,
          INVALID_PARAMS=-32602,
          INTERNAL_ERROR=-32603
	};

        RPC2Error(void);
        RPC2Error(const RPC2Error& err);
        RPC2Error(int code, const std::string& message,int id);
        RPC2Error(int code, const std::string& message);
        virtual ~RPC2Error();

        int getId() const;
        void setId(const int);
        void resetId(void);

        int getErrorCode(void) const;
        void setErrorCode(int code);

        const std::string& getErrorString(void) const;
        void setErrorString(const std::string& message);

    private:
    //! setting mId to 0 will be result of "resetting", in such packets id passed as null (error only)
	int mId;
        int mCode;
        std::string mMessage;
        
        friend class RPC2ErrorMarshaller;
    };
}

#endif
