#pragma once
#include "cMsgProcHandler.h"
#include "../../Components/GUI/cGUI.h"

class cCharacter;

enum class IngameInput : short
{
    Up,
    Left,
    Down,
    Right,
    A,
    B,
    C,
    Shield,
    Cancel,
    None
};

class cInputManager : public cSingleton<cInputManager>
{
    struct Action
    {
        std::function<void()> redo;
        std::function<void()> undo;
    };

    struct InputData
    {
        IngameInput input;
        short removeTimer;
        int pressedAt;

        InputData(IngameInput _input, short _removeTimer, int _pressedAt)
        {
            input = _input;
            removeTimer = _removeTimer;
            pressedAt = _pressedAt;
        }
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
    std::vector<InputData> m_InputBuffer;
    std::string m_StringInputBuffer;
    std::vector<cMsgProcHandler*> m_MsgProcHandlers;
    std::vector<cGUI*> m_ClickedGUIs;
    std::list<Action> m_UndoQueue;
    std::list<Action> m_RedoQueue;
    const int m_DoQueueMaxCount = 100;
    short m_GameInput = 0;
    char m_GameInputBindings[16] {'W', 'A', 'S', 'D', 'H', 'J', 'K', 'L', 'G', };
    char m_GameInputToNotations[16] {'8', '4', '2', '6', 'a', 'b', 'c', 'd', 'r', };
    char m_GameInputBufferedFrame[16] {12, 12, 12, 12, 4, 4, 4, -1, -1, };
    short m_GameInputPressTimer[16] {0, };

public:
    bool KeyDown(int _key) const {return m_CurKeys[_key] && !m_OldKeys[_key];}
    bool KeyPress(int _key) const {return m_CurKeys[_key] || m_OldKeys[_key];}
    bool KeyUp(int _key) const {return !m_CurKeys[_key] && m_OldKeys[_key];}
    Vec2 GetMousePos() const {return m_MousePos;}
    void SubscribeMsgProcHandler(cMsgProcHandler* _handler) {m_MsgProcHandlers.push_back(_handler);}
    void UnsubscribeMsgProcHandler(cMsgProcHandler* _handler);
    void AddToClickedGUIs(cGUI* _gui) {m_ClickedGUIs.push_back(_gui);}

    bool SetClipboard(UINT _format, Serializer& _serializer);
    bool GetClipboard(UINT _format, Serializer& _serializer);
    void Do(const std::function<void()>& _do, const std::function<void()>& _undo);
    void ClearDoQueue();
    
    void ClearInputBuffer();
    bool CheckInputBuffer(std::string _command, cCharacter* _character);
    short GetGameInput() const {return m_GameInput;}
    bool CheckGameInput(IngameInput _input) const {return (m_GameInput & 1 << (short)_input) != 0;}
    short GetGameInputPressTimer(IngameInput _input) const {return m_GameInputPressTimer[(short)_input];}
    int GetInputBufferSize() const {return m_InputBuffer.size();}
    IngameInput GetLastBufferedInput() const {return m_InputBuffer[m_InputBuffer.size() - 1].input;}
};

#define INPUT cInputManager::GetInstance()

#define CF_STRING 1000
#define CF_SPRITE_BOX 1001
#define CF_SPRITE 1002