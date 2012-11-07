#ifndef DELETECOMMAND_CLASS
#define DELETECOMMAND_CLASS

#include "JSONHandler/RPC2Request.h"
#include <string>

namespace RPC2Communication
{
    class DeleteCommand : public RPC2Request
    {
    public:
        DeleteCommand();
        ~DeleteCommand();

        void setCmdId(int id);
        int getCmdId() const;
    
    private:
        int cmdId;
        friend class DeleteCommandMarshaller;
    };
}

#endif