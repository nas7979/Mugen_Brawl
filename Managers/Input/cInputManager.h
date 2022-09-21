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
    std::vector<InputData> m_InputBuffer[MAX_PLAYER];
    std::string m_StringInputBuffer[MAX_PLAYER];
    std::vector<cMsgProcHandler*> m_MsgProcHandlers;
    std::vector<cGUI*> m_ClickedGUIs;
    std::list<Action> m_UndoQueue;
    std::list<Action> m_RedoQueue;
    const int m_DoQueueMaxCount = 100;
    short m_GameInput[MAX_PLAYER];
    unsigned char m_GameInputBindings[16] {'W', 'A', 'S', 'D', 'H', 'J', 'K', 'L', 'G', };
    unsigned char m_GameInputBindings2P[16] {VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_OEM_PLUS, VK_NUMPAD6, };
    char m_GameInputToNotations[16] {'8', '4', '2', '6', 'a', 'b', 'c', 'd', 'r', };
    char m_GameInputBufferedFrame[16] {12, 12, 12, 12, 4, 4, 4, -1, -1, };
    short m_GameInputPressTimer[MAX_PLAYER][16] {{0, }, };

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
    
    void ClearInputBuffer(short _playerIndex);
    bool CheckInputBuffer(std::string _command, cCharacter* _character);
    bool CheckGameInput(IngameInput _input, short _playerIndex) const {return (m_GameInput[_playerIndex] & 1 << (short)_input) != 0;}
    short GetGameInputPressTimer(IngameInput _input, short _playerIndex) const {return m_GameInputPressTimer[_playerIndex][(short)_input];}
    int GetInputBufferSize(short _playerIndex) const {return m_InputBuffer[_playerIndex].size();}
    IngameInput GetLastBufferedInput(short _playerIndex) const {return m_InputBuffer[_playerIndex][m_InputBuffer[_playerIndex].size() - 1].input;}
};

#define INPUT cInputManager::GetInstance()

#define CF_STRING 1000
#define CF_SPRITE_BOX 1001
#define CF_SPRITE 1002