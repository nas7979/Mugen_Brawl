#pragma once

class cMsgProcHandler
{
public:
    cMsgProcHandler();
    virtual ~cMsgProcHandler();
    
    virtual void MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
