#pragma once
#include "cMsgProcHandler.h"
#include "../../Components/GUI/cGUI.h"

enum class IngameInput
{
    Up = 0x0001,
    Left = 0x0002,
    Down = 0x0004,
    Right = 0x0008,
    A = 0x0010,
    B = 0x0020,
    C = 0x0040,
    Shield = 0x0080,
    Cancel = 0x0100
};

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
    short m_GameInput = 0;
    char m_GameInputBindings[16] {'W', 'A', 'S', 'D', 'H', 'J', 'K', 'L', 'G', };

public:
    bool KeyDown(int _key) const {return m_CurKeys[_key] && !m_OldKeys[_key];}
    bool KeyPress(int _key) const {return m_CurKeys[_key] || m_OldKeys[_key];}
    bool KeyUp(int _key) const {return !m_CurKeys[_key] && m_OldKeys[_key];}
    Vec2 GetMousePos() const {return m_MousePos;}
    std::string GetWindowInputBuffer() {return m_WindowInputBuffer;}
    void SubscribeMsgProcHandler(cMsgProcHandler* _handler) {m_MsgProcHandlers.push_back(_handler);}
    void UnsubscribeMsgProcHandler(cMsgProcHandler* _handler);
    void AddToClickedGUIs(cGUI* _gui) {m_ClickedGUIs.push_back(_gui);}

    bool SetClipboard(UINT _format, Serializer& _serializer);
    bool GetClipboard(UINT _format, Serializer& _serializer);
    void Do(const std::function<void()>& _do, const std::function<void()>& _undo);
    void ClearDoQueue();
    
    void ReadAndClearInputBuffer();
    short GetGameInput() const {return m_GameInput;}
    bool CheckGameInput(IngameInput _input) const {return (m_GameInput & (short)_input) != 0;}
};

#define INPUT cInputManager::GetInstance()

#define CF_STRING 1000
#define CF_SPRITE_BOX 1001
#define CF_SPRITE 1002