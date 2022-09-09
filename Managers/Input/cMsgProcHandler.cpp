#include "DXUT.h"
#include "cMsgProcHandler.h"

cMsgProcHandler::cMsgProcHandler()
{
    INPUT->SubscribeMsgProcHandler(this);
}

cMsgProcHandler::~cMsgProcHandler()
{
    INPUT->UnsubscribeMsgProcHandler(this);
}
