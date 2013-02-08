#include "AppMgr/MessageChaining.hpp"

using namespace NsAppManager;

MessageChaining::MessageChaining(int _connectionKey,
            unsigned int _correlationID):
 correlationID(_correlationID)
,connectionKey(_connectionKey)
,success(true)
//,rpcMessageId(_rpcMessageId)
,counter(1)
{}        

MessageChaining::~MessageChaining()
{
    counter = 0;
}