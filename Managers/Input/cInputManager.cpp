#include "DXUT.h"
#include "cInputManager.h"

void cInputManager::Init()
{
}

void cInputManager::Update()
{
    if (GetFocus() != DXUTGetHWND())
        return;
    
    memcpy(m_OldKeys, m_CurKeys, sizeof(m_CurKeys));
    memset(m_CurKeys, 0, sizeof(m_CurKeys));

    for(int i = 0; i < 256; i++)
    {
        if (GetAsyncKeyState(i) & 0x8000)
        {
            m_CurKeys[i] = true;
        }
    }

    for (int playerIndex = 0; playerIndex < MAX_PLAYER; playerIndex++)
    {
        cCharacter* character = GAME->GetCharacter(playerIndex);
        std::vector<InputData>& inputBuffer = m_InputBuffer[playerIndex];
        std::string& stringInputBuffer = m_StringInputBuffer[playerIndex];
        short& gameInput = m_GameInput[playerIndex];

        short prevGameInput = gameInput;

        gameInput = 0;
        if (playerIndex == 0)
        {
            for (int i = 0; i < 16; i++)
            {
                if (m_CurKeys[m_GameInputBindings[i]])
                    gameInput |= 1 << i;
            }
        }
        else if (playerIndex == 1 && GGPO->GetIsOffline())
        {
            for (int i = 0; i < 16; i++)
            {
                if (m_CurKeys[m_GameInputBindings2P[i]])
                    gameInput |= 1 << i;
            }
        }
        else
        {
        }
        
        bool isInputed = false;
        InputData* lastInput = inputBuffer.empty() ? &InputData(IngameInput::None, 0, 0) : &inputBuffer[inputBuffer.size() - 1];
        for (int i = 0; i < 16; i++)
        {
            short bitMask = 1 << i;
            if ((gameInput & bitMask) != 0)
            {
                m_GameInputPressTimer[playerIndex][i]++;
                gameInput |= bitMask;

                // if (i == (int)IngameInput::Up && (prevGameInput & bitMask) != 0 && inputBuffer.empty())
                // {
                //     
                // }

                if ((prevGameInput & bitMask) == 0)
                {
                    if (lastInput->input == (IngameInput)i && lastInput->pressedAt + m_GameInputBufferedFrame[i] <= FRAME_TIMER)
                        lastInput->removeTimer = 0;
                
                    inputBuffer.emplace_back((IngameInput)i, m_GameInputBufferedFrame[i] > 5 ? -1 : m_GameInputBufferedFrame[i], FRAME_TIMER);
                    if (inputBuffer.size() > 10)
                        inputBuffer.erase(inputBuffer.begin());
                }
            }
            else
            {
                m_GameInputPressTimer[playerIndex][i] = 0;
            }
        }
        
        stringInputBuffer.clear();
        if (character == nullptr || character->GetHitStop() > 0)
            continue;
        
        for (auto iter = inputBuffer.begin(); iter != inputBuffer.end();)
        {
            if ((*iter).removeTimer < 0)
            {
                if (KeyUp(m_GameInputBindings[(short)(*iter).input]))
                    (*iter).removeTimer = m_GameInputBufferedFrame[(short)(*iter).input];
            }
            else
            {
                if ((*iter).removeTimer == 0)
                {
                    iter = inputBuffer.erase(iter);
                    continue;
                }

                (*iter).removeTimer--;

                if (isInputed && (*iter).input >= IngameInput::Up && (*iter).input <= IngameInput::Right)
                    (*iter).removeTimer += m_GameInputBufferedFrame[(short)(*iter).input] / 4;
            }
        
            stringInputBuffer.push_back(m_GameInputToNotations[(short)(*iter).input]);
            iter++;
        }
    }
    
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(DXUTGetHWND(), &pt);
    m_MousePos = Vec2(pt.x, pt.y);
    m_MousePos.x /= IMAGE->GetScreenScale().x;
    m_MousePos.y /= IMAGE->GetScreenScale().y;

    if (!m_ClickedGUIs.empty())
    {
        std::sort(m_ClickedGUIs.begin(), m_ClickedGUIs.end(), [](cGUI* a, cGUI* b)->bool{return a->GetOwner()->GetPos().z < b->GetOwner()->GetPos().z;});
        m_ClickedGUIs[0]->GetDoubleClickInterval() > 0 ? m_ClickedGUIs[0]->OnDoubleClick() : m_ClickedGUIs[0]->OnClick();
        for (int i = 1; i < m_ClickedGUIs.size(); i++)
        {
            m_ClickedGUIs[i]->OnClickOutSide();
        }

        m_ClickedGUIs.clear();
    }

    if (KeyPress(VK_LCONTROL))
    {
        if (KeyDown('Z') && !m_UndoQueue.empty())
        {
            m_UndoQueue.front().undo();
            m_RedoQueue.push_front(m_UndoQueue.front());
            m_UndoQueue.erase(m_UndoQueue.begin());
        }
        if (KeyDown('Y') && !m_RedoQueue.empty())
        {
            m_RedoQueue.front().redo();
            m_UndoQueue.push_front(m_RedoQueue.front());
            m_RedoQueue.erase(m_RedoQueue.begin());
        }
    }
}

void cInputManager::Render()
{
}

void cInputManager::HandleWindowInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    for (auto handler : m_MsgProcHandlers)
    {
        if (handler->MsgProcGetOwner()->IsActive())
            handler->MsgProc(uMsg, wParam, lParam);
    }
}

void cInputManager::Release()
{
}

void cInputManager::UnsubscribeMsgProcHandler(cMsgProcHandler* _handler)
{
    for (auto& iter = m_MsgProcHandlers.begin(); iter != m_MsgProcHandlers.end(); iter++)
    {
        if ((*iter) == _handler)
        {
            m_MsgProcHandlers.erase(iter);
            return;
        }
    }
}

bool cInputManager::SetClipboard(UINT _format, Serializer& _serializer)
{
    HANDLE mem = GlobalAlloc(GMEM_FIXED, _serializer.GetSize());
    void* data = GlobalLock(mem);
    if (data == nullptr)
        return false;

    UINT pointer = 0;
    _serializer.Serialize((char*)data, pointer);
    GlobalUnlock(mem);

    if (!OpenClipboard(DXUTGetHWND()))
        return false;

    EmptyClipboard();
    SetClipboardData(_format, mem);
    CloseClipboard();

    return true;
}

bool cInputManager::GetClipboard(UINT _format, Serializer& _serializer)
{
    if (_format != GetPriorityClipboardFormat(&_format, 1))
        return false;

    if (!OpenClipboard(DXUTGetHWND()))
        return false;

    HANDLE mem = GetClipboardData(_format);
    void* data = GlobalLock(mem);
    if (data == nullptr)
        return false;

    UINT pointer = 0;
    _serializer.Deserialize((char*)data, pointer);

    GlobalUnlock(data);
    CloseClipboard();

    return true;
}

void cInputManager::Do(const std::function<void()>& _do, const std::function<void()>& _undo)
{
    _do();
    m_UndoQueue.push_front(Action {_do, _undo});
    if (m_UndoQueue.size() > m_DoQueueMaxCount)
        m_UndoQueue.erase(--m_UndoQueue.end());

    m_RedoQueue.clear();
}

void cInputManager::ClearDoQueue()
{
    m_UndoQueue.clear();
    m_RedoQueue.clear();
}

void cInputManager::ClearInputBuffer(short _playerIndex)
{
    m_InputBuffer[_playerIndex].clear();
}

bool cInputManager::CheckInputBuffer(std::string _command, cCharacter* _character)
{
    if (Sign(_character->GetOwner()->GetScale().x) == -1)
    {
        for (int i = 0; i < _command.size(); i++)
        {
            if (_command[i] == '6')
                _command[i] = '4';
        }
    }

    short playerIndex = _character->GetPlayerIndex();
    size_t hasCommand = m_StringInputBuffer[playerIndex].find(_command);
    if (hasCommand != std::string::npos)
    {
        char inputDir = m_StringInputBuffer[playerIndex][hasCommand + _command.length() - 1];
        if (inputDir == '4')
            _character->SetDirection(-1);
        else if (inputDir == '6')
            _character->SetDirection(1);

        for (int i = hasCommand; i < hasCommand + _command.size(); i++)
        {
            //m_InputBuffer[i].removeTimer = INPUT_BUFFER_CLEAR_DELAY;
        }
        return true;
    }

    return false;
}
