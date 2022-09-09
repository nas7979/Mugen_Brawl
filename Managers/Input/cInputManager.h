#pragma once
#include "cMsgProcHandler.h"
#include "../../Components/GUI/cGUI.h"

class cInputManager : public cSingleton<cInputManager>
{
    struct Action
    {
        std::function<void()> redo;
        std::function<void()> undo;
    };
    
public:
    void Init();
    void Update();
    void Render();
    void HandleWindowInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void Release();

private:
    bool m_CurKeys[256];
    bool m_OldKeys[256];
    Vec2 m_MousePos;
    std::string m_InputBuffer;
    int m_InputBufferClearTimer = 0;
    const int INPUT_BUFFER_CLEAR_DELAY = 6;
    std::string m_WindowInputBuffer;
    std::vector<cMsgProcHandler*> m_MsgProcHandlers;
    std::vector<cGUI*> m_ClickedGUIs;
    std::list<Action> m_UndoQueue;
    std::list<Action> m_RedoQueue;
    const int m_DoQueueMaxCount = 100;

public:
    bool KeyDown(int _key) {return m_CurKeys[_key] && !m_OldKeys[_key];}
    bool KeyPress(int _key) {return m_CurKeys[_key] || m_OldKeys[_key];}
    bool KeyUp(int _key) {return !m_CurKeys[_key] && m_OldKeys[_key];}
    Vec2 GetMousePos() {return m_MousePos;}
    std::string GetWindowInputBuffer() {return m_WindowInputBuffer;}
    void SubscribeMsgProcHandler(cMsgProcHandler* _handler) {m_MsgProcHandlers.push_back(_handler);}
    void UnsubscribeMsgProcHandler(cMsgProcHandler* _handler);
    void AddToClickedGUIs(cGUI* _gui) {m_ClickedGUIs.push_back(_gui);}

    bool SetClipboard(UINT _format, Serializer& _serializer);
    bool GetClipboard(UINT _format, Serializer& _serializer);
    void Do(const std::function<void()>& _do, const std::function<void()>& _undo);
    void ClearDoQueue();
    
    void ReadAndClearInputBuffer();
};

#define INPUT cInputManager::GetInstance()

#define CF_STRING 1000
#define CF_SPRITE_BOX 1001
#define CF_SPRITE 1002