#pragma once

class cMsgProcHandler
{
public:
    cMsgProcHandler(cObject* _owner);
    virtual ~cMsgProcHandler();

private:
    cObject* m_MsgProcOwner;
    
public:
    cObject* MsgProcGetOwner() const {return m_MsgProcOwner;}
    virtual void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
