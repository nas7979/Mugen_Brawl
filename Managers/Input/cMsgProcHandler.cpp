#include "DXUT.h"
#include "cMsgProcHandler.h"

cMsgProcHandler::cMsgProcHandler(cObject* _owner)
{
    m_MsgProcOwner = _owner;
    INPUT->SubscribeMsgProcHandler(this);
}

cMsgProcHandler::~cMsgProcHandler()
{
    INPUT->UnsubscribeMsgProcHandler(this);
}
