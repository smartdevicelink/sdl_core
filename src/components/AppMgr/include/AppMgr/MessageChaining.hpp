//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef MESSAGE_CHAINING_CLASS
#define MESSAGE_CHAINING_CLASS

namespace NsAppManager
{
    class MessageChaining
    {
    public:
        MessageChaining(int connectionKey,
            unsigned int correlationID);
        ~MessageChaining();

        unsigned int correlationID;
        int connectionKey;
        bool success;
        //unsigned int rpcMessageId;
        int counter;
    };
}

#endif
