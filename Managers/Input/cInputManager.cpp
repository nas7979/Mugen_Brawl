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

    m_GameInput = 0;
    bool isInputed = false;
    InputData* lastInput = m_InputBuffer.empty() ? &InputData(IngameInput::None, 0, 0) : &m_InputBuffer[m_InputBuffer.size() - 1];
    for (int i = 0; i < 16; i++)
    {
        if (m_CurKeys[m_GameInputBindings[i]])
        {
            m_GameInputPressTimer[i]++;
            m_GameInput |= 1 << i;

            if (!m_OldKeys[m_GameInputBindings[i]])
            {
                if (lastInput->input == (IngameInput)i && lastInput->pressedAt + m_GameInputBufferedFrame[i] <= FRAME_TIMER)
                    lastInput->removeTimer = 0;
                
                m_InputBuffer.emplace_back((IngameInput)i, m_GameInputBufferedFrame[i] > 10 ? -1 : m_GameInputBufferedFrame[i], FRAME_TIMER);
                if (m_InputBuffer.size() > 10)
                    m_InputBuffer.erase(m_InputBuffer.begin());
            }
        }
        else
        {
            m_GameInputPressTimer[i] = 0;
        }
    }

    m_StringInputBuffer.clear();
    for (auto iter = m_InputBuffer.begin(); iter != m_InputBuffer.end();)
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
                iter = m_InputBuffer.erase(iter);
                continue;
            }

            (*iter).removeTimer--;

            if (isInputed && (*iter).input >= IngameInput::Up && (*iter).input <= IngameInput::Right)
                (*iter).removeTimer += m_GameInputBufferedFrame[(short)(*iter).input] / 4;
        }
        
        m_StringInputBuffer.push_back(m_GameInputToNotations[(short)(*iter).input]);
        iter++;
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

void cInputManager::ClearInputBuffer()
{
    m_InputBuffer.clear();
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

    size_t hasCommand = m_StringInputBuffer.find(_command);
    if (hasCommand != std::string::npos)
    {
        char inputDir = m_StringInputBuffer[hasCommand + _command.length() - 1];
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
