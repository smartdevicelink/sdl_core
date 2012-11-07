#ifndef UIONCOMMAND_CLASS
#define UIONCOMMAND_CLASS

#include "JSONHandler/RPC2Notification.h"

namespace RPC2Communication
{
    class OnCommand : public RPC2Notification
    {
    public:
        OnCommand();
        ~OnCommand();

        void setCommandId(int s);
        int getCommandId() const;
    
    private:
        int commandId;
        friend class OnCommandMarshaller;
    };

}

#endif